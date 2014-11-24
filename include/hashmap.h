#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include "list.h"
#include <inttypes.h>

typedef struct double_link Link;

struct hashmap {
	Link **arr;
	int length;
	int size;
};

typedef struct {
	uint64_t hash;
	char *key;
	void *value;
} hashed_pair;

int hashmap_getindex(char *key, struct hashmap *ht);
void *hashmap_get(char *key, struct hashmap *ht);
void hashmap_set(char *key, void *ptr, struct hashmap *ht);
void hashmap_remove(char *key, struct hashmap *hm);
struct hashmap *hashmap_create(int size);
void hashmap_destroy(struct hashmap *hm);
void hashmap_resize(struct hashmap *ht);
void hashmap_foreach(struct hashmap *hm, void (*fn)(char*, void*));
char **hashmap_keys(struct hashmap *hm);
#endif
