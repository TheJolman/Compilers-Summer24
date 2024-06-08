/* Prog2
 * Joshua Holman
 * June 8, 2024
 * Summer 2024 CPSC 323
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE* ptr;
  char ch;

  ptr = fopen("input.txt", "r");

  if (ptr == NULL) {
    fprintf(stderr, "file can't be opened. Exiting...\n");
    return EXIT_FAILURE;
  }

  do {
    ch = fgetc(ptr);
    printf("%c", ch);

  } while (ch != EOF);
  putchar('\n');

  fclose(ptr);
  return 0;
}
