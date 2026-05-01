
#include <stdio.h>
int main(void) {
  int a = 4;
  int *ptr = &a;
  printf("Value retrieved via pointer: %i\n", *ptr);
  return 0;
}
