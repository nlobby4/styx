#define _GNU_SOURCE
#include "response.h"
#include "globals.h"
#include "state.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define HEADER(bufs) (&bufs->resp.head)
#define BODY(bufs) (&bufs->resp.body)
#define FORMAT_HEADER(http_code, msg) ("HTTP/1.1 " #http_code " " msg "\r\n")
#define HEADER_FIELD(field, content) (field ": " content "\r\n")
#define SEND(buf, result, length)                                             \
  do                                                                          \
    {                                                                         \
      length = strlen (buf);                                                  \
      result = send (connection, buf, length * sizeof (char), 0);             \
      if (result != length)                                                   \
        return false;                                                         \
    }                                                                         \
  while (0)

static bool
send_response (message *response)
{
  ssize_t res = 0;
  ssize_t len = 0;
  SEND (response->head.payload, res, len);
  SEND (response->body.payload, res, len);
  printf ("message sent:\n%s%s", response->head.payload,
          response->body.payload);

  return true;
}

static bool
copy_into_buffer (buffer *buff, const char *msg)
{
  if ((size_t)buff->size <= strlen (msg))
    {
      warning ("message too big for buffer");
      return false;
    }
  strcat (buff->payload, msg);
  return true;
}

static const char *
get_mime_type (const char *path)
{
  if (!strcmp ("/", path))
    return "text/html";
  static const struct
  {
    const char *key;
    const char *value;
  } mime_types[] = { { .key = "html", .value = "text/html" },
                     { .key = "htm", .value = "text/html" },
                     { .key = "css", .value = "text/css" },
                     { .key = "js", .value = "text/javascript" },
                     { .key = "mjs", .value = "text/javascript" },
                     { .key = "avif", .value = "image/avif" },
                     { .key = "ico", .value = "image/vnd.microsoft.icon" },
                     { .key = "jpeg", .value = "image/jpeg" },
                     { .key = "jpg", .value = "image/jpeg" },
                     { .key = "mp3", .value = "audio/mpeg" },
                     { .key = "mp4", .value = "video/mp4" },
                     { .key = "mpeg", .value = "video/mpeg" },
                     { .key = "otf", .value = "font/otf" },
                     { .key = "ttf", .value = "font/ttf" },
                     { .key = "png", .value = "image/png" },
                     { .key = "pdf", .value = "application/pdf" },
                     { .key = "svg", .value = "image/svg+xml" },
                     { .key = "txt", .value = "text/plain" },
                     { .key = "wav", .value = "audio/wav" },
                     { .key = "weba", .value = "audio/webm" },
                     { .key = "webm", .value = "video/webm" },
                     { .key = "webp", .value = "image/webp" },
                     { .key = "xml", .value = "application/xml" },
                     { .key = "json", .value = "application/json" },
                     { .key = "jsonld", .value = "application/ld+json" },
                     { .key = "gif", .value = "image/gif" } };
  const char *file_name;
  const char *file_ending;
  if ((file_name = strrchr (path, '\\')) == NULL || *(++file_name) == '\0'
      || (file_ending = strrchr (file_name, '.')) == NULL
      || *(++file_ending) == '\0')
    return "application/octet-stream";
  for (size_t i = 0; i < (sizeof (mime_types) / sizeof (*mime_types)); ++i)
    {
      if (!strcasecmp (file_ending, mime_types[i].key))
        return mime_types[i].value;
    }
  return "application/octet-stream";
}

static status
read_file (buffer *body, const char *path)
{
  char correct_path[sizeof (char) * 7 + strlen (path) + 1];
  memset (correct_path, 0, sizeof (correct_path));
  strcpy (correct_path, "static/");
  strcat (correct_path, path);
  status s;
  if (!strcmp ("/", path))
    {
      s = read_file (body, "index.html");
      return s == NOT_FOUND ? read_file (body, "index.htm") : s;
    }

  FILE *fp = fopen (correct_path, "r");
  if (fp == NULL)
    {
      warning ("cannot open %s", correct_path);
      return NOT_FOUND;
    }
  if (fseek (fp, 0, SEEK_END) == -1)
    {
      fclose (fp);
      warning ("cannot find end of %s", correct_path);
      return INTERNAL_SERVER_ERROR;
    }
  long file_size = ftell (fp);
  if (file_size <= 0)
    {
      fclose (fp);
      warning ("cannot read %s", correct_path);
      return INTERNAL_SERVER_ERROR;
    }
  if (file_size >= body->size)
    {
      fclose (fp);
      warning ("%s too big to be sent", correct_path);
      return INSUFFICIENT_STORAGE;
    }
  rewind (fp);
  size_t res = fread (body->payload, file_size, 1, fp);
  fclose (fp);
  if (res != 1)
    {
      warning ("%s cannot be read correctly", correct_path);
      return INTERNAL_SERVER_ERROR;
    }
  return OK;
}

void
response (message_buffers *bufs, header_data *request_data,
          connection_state *state)
{
  static const char *error_html
      = "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "\t<meta charset=\"UTF-8\">\n"
        "\t<meta name=\"viewport\" content=\"width=device-width, "
        "initial-scale=1.0\">\n"
        "\t<title>%d %s</title>\n"
        "</head>\n"
        "<body>\n"
        "\t<h1>An error has occured: %d %s</h1>\n"
        "</body>\n"
        "</html>\n";
  bool result = false;
  char format_str[BUFSIZ] = { '\0' };
  char temp[BUFSIZ] = { '\0' };
  int current_size = BUFSIZ - 1;
  if (state->code == NOT_PROCESSED)
    {
      state->code = read_file (&bufs->resp.body, request_data->path);
      if (state->code == OK)
        {
          current_size -= snprintf (format_str, current_size,
                                    "Content-Type: %s\r\n"
                                    "Content-Length: %lu\r\n",
                                    get_mime_type (request_data->path),
                                    strlen (bufs->resp.body.payload));

          if (current_size < 0)
            {
              warning ("format buffer size exceeded");
              state->code = INTERNAL_SERVER_ERROR;
            }
          if (state->current_request == state->max_requests
              && state->keep_alive)
            current_size
                -= snprintf (temp, current_size,
                             "Connection: Keep-Alive\r\n"
                             "Keep-Alive: timeout=%ld, max=%d\r\n",
                             state->timeout.tv_sec, state->max_requests);

          if (current_size < 0)
            {
              warning ("format buffer size exceeded");
              state->code = INTERNAL_SERVER_ERROR;
            }
          else
            strcat (format_str, temp);
        }
    }
  const char *code_msg;
  switch (state->code)
    {
    case CONTENT_TOO_LARGE:
      result = copy_into_buffer (HEADER (bufs),
                                 FORMAT_HEADER (413, "Payload Too Large"))
               && copy_into_buffer (HEADER (bufs), "\r\n");
      code_msg = "Payload Too Large";
      break;
    case REQU_HEAD_FIELDS_TOO_LARGE:
      result = copy_into_buffer (
                   HEADER (bufs),
                   FORMAT_HEADER (431, "Request Header Fields Too Large"))
               && copy_into_buffer (HEADER (bufs), "\r\n");
      code_msg = "Request Header Fields Too Large";
      break;
    case NOT_IMPLEMENTED:
      result = copy_into_buffer (HEADER (bufs),
                                 FORMAT_HEADER (501, "Not Implemented"))
               && copy_into_buffer (HEADER (bufs), "\r\n");
      code_msg = "Not Implemented";
      break;
    case NOT_FOUND:
      result
          = copy_into_buffer (HEADER (bufs), FORMAT_HEADER (404, "Not Found"))
            && copy_into_buffer (HEADER (bufs), "\r\n");
      code_msg = "Not Found";
      break;
    case BAD_REQUEST:
      result = copy_into_buffer (HEADER (bufs),
                                 FORMAT_HEADER (400, "Bad Request"))
               && copy_into_buffer (HEADER (bufs), "\r\n");
      code_msg = "Bad Request";
      break;
    case INSUFFICIENT_STORAGE:
      result = copy_into_buffer (HEADER (bufs),
                                 FORMAT_HEADER (507, "Insufficient Storage"))
               && copy_into_buffer (HEADER (bufs), "\r\n");
      code_msg = "Insufficient Storage";
      break;
    case OK:
      result = copy_into_buffer (HEADER (bufs), FORMAT_HEADER (200, "Ok"))

               && copy_into_buffer (HEADER (bufs), format_str)
               && copy_into_buffer (HEADER (bufs), "\r\n");
      if (!strcmp ("HEAD", request_data->method))
        *bufs->resp.body.payload = '\0';
      break;
    case INTERNAL_SERVER_ERROR:
    default:
      result = copy_into_buffer (HEADER (bufs),
                                 FORMAT_HEADER (500, "Internal Server Error"))
               && copy_into_buffer (HEADER (bufs), "\r\n");
      code_msg = "Internal Server Error";
#ifdef DEBUG
      if (state->code != INTERNAL_SERVER_ERROR)
        warning ("invalid status state->code %d", state->code);
#endif
    }
  if (state->code != OK)
    {
      memset (format_str, 0, BUFSIZ);
      sprintf (format_str, error_html, state->code, code_msg, state->code,
               code_msg);
      result = result && copy_into_buffer (BODY (bufs), format_str);
    }
  if (result && !send_response (&bufs->resp))
    warning ("response not sent properly");
}