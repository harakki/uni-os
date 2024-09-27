#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

list_t *insert(list_t *head, int element, data_t data) {
  // Create new element to insert
  list_t *new_node = (list_t *)malloc(sizeof(list_t));
  if (new_node == NULL) {
    return head;
  }
  new_node->data = data;
  new_node->next = NULL;
  // Insert element before the head
  if (element <= 0 || head == NULL) {
    new_node->next = head;
    return new_node;
  }
  size_t list_size = size(head);
  // Insert element if size of the list is smaller than the chosen place
  if (element >= list_size) {
    list_t *temp = head;
    while (temp->next != NULL) {
      temp = temp->next;
    }
    temp->next = new_node;
    return head;
  }
  // Insert element in the list between head and tail
  list_t *ptr = head;
  // Moving up to the place where the data needs to be inserted
  for (size_t i = 0; i < element - 1 && ptr->next != NULL; i++) {
    ptr = ptr->next;
  }
  // Link the new element to the elements in the list
  new_node->next = ptr->next;
  ptr->next = new_node;
  return head;
}

list_t *push_back(list_t *head, data_t data) {
  // In case if list does not exist
  if (head == NULL) {
    list_t *new_node = (list_t *)malloc(sizeof(list_t));
    if (new_node == NULL) {
      return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
  } else {
    head->next = push_back(head->next, data);
    return head;
  }
}

list_t *copy(list_t *from_head, list_t *to_head) {
  if (from_head == NULL) {
    return to_head;
  }
  list_t *ptr = from_head;
  while (ptr != NULL) {
    to_head = push_back(to_head, ptr->data);
    ptr = ptr->next;
  }
  return to_head;
}

list_t *delete(list_t *head, int element) {
  if (head == NULL || element < 0 || element >= size(head)) {
    return head;
  }
  // Delete head and pass next element as the new head
  if (element == 0) {
    list_t *next_head = head->next;
    free(head);
    return next_head;
  }

  list_t *ptr = head;
  list_t *delete_vault;

  // Moving up to place where need to delete data
  for (int i = 0; i < element - 1 && ptr->next != NULL; i++) {
    ptr = ptr->next;
  }

  // Pick element for removing
  delete_vault = ptr->next;
  if (delete_vault == NULL) {
    return head;
  }
  // Linking list without element for removing
  ptr->next = delete_vault->next;
  // Remove needed element
  free(delete_vault);
  return head;
}

list_t *pop_back(list_t *head) {
  if (head == NULL) {
    return NULL;
  }
  // If list contains only one element
  if (head->next == NULL) {
    free(head);
    return NULL;
  }
  list_t *ptr = head;
  // Move pointer to the 1 element before latest
  while (ptr->next->next != NULL) {
    ptr = ptr->next;
  }
  // Remove latest element
  free(ptr->next);
  ptr->next = NULL;
  return head;
}

data_t *get(list_t *head, int element) {
  if (head == NULL || element < 0 || element >= size(head)) {
    return NULL;
  }
  list_t *ptr = head;

  // Moving up to place where need to get data
  for (int i = 0; i < element; i++) {
    ptr = ptr->next;
  }
  // Create copy of the data to return
  data_t *data = (data_t *)malloc(sizeof(data_t));
  if (data == NULL) {
    return NULL;
  }
  *data = ptr->data;
  return data;
}

list_t *back(list_t *head) {
  if (head == NULL) {
    return NULL;
  }
  list_t *ptr = head;
  while (ptr->next != NULL) {
    ptr = ptr->next;
  }
  return ptr;
}

size_t size(list_t *head) {
  size_t size = 0;
  list_t *ptr = head;
  while (ptr != NULL) {
    ptr = ptr->next;
    size++;
  }
  return size;
}

void clean(list_t *head) {
  list_t *ptr = head;
  list_t *next;
  while (ptr != NULL) {
    next = ptr->next;
    free(ptr);
    ptr = next;
  }
}

int serialize(list_t *head, const char *filename) {
  // Open and check success of opening file descriptor
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("serialize:file-open:error");
    return -1;
  }
  // Read all data from .csv file
  list_t *ptr = head;
  while (ptr != NULL) {
    fprintf(file, "%s,%06X\n", ptr->data.name, ptr->data.hex);
    ptr = ptr->next;
  }
  // Close file descriptor
  fclose(file);
  return 0;
}

int deserialize(list_t **head, const char *filename) {
  // Open and check success of opening file descriptor
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("deserialize:file-open:error");
    return -1;
  }
  // Clean list
  clean(*head);
  *head = NULL;
  // Create array for storing readed data
  char line[256];
  data_t data;
  // Read all lines of file
  while (fgets(line, sizeof(line), file)) {
    char *comma = strchr(line, ',');
    // Check if string of data is valid
    if (comma == NULL) {
      fclose(file);
      perror("deserialize:file-read:format-error:comma");
      return -1;
    }
    // Replace comma with NULL
    *comma = '\0';
    // Copy new data into data structure
    strncpy(data.name, line, sizeof(data.name) - 1);
    data.name[sizeof(data.name) - 1] = '\0';
    data.hex = (int)strtol(comma + 1, NULL, 16);
    // Add data into the list
    *head = push_back(*head, data);
  }
  // Close file descriptor
  fclose(file);
  return 0;
}
