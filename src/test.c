#include "hash.h"

void set_value(struct hashmap *map) {
    int *i = malloc(sizeof(int));
    *i = 20;
    hashmap_set("hi", (void*)i, map);
}


int main() {
    struct hashmap *map = hashmap_create(32);
    set_value(map);
    int *i = hashmap_get("hi", map);
    printf("%d\n", *i);
    free(i);
    void *ptr = hashmap_get("abc", map);
    printf("%d\n", (ptr == NULL));
    hashmap_destroy(map);
    return 0;
}
