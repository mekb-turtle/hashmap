/**
 * \file fnv1_64.h
 * \brief FNV-1(a) 64-bit hash function.
 */

#ifndef _FNV1_64_H
#define _FNV1_64_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief FNV-1 64-bit hash function.
 *
 * \param data The data to hash.
 * \param len The length of the data.
 * \return uint64_t Output hash.
 */
extern uint64_t fnv1_64_hash_n(const void *data, size_t len);

/**
 * \brief FNV-1a 64-bit hash function.
 *
 * \param data The data to hash.
 * \param len The length of the data.
 * \return uint64_t Output hash.
 */
extern uint64_t fnv1a_64_hash_n(const void *data, size_t len);

/**
 * \brief FNV-1 64-bit hash function.
 *
 * \param data Null-terminated string to hash.
 * \return uint64_t Output hash.
 */
extern uint64_t fnv1_64_hash(const void *data);

/**
 * \brief FNV-1a 64-bit hash function.
 *
 * \param data Null-terminated string to hash.
 * \return uint64_t Output hash.
 */
extern uint64_t fnv1a_64_hash(const void *data);

#ifdef __cplusplus
}
#endif

#endif
