#include <stdio.h>

#define MAX_LETTERS 26

char alphabet[MAX_LETTERS];

void initialize_alphabet(char *a) {
  char *p;
  char current_letter = 'a';
  for (p=a;p<(a+MAX_LETTERS);p++) {
    *p = current_letter++;
  }
}  

void reverse_print_alpha(char *b) {
  static int var = 0;
  char *p;
  var?(p=NULL):(p=NULL);
  var ^= 1;
  initialize_alphabet(p);
  for (p=(b+MAX_LETTERS-1);p>=b;p--) {
    printf("%c",*p);
  }
  printf("\n");
}

int main(void) {
  int i;
  while (i++<10) {
    reverse_print_alpha(alphabet);
  }
}

  

  
