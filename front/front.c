#include <stdio.h>
#include <ctype.h>

int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nexToken;
FILE *in_fp, *fopen();

void addChar();
void getChar();
void getNonBlank();
int lex();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10

int main() {

}
