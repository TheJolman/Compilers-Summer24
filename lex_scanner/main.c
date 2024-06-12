/* This is essentially the lexical scanner from Parsons' compiler book but translated to C
 * Page 58/58
 * Instead of parsing Pascal it will parse my own very simple language 
 * c style block comments
 * lines delimited with ;
 * keywords: if, else, elif, for, while
 * operators: +-=/%*&|
 * other punctuation: ;,(){}[]
 * */

#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_COLS 1024
typedef unsigned uint;

typedef enum {
  Start,
  InIdentifier,
  EndIdentifier,
  InNumber,
  EndNumber,
  FoundSlash,
  InComment,
  FoundStarInComment,
  EndComment,
  FoundPunctuation,
  Error,
  Num_States
} State;

typedef enum {
  Letter,
  Digit,
  Space,
  Punctuation,
  Slash,
  Star,
  Num_Possible_Inputs
} Input;

const int transition_table[Num_States][Num_Possible_Inputs] = {
                      /* Letter, Digit, Space, Punct, /,  * */
  /* Start */     {InIdentifier, InNumber,     Start,         FoundPunctuation, FoundSlash,    FoundPunctuation},
  /* InId */      {InIdentifier, InIdentifier, EndIdentifier, EndIdentifier,    EndIdentifier, EndIdentifier},
  /* EndId */     {Start,        Start,        Start,         Start,            Start,         Start},
  /* InNum */     {EndNumber,    InNumber,     EndNumber,     EndNumber,        EndNumber,     EndNumber},
  /* EndNum */    {Start,        Start,        Start,         Start,            Start,         Start},
  /* FoundSlash */{InIdentifier, InNumber,     Start,         Error,            Error,         InComment},
  /* InComment */ {InComment,    InComment,    InComment,     InComment,        FoundStarInComment, InComment},
  /* FoundStar */ {InComment,    InComment,    InComment,     InComment,        InComment,     EndComment},
  /* EndComment */{Start,        Start,        Start,         Start,            Start,         Start},
  /* FoundPunct */{InIdentifier, InNumber,     Start,         Error,            Error,         Error},
  /* Error */    {Error,        Error,        Start,         Error,            Error,         Error},
};

const char *keywords[5] = {
  "for", "while", "if", "elif", "else",
};

const char *punctuation = ";,(){}[]";


int main(void) {
  const char input_filename[] = "input.txt";
  FILE *input_file = fopen(input_filename, "r");

  if (!input_file) {
    fprintf(stderr, "Falied to open input file. Exiting...\n");
    return 1;
  }

  char line[MAX_COLS];

  while (fgets(line, sizeof(line), input_file)) {
    uint col = 0;
    uint line_len = strlen(line);
    char lexeme[MAX_COLS] = {0};
    State state = Start;
    Input input;
    
    
    while (col < line_len) {
      char ch = line[col];

      printf("%c", ch);

      state = transition_table[state][input];

      col++;
    }
  }

  return 0;
}
