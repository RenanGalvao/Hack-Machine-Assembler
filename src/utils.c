#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void strip(char *s) {
  int last_index = strlen(s) - 1;
  // set all spaces with 0 from end to start
  while (isblank(s[last_index]) || isspace(s[last_index]) ||
         s[last_index] == '\n') {
    s[last_index--] = '\0';
  }

  // find first printable char
  int i = 0;
  while (isblank(s[i]) || isspace(s[i]) || s[i] == '\n') {
    i++;
  }

  // copy without left spaces
  char tmp[256];
  memset(tmp, 0, 256);

  int j = 0;
  for (; isprint(s[i]); i++, j++) {
    tmp[j] = s[i];
  }
  tmp[++j] = '\0';
  strcpy(s, tmp);
}

int is_empty_string(char *s) { return s[0] == '\0'; }

void reverse(char *s) {
  int i, j, c, len;
  len = strlen(s);
  for (i = 0, j = len - 1; i < (len / 2); i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

void decimal_to_binary(int d, char *s, int size, int pad) {
  double power, base, tmp;
  power = tmp = 0;
  base = 2;

  while (pow(base, power) < d) {
    power++;
  }

  if (pow(base, power) > d) {
    power--;
  }

  if (pad > 0) {
    for (int i = (int)power + 1; i < size; i++) {
      s[i] = '0';
    }
    s[size-1] = '\0';
  } else {
    s[(int)power + 1] = '\0';
  }

  while (power >= 0) {
    if (tmp + pow(base, power) <= d) {
      s[(int)power] = '1';
      tmp += pow(base, power);
    } else {
      s[(int)power] = '0';
    }

    power--;
  }

  reverse(s);
}

int is_numeric_string(char *s) {
  int len = strlen(s);
  for(int i = 0; i < len; i++) {
    // not a digit
    if(isdigit(s[i]) == 0){
      return 0;
    }
  }
  return 1;
}


void remove_spaces(char *s) {
  char tmp[256];
  int j = 0;

  for(int i = 0; i < (int)strlen(s); i++) {
    // not space
    if(isspace(s[i]) == 0) {
      tmp[j++] = s[i];
    }
  }

  tmp[j] = '\0';
  strcpy(s, tmp);
}

void clear_line(char *s) {
  // remove comment
  char *c = strchr(s, '/');
  if(c != NULL) {
    *c = '\0';
  }

  // remove spaces around
  strip(s);

  // remove spaces withing instruction
  remove_spaces(s);
}

char **explode(const char *source, const char *delimiter) {
  char *tmp = malloc(sizeof(char) * (strlen(source) + 1));
  strcpy(tmp, source);

  char **dest = NULL;
  char *token = strtok(tmp, delimiter);
  for(int i = 0; ; i++, token = strtok(NULL, delimiter)) {
    dest = realloc(dest, sizeof(char *) * (i + 1));

    // end of array
    if(token == NULL) {
      dest[i] = NULL;
      break;
    }

    dest[i] = malloc(sizeof(char) * (strlen(token) + 1));
    strcpy(dest[i], token);
  }

  free(tmp);
  return dest;
}

void free_exploded(char **arr) {
  for(int i = 0; arr[i] != NULL; i++) {
    free(arr[i]);
  }
  free(arr);
}

int arrlen(char **arr) {
  int size = 0;
  for(int i = 0; arr[i] != NULL; i++) {
    if(strlen(arr[i]) > 0) {
      size++;
    } else {
      break;
    }
  }

  return size;
}

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

  if (regexec(&reg, string, 1, pmatch, 0) != 0) {
    pmatch[0].rm_so = -1;
    pmatch[0].rm_eo = -1;
  }

  regfree(&reg);
}

char *get_file_name(char *file_path) {
  char *out = (char*)malloc(sizeof(char) * 128);
  char *c = strrchr(file_path, '/');
  if(c) {
    for(int i = 0; *c != '\0'; i++) {
      out[i] = *++c;
    }
  }

  c = strchr(out, '.');
  if(c) {
    *c = '\0';
  }

  return out;
}

FILE *open_source_file(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if(file == NULL) {
        perror("Error opening source file");
        exit(1);
    }

    return file;
}

FILE *open_output_file(char *file_path, FILE *source_file) {
    char out[128];
    char *c = strchr(file_path, '.');
    if (c) {
        *c = '\0';
    }
    snprintf(out, 128, "%s.hack", file_path);

    FILE *output_file = fopen(out, "w");
    if(output_file == NULL) {
        perror("Error opening output file");
        fclose(source_file);
        exit(1);
    }

    return output_file;
}

void write_output(const char *line, FILE *output_file) {
  char wider_line[18];
  snprintf(wider_line, 18, "%s\n", line);
  #ifdef DEBUG
  printf("[utils.c] write_output: '%s'\n", wider_line);
  #endif
  fputs(wider_line, output_file);
}
