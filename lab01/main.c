#include <stdio.h>

#include "list.h"

void print_list(list_t *head) {
  list_t *ptr = head;

  while (ptr != NULL) {
    printf("[%s | %06X] -> ", ptr->data.name, ptr->data.hex);
    ptr = ptr->next;
  }
  printf("NULL\n");
}

int main(void) {
  list_t *linked_list = NULL;
  // Fill new list
  deserialize(&linked_list, "../lab01/db.csv");
  // Create a copy of linked list
  list_t *linked_list_copy = NULL;
  linked_list_copy = copy(linked_list, linked_list_copy);
  // Delete second element
  linked_list = delete (linked_list, 1);
  // Output all lists
  print_list(linked_list);
  print_list(linked_list_copy);

  clean(linked_list);
  clean(linked_list_copy);

  return 0;
}
