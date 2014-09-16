#include <lib/list.h>
#include <stdlib.h>
void list_destroy(list_t * list) {
	/* Free all of the contents of a list */
	list_node_t * n = list->head;
	while (n) {
		free(n->value);
		n = n->next;
	}
}

void list_free(list_t * list) {
	/* Free the actual structure of a list */
	list_node_t * n = list->head;
	while (n) {
		list_node_t * s = n->next;
		free(n);
		n = s;
	}
}

void list_append(list_t * list, list_node_t * node) {
	node->next = NULL;
	/* Insert a node onto the end of a list */
	if (!list->tail) {
		list->head = node;
		node->prev = NULL;
	} else {
		list->tail->next = node;
		node->prev = list->tail;
	}
	list->tail = node;
	list->length++;
}

list_node_t * list_insert(list_t * list, void * item) {
	/* Insert an item into a list */
	list_node_t * node = malloc(sizeof(list_node_t));
	node->value = item;
	node->next  = NULL;
	node->prev  = NULL;
	list_append(list, node);

	return node;
}

void list_append_after(list_t * list, list_node_t * before, list_node_t * node) {
	if (!list->tail) {
		list_append(list, node);
		return;
	}
	if (before == NULL) {
		node->next = list->head;
		list->head->prev = node;
		list->head = node;
		list->length++;
		return;
	}
	if (before == list->tail) {
		list->tail = node;
	} else {
		before->next->prev = node;
		node->next = before->next;
	}
	node->prev = before;
	before->next = node;
	list->length++;
}

list_node_t * list_insert_after(list_t * list, list_node_t * before, void * item) {
	list_node_t * node = malloc(sizeof(list_node_t));
	node->value = item;
	node->next  = NULL;
	node->prev  = NULL;
	list_append_after(list, before, node);
	return node;
}

list_t * list_create(void) {
	/* Create a fresh list */
	list_t * out = malloc(sizeof(list_t));
	out->head = NULL;
	out->tail = NULL;
	out->length = 0;
	return out;
}

list_node_t * list_find(list_t * list, void * value) {
	foreach(item, list) {
		if (item->value == value) {
			return item;
		}
	}
	return NULL;
}

int list_index_of(list_t * list, void * value) {
	int i = 0;
	foreach(item, list) {
		if (item->value == value) {
			return i;
		}
		i++;
	}
	return -1; /* not find */
}

void list_remove(list_t * list, size_t index) {
	/* remove index from the list */
	if (index > list->length) return;
	size_t i = 0;
	list_node_t * n = list->head;
	while (i < index) {
		n = n->next;
		i++;
	}
	list_delete(list, n);
}

void list_delete(list_t * list, list_node_t * node) {
	/* remove node from the list */
	if (node == list->head) {
		list->head = node->next;
	}
	if (node == list->tail) {
		list->tail = node->prev;
	}
	if (node->prev) {
		node->prev->next = node->next;
	}
	if (node->next) {
		node->next->prev = node->prev;
	}
	node->prev = NULL;
	node->next = NULL;
	list->length--;
}

list_node_t * list_pop(list_t * list) {
	/* Remove and return the last value in the list
	 * If you don't need it, you still probably want to free it!
	 * Try free(list_pop(list)); !
	 * */
	if (!list->tail) return NULL;
	list_node_t * out = list->tail;
	list_delete(list, list->tail);
	return out;
}

list_node_t * list_dequeue(list_t * list) {
	if (!list->head) return NULL;
	list_node_t * out = list->head;
	list_delete(list, list->head);
	return out;
}

list_t * list_copy(list_t * original) {
	/* Create a new copy of original */
	list_t * out = list_create();
	list_node_t * node = original->head;
	while (node) {
		list_insert(out, node->value);
	}
	return out;
}

void list_merge(list_t * target, list_t * source) {
	/* Destructively merges source into target */
	if (target->tail) {
		target->tail->next = source->head;
	} else {
		target->head = source->head;
	}
	if (source->tail) {
		target->tail = source->tail;
	}
	target->length += source->length;
	free(source);
}
