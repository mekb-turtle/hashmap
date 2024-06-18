#include "hashmap.h"
#include "fnv1_32.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static struct hashmap *map;

struct person {
	int age;
	float height;
};

static size_t hash(const void *key) {
	return fnv1a_32_hash(key);
}

static bool compare(const void *a, const void *b) {
	return strcmp(a, b) == 0;
}

bool add_user(char *name, int age, float height) {
	char *key = strdup(name);
	if (!key) return NULL;
	struct person *value = malloc(sizeof(struct person));
	if (!value) return NULL;
	value->age = age;
	value->height = height;
	return hashmap_set(map, key, value);
}

void print_user(void *key, void *value, bool *proceed, bool *remove) {
	char *name = (char *) key;
	struct person *person = (struct person *) value;
	int age = person->age;
	float height = person->height;
	printf("%s is %i and %g tall\n", name, age, height);
}

bool show_user(char *name) {
	struct hashmap_entry *entry = hashmap_get(map, name);
	if (!entry) {
		printf("%s does not exist\n", name);
		return false;
	}
	print_user(entry->key, entry->value, NULL, NULL);
	return true;
}

int main() {
	map = hashmap_create(1024, hash, compare, malloc, free, free, free);
	if (!map) return 1;

	if (!add_user("Alice", 25, 1.75f)) return 1;
	if (!add_user("Bob", 30, 1.80f)) return 1;

	printf("All Users:\n");
	hashmap_loop(map, print_user);

	printf("\n");

	show_user("Alice");
	show_user("Bob");
	show_user("Charlie");

	printf("Removing Alice\n");
	hashmap_remove(map, "Alice");

	show_user("Alice");
	show_user("Bob");
	show_user("Charlie");

	hashmap_free(map);
	return 0;
}
