#include "linked_list.h"
#include <stdlib.h>

void add_node(node *head, node *new_node) {
	if (head->i == -1) {
		head->i = new_node->i;
	}
	else {
		node* temp = head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = new_node;
	}
}

void add_node_list(linked_list* list, node* new_node) {
	if (list->head == NULL) {
		list->head = new_node;
	}
	else {
		node* temp = list->head;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = new_node;
	}
}

void clear_list(node *head) {
	/*node* current_node = head->next;
	node* next;

	while (current_node != NULL) {
		next = current_node->next;
		free(current_node);
		current_node = next;
	}*/
	head->i = -1;
	head->next = NULL;
}

void print(node *head) {
	node *temp = head;

	while (temp != NULL) {
		printf("%d->", temp->i);
		temp = temp->next;
	}
	printf("\n");
}