/**
 * \file fnv1_32.h
 * \brief FNV-1(a) 32-bit hash function.
 */

#ifndef _FNV1_32_H
#define _FNV1_32_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief FNV-1 32-bit hash function.
 *
 * \param data The data to hash.
 * \param len The length of the data.
 * \return uint32_t Output hash.
 */
extern uint32_t fnv1_32_hash_n(const void *data, size_t len);

/**
 * \brief FNV-1a 32-bit hash function.
 *
 * \param data The data to hash.
 * \param len The length of the data.
 * \return uint32_t Output hash.
 */
extern uint32_t fnv1a_32_hash_n(const void *data, size_t len);

/**
 * \brief FNV-1 32-bit hash function.
 *
 * \param data Null-terminated string to hash.
 * \return uint32_t Output hash.
 */
extern uint32_t fnv1_32_hash(const void *data);

/**
 * \brief FNV-1a 32-bit hash function.
 *
 * \param data Null-terminated string to hash.
 * \return uint32_t Output hash.
 */
extern uint32_t fnv1a_32_hash(const void *data);

#ifdef __cplusplus
}
#endif

#endif
