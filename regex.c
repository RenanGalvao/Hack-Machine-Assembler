#include <stdio.h>
#include <stdlib.h>
#include "regex.h"

#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))

void re_match(const char *regex, const char *string, regmatch_t *pmatch) {
  regex_t reg;
  int err;

  err = regcomp(&reg, regex, REG_EXTENDED);
  if (err != 0) {
    char *errbuf;
    size_t errbuf_size = regerror(err, &reg, NULL, 0);

    errbuf = malloc(errbuf_size);
    regerror(err, &reg, errbuf, errbuf_size);

    fprintf(stderr, "%s\n", errbuf);
    free(errbuf);
    exit(1);
  }

  if (regexec(&reg, string, 1, pmatch, 0) == 0) {
    #ifdef DEBUG
    regoff_t off, len;

    off = pmatch[0].rm_so;
    len = pmatch[0].rm_eo - pmatch[0].rm_so;

    printf("[regex.c]: re_match original string: '%s'\n", string);
    printf("[regex.c]: re_match offset = %d; length = %d\n", off, len);
    printf("[regex.c]: re_match substring = '%.*s'\n", len, string + off);
    #endif // DEBUG
  } else {
    pmatch[0].rm_so = -1;
    pmatch[0].rm_eo = -1;
  }
}
