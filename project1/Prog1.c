/* Prog1
 * Joshua Holman
 * June 8, 2024
 * Summer 2024 CPSC 323
 * */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
  one, // starting state
  two,
  three,
  four, // accepting state
  dead
} states;

typedef enum {
  a, b
} transitions;

// these functions apply a transition state
states one_handler(transitions transition) {
  switch (transition) {
    case a: return two;
    case b: return three;
  }
}
states two_handler(transitions transition) {
  switch (transition) {
    case a: return two;
    case b: return three;
  }
}
states three_handler(transitions transition) {
  switch (transition) {
    case a: return four;
    case b: return three;
  }
}
states four_handler(transitions transition) {
  switch (transition) {
    case a: return four;
    case b: return three;
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "This program determines if a string from the alphabet {a, b} belongs to the language L = (a|b)*ba*a\n");
    fprintf(stderr, "Please provide an appropriate string delimited with a '$'\n");
    fprintf(stderr, "Usage example: %s aaba$\n", argv[0]);
    return EXIT_FAILURE;
  }

  char* input = argv[1];
  bool valid = true;

  // validate input
  if (input[strlen(input) - 1] != '$') {
    fprintf(stderr, "Input string must end with a '$'\n");
    return EXIT_FAILURE;

  }

  for (size_t i = 0; i < strlen(input) - 1; i++) {
    char ch = input[i];
    if (ch != 'a' && ch != 'b') {
      valid = false;
      break;
    }
  }
  if (!valid) {
    fprintf(stderr, "Input string must consist of the letters 'a' and 'b'.\n");
    return EXIT_FAILURE;
  }

  // string should be valid at this point
  states state = one;
  for (size_t i = 0; i < strlen(input) - 1; i++) {
    char ch = input[i];
    transitions trans = (ch == 'a' ? a : b); // convert char to enum

    // apply relevant transition state
    switch (state) {
      case one:
        state = one_handler(trans);
        break;
      case two:
        state = two_handler(trans);
        break;
      case three:
        state = three_handler(trans);
        break;
      case four:
        state = four_handler(trans);
        break;
      case dead:
        break;

    }
  }

  /*printf("End state: %d\n", state);*/
  bool accepted = (state == four);

  if (accepted)
    printf("String '%s' accepted.\n", input);
  else
    printf("String '%s' rejected.\n", input);

  return 0;
}
