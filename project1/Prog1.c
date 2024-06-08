#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum {
  one_four, // starting state
  two_four,
  three_four_five,
  four_six, // accepting state
  four_five,
  six, // accepting state
  five,
  dead
} states;

typedef enum {
  a, b
} transitions;

states One_Four_Handler(transitions transition) {
  switch (transition) {
    case a: return two_four;
    case b: return three_four_five;
  }
}

states Two_Four_Handler(transitions transition) {
  switch (transition) {
    case a: return four_six;
    case b: return four_five;
  }
}
states Three_Four_Five_Handler(transitions transition) {
  switch (transition) {
    case a: return four_six;
    case b: return four_five;
  }
}
states Four_Six_Handler(transitions transition) {
  switch (transition) {
    case a: return six;
    case b: return five;
  }
}
states Four_Five_Handler(transitions transition) {
  switch (transition) {
    case a: return six;
    case b: return five;
  }
}
states Six_Handler(transitions transition) {
  switch (transition) {
    case a: return six;
    case b: return dead;
  }
}
states Five_Handler(transitions transition) {
  switch (transition) {
    case a: return six;
    case b: return dead;
  }
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <input_string>\n", argv[0]);
    return EXIT_FAILURE;
  }

  char* input = argv[1];
  bool valid = true;

  // validate input
  for (size_t i = 0; i < strlen(input); i++) {
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





  /*states next_state = one_four;*/
  /**/
  /*const size_t MAX = 5;*/
  /*char buffer[MAX];*/
  /**/
  /**/
  /*printf("Enter a string from the alphabet {a, b}: ");*/
  /**/
  /*while (true) {*/
  /*  bool valid = true;*/
  /**/
  /*  if (!fgets(buffer, MAX, stdin)) {*/
  /*    printf("Read error. Exiting...\n");*/
  /*    return 1;*/
  /*  }*/
  /**/
  /*  buffer[strcspn(buffer, "\n")] = '\0';*/
  /**/
  /*  for (char *ptr = buffer; *ptr != '\0'; ptr++) {*/
  /*    if (*ptr != 'a' && *ptr != 'b') {*/
  /*      valid = false;*/
  /*      rewind(stdin);*/
  /*      break;*/
  /*    }*/
  /*  }*/
  /**/
  /*  if (!valid) printf("Invalid string. Try again: ");*/
  /*  else break;*/
  /*}*/
  /**/
  /*printf("Entered string: %s\n", buffer);*/

  return 0;
}
