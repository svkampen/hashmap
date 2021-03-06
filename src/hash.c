#include "hashmap.h"
#include "hashfn.h"
#include <string.h>
#include <stdlib.h>

#define LOAD_FACTOR (2/3.0f)

static uint32_t seed = 0xE3C1AE9D;

int hashmap_getindex(char *key, struct hashmap *ht) {
	int len = strlen(key);
	uint64_t *hashes = malloc(2*sizeof(uint64_t));

	MurmurHash3_x64_128((void*)key, len, seed, (void*)hashes);
	// the hash is 128-bit, but we'll only use the first 64 bits.

	int index = (hashes[0] % ht->size);
	free(hashes);
	return index;
}

void *hashmap_get(char *key, struct hashmap *ht) {
	int len = strlen(key);
	uint64_t *hashes = malloc(2*sizeof(uint64_t));

	MurmurHash3_x64_128((void*)key, len, seed, (void*)hashes);
	// the hash is 128-bit, but we'll only use the first 64 bits.

	int index = (hashes[0] % ht->size);

	free(hashes);

	Link *list = (ht->arr)[index];
	void *ptr = NULL;
	for (Link *i = list; i != NULL; i = i->next) {
		if (i->ptr == NULL) {
			continue;
		}

		hashed_pair *p = (hashed_pair*)i->ptr;
		if (strcmp(p->key, key) == 0) {
			ptr = p->value;
		}
	}
	return ptr;
}

void hashmap_remove(char *key, struct hashmap *hm) {
	int len = strlen(key);
	uint64_t *hashes = malloc(2*sizeof(uint64_t));

	MurmurHash3_x64_128((void*)key, len, seed, (void*)hashes);

	int index = (hashes[0] % hm->size);

	free(hashes);

	Link *list = (hm->arr)[index];
	void *ptr = NULL;

	for (Link *i = list; i != NULL; i = i->next) {
		if (i->ptr == NULL) {
			continue;
		}

		hashed_pair *p = (hashed_pair*)i->ptr;

		if (strcmp(p->key, key) == 0) {
			free(p->key);
			free(p);
			i->ptr = NULL;
			hm->length--;
			break;
		}
	}
}

void hashmap_set(char *key, void *ptr, struct hashmap *ht) {
	if (((ht->length + 1) / ht->size) > LOAD_FACTOR) {
		hashmap_resize(ht);
	}

	int len = strlen(key);
	uint64_t *hashes = malloc(2*sizeof(uint64_t));

	MurmurHash3_x64_128((void*)key, len, seed, (void*)hashes);

	int index = (hashes[0] % ht->size);

	Link* list = ht->arr[index];

	// check whether the key is already in the list
	for (Link *i = list; i != NULL; i = i->next) {
		if (i->ptr == NULL) {
			continue;
		}

		hashed_pair *pair = (hashed_pair*)i->ptr;
		if (pair->key == key) {
			// we don't even have to allocate a pair now!
			pair->value = ptr;
			free(hashes);
			return;
		}
	}

	hashed_pair *pair = calloc(1, sizeof(hashed_pair));
	pair->hash = hashes[0];
	pair->key = strdup(key);
	pair->value = ptr;

	free(hashes);

	ht->length++;

	push_val(list, (void*)pair);
}

void hashmap_resize(struct hashmap *ht) {
	ht->size *= 2;
	Link **new_arr = calloc(ht->size, sizeof(Link*));

	for (int i = 0; i < ht->size; ++i) {
		new_arr[i] = calloc(1, sizeof(Link));
	}

	for (int i = 0; i < (ht->size / 2); ++i) {
		for (Link *j = ht->arr[i]; j != NULL; j = j->next) {
			if (j->ptr == NULL) {
				continue;
			}

			hashed_pair *p = (hashed_pair*)j->ptr;
			int index = p->hash % ht->size;

			push_val(new_arr[index], (void*)p);
		}
		free_list(ht->arr[i]);
	}

	free(ht->arr);
	ht->arr = new_arr;
}

struct hashmap *hashmap_create(int size) {
	struct hashmap *ht = calloc(1, sizeof(struct hashmap));
	ht->length = 0;
	ht->size = size;

	ht->arr = calloc(ht->size, sizeof(Link*));

	for (int i = 0; i < ht->size; ++i) {
		ht->arr[i] = calloc(1, sizeof(Link));
	}

	return ht;
}

void hashmap_foreach(struct hashmap *hm, void (*fn)(char*, void*)) {
	for (int i = 0; i < hm->size; ++i) {
		for (Link *j = hm->arr[i]; j != NULL; j = j->next) {
			if (j->ptr != NULL) {
				hashed_pair *pair = (hashed_pair*)j->ptr;
				fn(pair->key, pair->value);
			}
		}
	}
}

char **hashmap_keys(struct hashmap *hm) {
	char **keys = calloc(hm->length, sizeof(char*));
	int key_index = 0;
	for (int i = 0; i < hm->size; ++i) {
		for (Link *j = hm->arr[i]; j != NULL; j = j->next) {
			if (j->ptr != NULL) {
				hashed_pair *pair = (hashed_pair*)j->ptr;
				keys[key_index++] = pair->key;
			}
		}
	}
	return keys;
}

void hashmap_destroy(struct hashmap *hm) {
	// first, free all linked lists.
	for (int j = 0; j < hm->size; ++j) {
		for (Link *i = hm->arr[j]; i != NULL; i = i->next) {
			if (i->ptr != NULL) {
				hashed_pair *pair = (hashed_pair*)i->ptr;
				free(pair->key);
				free(pair);
			}
		}
		free_list(hm->arr[j]);
	}

	free(hm->arr);
	free(hm);
}
