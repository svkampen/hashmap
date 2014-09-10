#include "list.h"

/* Pushes a link onto the top of the list. May traverse the list until at the head,
 * if a pointer to the head of the list is not passed in.
 *
 * @param list A pointer to a link in the list, preferably the head.
 * @param link The link to be added to the top of the list.
 * @return A pointer to the new head of the list.
 */
struct double_link *push_link_top(struct double_link *list, struct double_link *link) {
	while (list->prev != NULL) {
		list = list->prev;
	}
	link->next = list;
	list->prev = link;
	return link;
}

/* Sticks a link to the end of a list. May traverse list until tail.
 * 
 * @param list A pointer to a link in the list, preferably the tail.
 * @param link A pointer to the link that should be added to the list.
 * @return A pointer to the new tail of the list.
 */
struct double_link *push_link(struct double_link *list, struct double_link *link) {
	while (list->next != NULL) {
		list = list->next;
	}
	link->prev = list;
	list->next = link;
	return link;
}

void push_val(struct double_link *list, void *value) {
	while (list->next != NULL) {
		list = list->next;
	}
	if (list->ptr == NULL) {
		list->ptr = value;
	} else {
		LINK_CREATE(link);
		link->ptr = value;
		link->prev = list;
		list->next = link;
	}
}

void remove_link(struct double_link *link) {
	if (link->next == NULL && link->prev == NULL) {
		// this link has no siblings, so we can't link them.
		free(link);
		return;
	}
	
	if (link->next == NULL) {
		link->prev->next = NULL;
	} else if (link->prev == NULL) {
		link->next->prev = NULL;
	} else {
		link->prev->next = link->next;
		link->next->prev = link->prev;
	}

	free(link);
	return;
}

/* Get the head of a list.
 *
 * @param list A pointer to an element of a list.
 * @return A painter to the head of the list.
 */
struct double_link *get_list_head(struct double_link *list) {
	while (list->prev != NULL) {
		list = list->prev;
	}
	return list;
}

void free_list(struct double_link *list) {
	struct double_link *head = get_list_head(list);

	if (head->next == NULL) {
		free(head);
		return;
	}

	for (struct double_link *l = head->next; l != NULL; l = l->next) {
		if (l->next == NULL) {
			free(l);
		} else {
			free(l->prev);
		}
	}
}
