#ifndef cre_re_h
#define cre_re_h

#include <stdbool.h>

#define MAX_STATES_SIZE 25
#define MAX_MATCHES_SIZE 25

typedef struct re_compiled_t re_compiled;

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
   re_compiled *pattern;
   char **text;
   char **last_match;
} re_iter;

typedef struct {
   unsigned int begin;
   unsigned int end;
} re_match;

typedef struct re_iter_t re_iter;

re_compiled compile(const char *regex);
re_match* match(re_compiled *pattern, const char *text);
re_match* find_all(re_compiled *pattern, const char *text);
re_iter find_iter(re_compiled *pattern, const char *text);
re_match* find_next(re_iter iter);
void end_iter(re_iter iter);

#endif