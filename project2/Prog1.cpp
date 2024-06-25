/* Joshua Holman
 * CPSC 323 Project 2
 * 17 June 2024
 *
 *
 * I originally did this in C but switched to Cpp to try and resolve some curious issues I was having. 
 * It did resolve a few small things but the program still does not work correctly.
 * As you might already realize it does not accept any strings and 'finds' a syntax error on the first character.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stack>
#include <string>

#define NUM_NONTERMS 5
#define NUM_TERMS 10
#define MAX_STACK_SIZE 100
#define MAX_INPUT_SIZE 100

const char *terminals = "&%!<>tf()$";
const char *nonterminals = "EGFHT";

const char *parse_table[NUM_NONTERMS][NUM_TERMS] = {
    {"", "", "", "", "", "F!G", "F!G", "", "", ""},
    {"&TG", "%TG", "", "", "", "", "", "", "ε", "ε"},
    {"", "", "ε", "", "", "tH", "fH", "", "", ""},
    {"", "", "", "<TH", ">TH", "", "", "", "", ""},
    {"", "", "", "", "", "t", "f", "(E)", "", ""},
};

void error(const char *message, char *input, int position) {
  fprintf(stderr, "Syntax Error: %s\n", message);
  fprintf(stderr, "%s\n", input);
  for (int i = 0; i < position; i++) {
    putchar(' ');
  }
  printf("^\n");
  exit(1);
}

int main(void) {
  /* This program traces an input string given by the user given by the
   * following grammar:
   * E -> E&T
   * E -> E%T
   * E -> F!
   * F -> F<T
   * F -> t | f 
   * T -> (E) | t | f
   *
   * and with left recursion removed:
   * E -> F!G
   * G -> &TG | %TG | ε
   * F -> tH | fH
   * H -> <TH | >TH | ε
   * T -> (E) | t | f
   * */

  char input[MAX_INPUT_SIZE];

  printf("Enter an input string delimited with a $: ");
  if (fgets(input, sizeof(input), stdin) == NULL) {
    fprintf(stderr, "fgets error\n");
  }

  // remove trailing newline
  size_t len = strlen(input);
  if (len > 0 && input[len - 1] == '\n') {
    input[len - 1] = '\0';
  }

  // validate input
  if (input[strlen(input) - 1] != '$') {
    error("Input string must end with a '$'", input, strlen(input));
  }

  std::string cpp_input = input;
  printf("You entered: %s\n", input);

  std::stack<char> stack;
  char *token_ptr = input;
  stack.push('$');
  std::cout << stack.top() << "\n";
  stack.push('E');
  std::cout << stack.top() << "\n";
  int row, col;

  while (!stack.empty()) {

    // if I had more time I would convert these swtitches into a cpp map or something

    // determine row and col positions
    switch (*token_ptr) {
    case '&':
      col = 0;
      break;
    case '%':
      col = 1;
      break;
    case '!':
      col = 2;
      break;
    case '<':
      col = 3;
      break;
    case '>':
      col = 4;
      break;
    case 't':
      col = 5;
      break;
    case 'f':
      col = 6;
      break;
    case '(':
      col = 7;
      break;
    case ')':
      col = 8;
      break;
    case '$':
      col = 9;
      break;
      error("Invalid input symbol", input, token_ptr - input);
    }

    switch (stack.top()) {
    case 'E':
      row = 0;
      break;
    case 'G':
      row = 1;
      break;
    case 'F':
      row = 2;
      break;
    case 'H':
      row = 3;
      break;
    case 'T':
      row = 4;
      break;
    default:
      if (strchr(terminals, stack.top()) == NULL)
        std::cerr << "Offending symbol: " << stack.top();
        error("Invalid stack symbol", input, token_ptr - input);
    }

    /*if (strchr(terminals, *token_ptr) != NULL) { // if terminal pop stack and go to next input token*/

    if (cpp_input.find(*token_ptr) != std::string::npos) { // if terminal
      if (stack.top() == *token_ptr) {
        if (*token_ptr != '\0') {
          token_ptr++;
        }
        else error("Unexpected end of input (1)", input, token_ptr - input);
        stack.pop();
        token_ptr++;
      } else {
        error("Unexpected terminal symbol", input, token_ptr - input);
      }
    } else { // nonterminal
      const char *table_entry = parse_table[row][col];
      if (strcmp(table_entry, "") != 0) { // if table entry is nonblank
        // pop stack and push production onto stack in reverse order
        stack.pop();
        size_t len = strlen(table_entry);
        for (int i = len; i > 0; i--) {
          stack.push(table_entry[i - 1]);
        }
      }
      else {
        error("Unexpected nonterminal symbol", input, token_ptr - input);
      }
    }
  }

  if (stack.empty() && *token_ptr == '$') {
    printf("string accepted");
  } else {
    error("Unexpected end of input (2)", input, token_ptr - input);
  }

  return 0;
}
