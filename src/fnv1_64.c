#include "fnv1_64.h"

static const uint64_t offset = 0xcbf29ce484222325, prime = 0x00000100000001b3;

uint64_t fnv1_64_hash_n(const void *data, size_t len) {
	uint64_t hash = offset;
	for (size_t i = 0; i < len; i++) {
		hash *= prime;
		hash ^= ((uint8_t *) data)[i];
	}
	return hash;
}

uint64_t fnv1a_64_hash_n(const void *data, size_t len) {
	uint64_t hash = offset;
	for (size_t i = 0; i < len; i++) {
		hash ^= ((uint8_t *) data)[i];
		hash *= prime;
	}
	return hash;
}

uint64_t fnv1_64_hash(const void *data) {
	uint64_t hash = offset;
	for (const uint8_t *i = data; *i; i++) {
		hash *= prime;
		hash ^= *i;
	}
	return hash;
}

uint64_t fnv1a_64_hash(const void *data) {
	uint64_t hash = offset;
	for (const uint8_t *i = data; *i; i++) {
		hash ^= *i;
		hash *= prime;
	}
	return hash;
}
