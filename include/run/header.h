/**
 * @file header.h
 * @author -
 * @brief Contains all data structures and functions required to parse a
 * HTTP-request header and utilize said data structures.
 * @version 1.0.1
 * @date 07/03/2025
 */
#ifndef _HEADER_H
#define _HEADER_H

#include <stddef.h>

/**
 * @brief Represents a single HTTP header key-value pair.
 */
typedef struct
{
  char *key;   /**< The header key. */
  char *value; /**< The header value. */
} header;

/**
 * @brief Represents a vector of HTTP headers.
 */
typedef struct
{
  header *h_array; /**< An Array of headers. */
  size_t length;   /**< Number of headers contained the array. */
  size_t capacity; /**< Capacity of the array. */
} header_vec;

/**
 * @brief Used for storing the parsed HTTP header data from the client's
 * request.
 */
typedef struct
{
  char *method;     /**< HTTP method (this implementation only supports GET and
                       HEAD). */
  char *path;       /**< The requested file path. */
  char *version;    /**< The HTTP version. */
  header_vec h_vec; /**< Header vector. */
} header_data;

/**
 * @brief Parses a raw HTTP request header string into a header_data structure.
 *
 * @param header_str The raw header string to parse, which is modified
 * in-place.
 * @return A pointer to the parsed header_data structure.
 * @note The returned pointer is to a singleton. If another request is parsed,
 * the singleton is overwritten.
 */
header_data *parse (char *header_str);

/**
 * @brief Frees the memory allocated inside the header_data singleton.
 *
 * @param data The header_data structure to free.
 */
void free_data (header_data *data);

/**
 * @brief Looks up a header value by key in a header_vec.
 *
 * @param vec The header_vec to search.
 * @param key The key to look up.
 * @return The value associated with the key, or NULL if not found.
 */
char *vec_lookup (header_vec *vec, const char *key);

#endif