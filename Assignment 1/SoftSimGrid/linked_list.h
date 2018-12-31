#pragma once

typedef struct node {
	int i;
	struct node* next;
}node;

typedef struct linked_list {
	struct node* head;
} linked_list;

void add_node(node *head, node *new_node);
void add_node_list(linked_list* list, node* new_node);
void clear_list(node *head);
void print(node *head);