/**
 * \file hashmap.h
 * \brief C hashmap library.
 */

#ifndef _HASHMAP_H
#define _HASHMAP_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _HASHMAP_WARN_UNUSED
#ifdef __GNUC__
#define _HASHMAP_WARN_UNUSED __attribute__((warn_unused_result))
#else
#define _HASHMAP_WARN_UNUSED
#endif
#endif

/**
 * \brief An entry in a ::hashmap.
 */
struct hashmap_entry {
	/**
	 * \brief The key of the entry.
	 *
	 * This can be casted to any pointer type. It will be freed with free_key.
	 */
	void *key;

	/**
	 * \brief The value of the entry.
	 *
	 * This can be casted to any pointer type. It will be freed with free_value.
	 */
	void *value;

	/**
	 * \brief The next entry in the bucket.
	 *
	 * \warning This should not be modified or accessed directly.
	 */
	struct hashmap_entry *next;
};

/**
 * \brief A hashmap.
 *
 * This is a hashmap that uses a linked list to handle collisions.
 *
 * \sa \link ::hashmap_create hashmap_create\endlink
 */
struct hashmap {
	/**
	 * \brief Buckets containing entries.
	 *
	 * \warning This should not be modified or accessed directly.
	 */
	struct hashmap_entry **buckets;

	/**
	 * \brief Number of buckets.
	 *
	 * \warning This should not be modified directly.
	 */
	size_t size;

	/**
	 * \brief Function pointer that hashes a key.
	 *
	 * This must be a deterministic hash function.
	 *
	 * \warning Must not be NULL.
	 */
	size_t (*hash_key)(const void *key);

	/**
	 * \brief Function pointer that compares if two keys are equal.
	 *
	 * \warning Must not be NULL.
	 */
	bool (*compare_key)(const void *key1, const void *key2);

	/**
	 * \brief Function pointer that allocates memory.
	 *
	 * \warning Must not be NULL.
	 */
	void *(*alloc_data)(size_t size);

	/**
	 * \brief Function pointer that frees memory.
	 *
	 * \warning Must not be NULL.
	 */
	void (*free_data)(void *ptr);

	/**
	 * \brief Function pointer that frees a key.
	 */
	void (*free_key)(void *key);

	/**
	 * \brief Function pointer that frees a value.
	 */
	void (*free_value)(void *value);
};

/**
 * \brief Create a new ::hashmap.
 *
 * \param size How many buckets to allocate. The larger the size, the less likely a collision will occur, but the more memory will be used.
 * \param hash_key Function pointer that hashes a key. This must be a deterministic hash function.
 * \param compare_key Function pointer that compares if two keys are equal.
 * \param alloc_data Function pointer that allocates memory. If you are not using a custom allocator, you can set this to malloc.
 * \param free_data Function pointer that frees memory. If you are not using a custom allocator, you can set this to free.
 * \param free_key Function pointer that frees a key.
 * \param free_value Function pointer that frees a value.
 * \return struct ::hashmap* A pointer to the ::hashmap. This should be freed with ::hashmap_free when you are done with it.
 * \return NULL If the ::hashmap could not be created.
 */
extern struct hashmap *_HASHMAP_WARN_UNUSED
hashmap_create(size_t size,
               size_t (*hash_key)(const void *key),
               bool (*compare_key)(const void *key1, const void *key2),
               void *(*alloc_data)(size_t size),
               void (*free_data)(void *ptr),
               void (*free_key)(void *key),
               void (*free_value)(void *value));

/**
 * \brief Frees a ::hashmap and all of its entries.
 *
 * \param map The ::hashmap to free
 */
extern void hashmap_free(struct hashmap *map);

/**
 * \brief Frees and removes all entries in a ::hashmap, but does not free the ::hashmap itself
 *
 * \param map The ::hashmap to clear
 */
extern void hashmap_clear(struct hashmap *map);

/**
 * \brief Add or overwrite an entry in the ::hashmap.
 *
 * If the key already exists, it will be overwritten and the old entry will be freed.
 *
 * \warning The key and value must be always accessible in memory.
 *
 * \param map The ::hashmap to add to or modify
 * \param key The key to set. This will be freed with free_key.
 * \param value The value to set. This will be freed with free_value.
 * \return struct ::hashmap_entry* A pointer to the entry that was set
 * \return NULL If the entry could not be set
 */
extern struct hashmap_entry *hashmap_set(struct hashmap *map, void *key, void *value);

/**
 * \brief Remove an entry from the ::hashmap by key.
 *
 * \param map The ::hashmap to remove the entry from
 * \param key The key to remove
 * \return true If an entry was found and removed
 * \return false If no entry was found
 */
extern bool hashmap_remove(struct hashmap *map, void *key);

/**
 * \brief Get an \link ::hashmap_entry entry\endlink from the ::hashmap by key.
 *
 * \param map The ::hashmap to get the entry from
 * \param key The key to get
 * \return struct hashmap_entry* A pointer to the entry that was found, or NULL if no entry was found
 */
extern struct hashmap_entry *_HASHMAP_WARN_UNUSED hashmap_get(struct hashmap *map, void *key);

/**
 * \brief Get the \link ::hashmap_entry::value value\endlink of an \link ::hashmap_entry entry\endlink from the ::hashmap by key.
 *
 * Equivalent to \link ::hashmap_get hashmap_get\endlink(map, key)->\link ::hashmap_entry::value value\endlink.
 *
 * \param map The hashmap to get the entry from
 * \param key The key to get
 * \return struct hashmap_entry* The value of the entry that was found, or NULL if no entry was found
 */
extern void *_HASHMAP_WARN_UNUSED hashmap_get_value(struct hashmap *map, void *key);

/**
 * \brief Loop over all entries in the ::hashmap.
 *
 * \param map The ::hashmap to loop over.
 * \param callback The function to call for each entry.
 * \warning ::hashmap_remove and other \link hashmap.h hashmap_*\endlink functions are unsafe to call in the callback
 */
extern void hashmap_loop(struct hashmap *map, void (*callback)(void *key, void *value, bool *proceed, bool *remove));

#undef _HASHMAP_WARN_UNUSED

#ifdef __cplusplus
}
#endif
#endif
