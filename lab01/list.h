#ifndef UNI_OS_LIST_H
#define UNI_OS_LIST_H

#include <stddef.h>

// Structure for storing any data
typedef struct {
  int id;
  char full_name[64];
  int group;
} data_t;

// Linked list structure
typedef struct node {
  data_t data;
  struct node *next;
} list_t;

// Insert data at specific place in the list
list_t *insert(list_t *head, int element, data_t data);

// Insert data at the end of the list
list_t *push_back(list_t *head, data_t data);

// Create a copy of the list
list_t *copy(list_t *from_head, list_t *to_head);

// Delete specific element from the list
list_t *delete(list_t *head, int element);

// Delete last element in the list
list_t *pop_back(list_t *head);

// Get specific element from the list
data_t *get(list_t *head, int element);

// Get last element from the list
list_t *back(list_t *head);

// Get ammount of elements in the list
size_t size(list_t *head);

// Delete list
void clean(list_t *head);

// Import data from CSV file
void serialize(list_t *head, const char *filename);

// Export data from CSV file
void deserialize(list_t *head, const char *filename);

#endif  // UNI_OS_LIST_H
