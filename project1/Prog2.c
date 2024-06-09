/* Prog2
 * Joshua Holman
 * June 8, 2024
 * Summer 2024 CPSC 323
 * */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_COLS 1000

typedef enum {
  start,
  in_token,
  in_whitespace,
  in_delimiter,
  in_comment
} State;

bool is_delimiter(char ch) {
  const char delimiters[] = "+-*/=(){}[]%?:|&,;";
  return strchr(delimiters, ch) != NULL;
}

int main() {
  char input_filename[] = "input.txt";
  char output_filename[] = "output.txt";
  FILE *input_file = fopen(input_filename, "r");
  FILE *output_file = fopen(output_filename, "w");

  if (input_file == NULL) {
    perror("Failed to open input file. Exiting...\n");
    return EXIT_FAILURE;
  }

  if (output_file == NULL) {
      perror("Failed to open output file. Exiting...\n");
      return EXIT_FAILURE;
  }

  char line[MAX_COLS];

  while (fgets(line, sizeof(line), input_file)) {
    int col = 0;
    int line_len = strlen(line);
    char lexeme[MAX_COLS] = {0};
    State state = start;

    while (col < line_len) {
      char ch = line[col];

      switch (state) {
        case start:
          if (ch == '/') {
            state = in_comment;
          }
          else if (is_delimiter(ch)) {
            lexeme[strlen(lexeme)] = ch;
            state = in_delimiter;
          }
          else if (!isspace(ch)) {
            lexeme[strlen(lexeme)] = ch;
            state = in_token;
          }
          break;

        case in_token:
          if (ch == '/') {
            state = in_comment;
          }
          else if (is_delimiter(ch)) {
            lexeme[strlen(lexeme)] = ' ';
            lexeme[strlen(lexeme)] = ch;
          }
          else if (isspace(ch)) {
            lexeme[strlen(lexeme)] = ' ';
            state = in_whitespace;
          }
          else {
            lexeme[strlen(lexeme)] = ch;
          }
          break;

        case in_whitespace:
          if (ch == '/') {
            state = in_comment;
          }
          else if (is_delimiter(ch)) {
            lexeme[strlen(lexeme)] = ch;
            lexeme[strlen(lexeme)] = ' ';
            state = in_delimiter;
          }
          else if (!isspace(ch)) {
            lexeme[strlen(lexeme)] = ch;
            state = in_token;
          }
          break;

        case in_delimiter:
          if (!is_delimiter(ch)) {
            if (!isspace(ch)) {
              lexeme[strlen(lexeme)] = ch;
              state = in_token;
            }
            else {
              state = in_whitespace;
            }
          }
          else { // when current char is a delim
            lexeme[strlen(lexeme)] = ' ';
            lexeme[strlen(lexeme)] = ch;
          }
          break;

        case in_comment:
          if (ch == '\n') {
            state = start;
          }
          break;
      }

      col++;
    }

    if (strlen(lexeme) > 0 && isspace(lexeme[strlen(lexeme) - 1])) {
      lexeme[strlen(lexeme) - 1] = '\0';
    }

    if (strlen(lexeme) > 0) {
      fprintf(output_file, "%s\n", lexeme);
    }
  }

  fclose(input_file);
  return 0;
}
