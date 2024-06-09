#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define MAX_TOKEN_LENGTH 100

typedef enum {
  RESERVED_WORD,
  IDENTIFIER,
  NUMBER,
  SPECIAL_SYMBOL,
  OPERATOR,
  INVALID
} TokenType;

const char* reserved_words[] = {
  "cin>>", "for", "int", "while", "for", "if", "else",
  "do", "enum", "const", "char", "float", "double"
};

const char* special_symbols[] = {
  ";", "(", ")", "{", "}", "[", "]", "=", ">",
  "<", ">=", ","
};

const char* operators[] = {
  "+", "-", "++", "--", "*", "/", "%"
};

bool is_reserved_word(const char* token) {
  for (int i = 0; i < sizeof(reserved_words) / sizeof(reserved_words[0]); i++) {
    if (strcmp(token, reserved_words[i]) == 0)
      return true;
  }
  return false;
}

bool is_special_symbol(const char* token) {
  for (int i = 0; i < sizeof(special_symbols) / sizeof(special_symbols[0]); i++) {
    if (strcmp(token, special_symbols[i]) == 0)
      return true;
  }
  return false;
}

bool is_operator(const char* token) {
  for (int i = 0; i < sizeof(operators) / sizeof(operators[0]); i++) {
    if (strcmp(token, operators[i]) == 0)
      return true;
  }
  return false;
}

TokenType get_token_type(const char* token) {
  if (is_reserved_word(token))
    return RESERVED_WORD;
  if (is_special_symbol(token))
    return SPECIAL_SYMBOL;
  if (is_operator(token))
    return OPERATOR;
  if (isdigit(token[0]))
    return NUMBER;
  if (isalpha(token[0]))
    return IDENTIFIER;
  return INVALID;
}

bool is_delimiter(char c) {
  char token[2] = {c, '\0'};
  return isspace(c) || is_special_symbol(token) || is_operator(token);
}

int main() {
  char input[1000];
  char token[MAX_TOKEN_LENGTH];
  int col, token_index;

  printf("Enter a statement: ");
  if (!fgets(input, sizeof(input), stdin))
    perror("fgets()");

  col = 0;
  while (input[col] != '\0') {
    token_index = 0;

    // Skip whitespace
    while (isspace(input[col]))
      col++;

    // Check for delimiters
    if (is_delimiter(input[col])) {
      token[token_index++] = input[col++];
      token[token_index] = '\0';
    }
    else {
      // Extract token until a delimiter or end of input is reached
      while (!is_delimiter(input[col]) && input[col] != '\0') {
        token[token_index++] = input[col++];
      }
      token[token_index] = '\0';
    }

    if (strlen(token) > 0) {
      TokenType type = get_token_type(token);
      switch (type) {
        case RESERVED_WORD:
          printf("%s\treserved word\n", token);
          break;
        case IDENTIFIER:
          printf("%s\tidentifier\n", token);
          break;
        case NUMBER:
          printf("%s\tnumber\n", token);
          break;
        case SPECIAL_SYMBOL:
          printf("%s\tspecial symbol\n", token);
          break;
        case OPERATOR:
          printf("%s\toperator\n", token);
          break;
        case INVALID:
          printf("%s\tinvalid\n", token);
          break;
      }
    }
  }

  return 0;
}
