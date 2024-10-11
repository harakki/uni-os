#include <stdio.h>
#include <unistd.h>

#include "../lab01/list.h"

int main(void) {
  printf("PID: %d\n", getpid());

  list_t *linked_list = NULL;
  // Fill new list
  deserialize(&linked_list, "../lab01/db.csv");
  // Create a copy of linked list
  list_t *linked_list_copy = NULL;
  linked_list_copy = copy(linked_list, linked_list_copy);
  // Delete second element
  linked_list = delete (linked_list, 1);

  clean(linked_list);
  clean(linked_list_copy);

  while (1);

  return 0;
}
