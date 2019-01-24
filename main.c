#include <stdio.h>
#include <stdlib.h>

#include "input.h"

void repl();
void output_match(const char *regex, const char *text);

int main(int argc, char const *argv[])
{
   if (argc == 1) {
      repl();
   } else if (argc > 2) {
      const char *regex = argv[1];
      const char *text = argv[2];
      output_match(regex, text);
   } else {
      printf("Usage: cre [regex] [text]\n");
      exit(65);
   }
   return 0;
}

void repl() {
   for (;;) {
      char *regex = readline("> ");
      add_history(regex);
      char *text = readline(">> ");
      add_history(text);
      output_match(regex, text);
      free(regex);
      free(text);
   }
}

void output_match(const char *regex, const char *text) {
   
}