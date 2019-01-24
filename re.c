#include <stdlib.h>
#include <string.h>

#include "re.h"

typedef struct {
   char *accept;
   struct re_state_t *next;
} re_link;

typedef struct re_state_t {
   bool is_final;
   re_link *links;
   int links_num;
} re_state;

typedef struct re_compiled_t {
   re_state *states;
   int states_num;
} re_compiled;

typedef struct re_iter_t {
   re_compiled pattern;
   char **text;
   char **last_match;
} re_iter;

re_compiled compile(const char *regex) {

}

re_match* match(re_compiled pattern, const char *text) {

}

re_match* find_all(re_compiled pattern, const char *text) {
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

re_iter find_iter(re_compiled pattern, const char *text) {
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