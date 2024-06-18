#include "hashmap.h"

struct hashmap *hashmap_create(size_t size,
                               size_t (*hash_key)(const void *key),
                               bool (*compare_key)(const void *key1, const void *key2),
                               void *(*alloc_data)(size_t size),
                               void (*free_data)(void *ptr),
                               void (*free_key)(void *key),
                               void (*free_value)(void *value)) {
	if (!hash_key) return NULL;
	if (!compare_key) return NULL;
	if (!alloc_data) return NULL;
	if (!free_data) return NULL;
	if (size <= 0) return NULL;

	// allocate memory for the map and initialize all buckets to NULL
	struct hashmap *map = alloc_data(sizeof(struct hashmap));
	if (!map) return NULL;

	map->size = 0;
	size_t total_size = size * sizeof(struct hashmap_entry *);
	map->buckets = alloc_data(total_size);
	if (!map->buckets) {
		free_data(map);
		return NULL;
	}
	for (size_t i = 0; i < size; i++) {
		map->buckets[i] = NULL;
	}

	map->size = size;
	map->hash_key = hash_key;
	map->compare_key = compare_key;
	map->alloc_data = alloc_data;
	map->free_data = free_data;
	map->free_key = free_key;
	map->free_value = free_value;
	return map;
}

static void internal_free_entry_kv(struct hashmap_entry *entry, struct hashmap *map) {
	if (!entry) return;
	// free key and value
	if (entry->key && map->free_key) map->free_key(entry->key);
	if (entry->value && map->free_value) map->free_value(entry->value);
	entry->key = NULL;
	entry->value = NULL;
}

static void internal_free_entry(struct hashmap_entry *entry, struct hashmap *map) {
	if (!entry) return;
	internal_free_entry_kv(entry, map);
	// free the entry
	map->free_data(entry);
}

void hashmap_clear(struct hashmap *map) {
	// free all entries
	for (size_t i = 0; i < map->size; i++) {
		struct hashmap_entry *entry = map->buckets[i];
		while (entry) {
			struct hashmap_entry *next = entry->next;
			internal_free_entry(entry, map);
			entry = next;
		}
		map->buckets[i] = NULL;
	}
}

void hashmap_free(struct hashmap *map) {
	hashmap_clear(map);
	// free buckets
	map->free_data(map->buckets);
	map->buckets = NULL;
	// free the map
	map->free_data(map);
}

static size_t internal_get_bucket_index(struct hashmap *map, void *key) {
	return map->hash_key(key) % map->size;
}

struct hashmap_entry *hashmap_set(struct hashmap *map, void *key, void *value) {
	// get the bucket for the key, then iterate over the linked list to find the key
	size_t bucket_index = internal_get_bucket_index(map, key);
	struct hashmap_entry *entry = map->buckets[bucket_index];
	while (entry) {
		if (map->compare_key(entry->key, key)) {
			// free old key and value
			internal_free_entry_kv(entry, map);
			// set the new key and value
			entry->key = key;
			entry->value = value;
			return entry;
		}
		if (!entry->next) break; // stop before we set entry to NULL
		entry = entry->next;
	}

	// if the key was not found, create a new entry
	struct hashmap_entry *new_entry = map->alloc_data(sizeof(struct hashmap_entry));
	if (!new_entry) return NULL;
	if (entry) {
		// update the previous entry's next pointer
		entry->next = new_entry;
	} else {
		// update the head entry's next pointer
		map->buckets[bucket_index] = new_entry;
	}
	// set the new key and value
	new_entry->key = key;
	new_entry->value = value;
	new_entry->next = NULL;
	return new_entry;
}

bool hashmap_remove(struct hashmap *map, void *key) {
	// get the bucket for the key, then iterate over the linked list to find the key
	size_t bucket_index = internal_get_bucket_index(map, key);
	struct hashmap_entry *head = map->buckets[bucket_index];
	struct hashmap_entry *entry = head, *prev = head;
	for (; entry; prev = entry, entry = entry->next) {
		// if the key was found, remove the entry
		if (map->compare_key(entry->key, key)) {
			if (head == entry) {
				// update the head entry's next pointer
				map->buckets[bucket_index] = entry->next;
			} else {
				// update the previous entry's next pointer
				prev->next = entry->next;
			}
			internal_free_entry(entry, map);
			return true;
		}
	}
	return false;
}

struct hashmap_entry *hashmap_get(struct hashmap *map, void *key) {
	// get the bucket for the key, then iterate over the linked list to find the key
	struct hashmap_entry *entry = map->buckets[internal_get_bucket_index(map, key)];
	for (; entry; entry = entry->next) {
		if (map->compare_key(entry->key, key)) return entry;
	}
	return NULL;
}

void *hashmap_get_value(struct hashmap *map, void *key) {
	struct hashmap_entry *entry = hashmap_get(map, key);
	if (!entry) return NULL;
	return entry->value;
}

void hashmap_loop(struct hashmap *map, void (*callback)(void *key, void *value, bool *proceed, bool *remove)) {
	// iterate over all buckets and entries, calling the callback for each entry
	for (size_t i = 0; i < map->size; i++) {
		struct hashmap_entry *head = map->buckets[i];
		struct hashmap_entry *entry = head, *prev = head;
		while (entry) {
			bool proceed = true, remove = false;
			// hashmap_remove is unsafe to call here
			callback(entry->key, entry->value, &proceed, &remove);
			if (remove) {
				if (head == entry) {
					// update the head entry's next pointer
					map->buckets[i] = entry->next;
				} else {
					// update the previous entry's next pointer
					prev->next = entry->next;
				}
				entry = entry->next;
				internal_free_entry(entry, map);
			} else {
				prev = entry;
				entry = entry->next;
			}
			if (!proceed) return;
		}
	}
}
