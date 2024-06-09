#include <stdbool.h>
#include <ctype.h>

bool is_delimiter(char ch) {
  return (ch == ';');
}

bool is_operator(char ch) {
  return (ch == '+' || ch == '-' || ch == '*' || ch == '/'
          || ch == '<' || ch == '>' || ch == '=');
}

bool is_valid_id(char* str) {
  return (!isdigit(str[0]) && !is_delimiter(str[0]));
}

bool is_whitespace(char ch) {
  return (ch == ' ' || ch == '\t' || ch == '\n');
}
