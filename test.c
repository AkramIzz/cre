#include <stdio.h>
#include <stdlib.h>

#include "re.h"

#define __MAX_SIZE_OF_LINKS 5
#define __MAX_SIZE_OF_STATES 15

void init_states(re_state *state) {
   re_state *current = state;
   while((int)(current - state) < __MAX_SIZE_OF_STATES) {
      current->is_final = false;
      current->links = malloc(sizeof(re_link) * __MAX_SIZE_OF_LINKS);
      current->links_num = 0;
      ++current;
   }
}

void print_states(re_state *start, int size) {
   for (re_state *state = start; (int)(state - start) < size; ++state) {
      printf("state: %p\tfinal: %i\tlinks: %i\n", &(*state), state->is_final, state->links_num);
      for (int i = 0; i < state->links_num; ++i) {
         printf("   link:\taccepts:%s\tnext: %p",
            state->links[i].accept, state->links[i].next
         );
      }
   }
}

int main(int argc, char const *argv[])
{
   re_compiled pattern = { malloc(sizeof(re_state) * __MAX_SIZE_OF_STATES), 0 };

   init_states(pattern.states);

   // Hand craft the pattern "ab*c+(ef){1, 3}[ghi][klm]"

   pattern.states->links->accept = "a";
   pattern.states->links->next = pattern.states + 1;
   pattern.states->links_num = 1;

   pattern.states[1].links[0].accept = "b";
   pattern.states[1].links[0].next = pattern.states + 1;
   pattern.states[1].links[1].accept = "c";
   pattern.states[1].links[1].next = pattern.states + 2;
   pattern.states[1].links_num = 2;

   pattern.states[2].links[0].accept = "c";
   pattern.states[2].links[0].next = pattern.states + 2;
   pattern.states[2].links[1].accept = "e";
   pattern.states[2].links[1].next = pattern.states + 3;
   pattern.states[2].links_num = 2;

   pattern.states[3].links[0].accept = "f";
   pattern.states[3].links[0].next = pattern.states + 4;
   pattern.states[3].links_num = 1;
   pattern.states[4].links[0].accept = "e";
   pattern.states[4].links[0].next = pattern.states + 5;
   pattern.states[4].links[1].accept = "ghi";
   pattern.states[4].links[1].next = pattern.states + 9;
   pattern.states[4].links_num = 1;
   
   pattern.states[5].links[0].accept = "f";
   pattern.states[5].links[0].next = pattern.states + 6;
   pattern.states[5].links_num = 1;
   pattern.states[6].links[0].accept = "e";
   pattern.states[6].links[0].next = pattern.states + 7;
   pattern.states[6].links[1].accept = "ghi";
   pattern.states[6].links[1].next = pattern.states + 9;
   pattern.states[6].links_num = 2;

   pattern.states[7].links[0].accept = "f";
   pattern.states[7].links[0].next = pattern.states + 8;
   pattern.states[7].links_num = 1;

   pattern.states[8].links[0].accept = "ghi";
   pattern.states[8].links[0].next = pattern.states + 9;
   pattern.states[8].links_num = 1;

   pattern.states[9].links[0].accept = "klm";
   pattern.states[9].links[0].next = pattern.states + 10;
   pattern.states[9].links_num = 1;

   pattern.states[10].is_final = true;
   pattern.states[11].links_num = 0;

   char *text = "helpaccefefgmhelp";
   re_match *m = match(&pattern, text);

   if (m && m->begin == 4 && m->end == 12) {
      fputs("match function test: Passed\n", stderr);
   } else {
      fputs("match function test: Failed\n", stderr);
      if (m) {
         fprintf(stderr, "begin: %i -> end: %i\n", m->begin, m->end);
      } else {
         fprintf(stderr, "return re_match* is NULL\n");
      }
   }
   
   return 0;
}