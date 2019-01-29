#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "re.h"

re_compiled compile(const char *regex) {

}

static bool char_in_str(char c, char *str) {
   for (char *ch = str; *ch != '\0'; ++ch) {
      if (c == *ch) {
         return true;
      }
   }
   return false;
}

void print_state(re_state *init, re_state *state) {
   printf("state: %li\tfinal: %i\tlinks: %i\n", state-init, state->is_final, state->links_num);
   for (int i = 0; i < state->links_num; ++i) {
      printf("   ->link\taccepts:%s\tnext: %li\n",
         state->links[i].accept, state->links[i].next-init
      );
   }
}

re_match* match(re_compiled *pattern, const char *text) {
   char terminating_char = '\0';
   for (const char *begin = text; *begin != terminating_char; begin++) {
      re_state *current_state = pattern->states;
      
      const char *end = begin;
      bool char_match = true;
      while (!current_state->is_final 
            && *end != '\0'
            && char_match) {
         print_state(pattern->states, current_state);
         printf("%c\n\n", *end);
         // accept a character
         char_match = false;
         for (int i = 0; i < current_state->links_num; ++i) {
            if (char_in_str(*end, current_state->links[i].accept)) {
               current_state = current_state->links[i].next;
               end++;
               char_match = true;
               break;
            }
         }
      }

      // match found.
      if (current_state->is_final) {
         re_match *m = malloc(sizeof(re_match));
         m->begin = (int) (begin - text);
         m->end = (int) (end - text) - 1; // *end itself isn't part of the match
         return m;
      }
   }
   // no match found
   return NULL;
}

re_match* find_all(re_compiled *pattern, const char *text) {
   re_match *match, *matches = malloc(sizeof(match) * MAX_MATCHES_SIZE);
   re_match *last_match = matches;
   
   re_iter iter = find_iter(pattern, text);
   while ((match = find_next(iter))
         && (last_match - matches) < MAX_MATCHES_SIZE) {
      memcpy(last_match, match, sizeof(match));
      ++last_match;
   }
   
   last_match->begin = 0;
   last_match->end = 0;

   return matches;
}

re_iter find_iter(re_compiled *pattern, const char *text) {
   re_iter iter;
   iter.pattern = pattern;
   iter.text = malloc(sizeof(char *));
   iter.last_match = malloc(sizeof(char *));

   int text_len = strlen(text);
   *iter.text = malloc(strlen(text));
   memcpy(*iter.text, text, text_len);
   
   *iter.last_match = *iter.text;
   return iter;
}

re_match* find_next(re_iter iter) {
   re_match *match_obj = match(iter.pattern, *iter.last_match);
   if (match_obj) {
      *iter.last_match += match_obj->end;
      return match_obj;
   }
   end_iter(iter);
   return NULL;
}

void end_iter(re_iter iter) {
   free(*iter.text);
   free(iter.text);
   free(iter.last_match);
}