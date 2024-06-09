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
  const char delimiters[] = "+-*/=(){}[]%?:|&";
  return strchr(delimiters, ch) != NULL;
}

int main() {
  char filename[] = "input.txt";

  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    perror("File open error. Exiting...\n");
    return EXIT_FAILURE;
  }

  char line[MAX_COLS];

  while (fgets(line, sizeof(line), file)) {
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
            state = in_delimiter;
          }
          else if (!isspace(ch)) {
            lexeme[strlen(lexeme)] = ch;
            state = in_token;
          }
          break;

        case in_delimiter:
          if (!is_delimiter(ch)) {
            lexeme[strlen(lexeme)] = ' ';
            state = in_token;
            if (!isspace(ch)) {
              lexeme[strlen(lexeme)] = ch;
              state = in_token;
            }
            else {
              state = in_whitespace;
            }
          }
          else {
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
      printf("%s\n", lexeme);
    }
  }


  fclose(file);
  return 0;


}
