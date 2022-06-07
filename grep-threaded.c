#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* (c) Larry Herman, 2022.  You are allowed to use this code yourself, but
 * not to provide it to anyone else.
 */

#define LINE_SZ 256

typedef struct grep {

  char *word;
  char *file;

} grep_threaded;

static void *read_line(void *ptr);

int main(int argc, char *argv[]) {

  grep_threaded *grep = malloc(sizeof(grep_threaded) * (argc - 2));
  pthread_t *t_ids = malloc(sizeof(pthread_t) * (argc - 2));
  int files, occurrences = 0;
  void *val = NULL;

  if(grep == NULL || t_ids == NULL) return 0;

  if(argc > 2) {

    for(files = 0; files < argc - 2; files++) {

      (grep + files)->word = argv[1];
      (grep + files)->file = argv[files + 2];
      pthread_create(t_ids + files, NULL, read_line, (grep + files));

    }

    for(files = 0; files < argc - 2; files++) {

      pthread_join(*(t_ids + files), &val);
      occurrences += * (int*) val;
      free(val);

    }

  }

  printf("%d\n", occurrences);

  free(t_ids);
  free(grep);

  return 0;

}

static void *read_line(void *ptr) {

  grep_threaded *grep = (grep_threaded*) ptr;
  FILE *fp;
  char line[LINE_SZ], *string_to_search_for;
  int *total_ptr = malloc(sizeof(int));

  if(total_ptr == NULL) return NULL;
  
  *total_ptr = 0;

  string_to_search_for = grep->word;

  fp= fopen(grep->file, "r");

  if (fp != NULL) {

    while (fgets(line, LINE_SZ, fp) != NULL)
      if (strstr(line, string_to_search_for) != NULL) (*total_ptr)++;

    fclose(fp);

  }

  /*printf("%d\n", total);*/

  return total_ptr;

}
