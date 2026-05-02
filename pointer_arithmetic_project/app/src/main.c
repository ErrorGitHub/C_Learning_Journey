
#include <stdio.h>
#include <stdlib.h>

enum { ARRAY_SIZE = 5 };

int main(void) {
  int *array = malloc(sizeof(*array) * ARRAY_SIZE);

  if (!array) {
    return -1;
  }

  printf("Please enter five numbers!\n");

  for (int x = 0; x < ARRAY_SIZE; x++) {
    printf("%i: ", (x + 1));
    scanf("%i", (array + x));
  }

  printf("\nThank you for those numbers, here they are again\n\n");

  for (int x = 0; x < ARRAY_SIZE; x++) {
    printf("%i: You entered %i\n", (x + 1), array[x]);
  }

  free(array);
  return 0;
}
