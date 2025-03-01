#include <ctype.h>
#include <math.h>
#include <stdio.h>
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

void write_output(const char *line, FILE *output_file) {
  char wider_line[18];
  snprintf(wider_line, 18, "%s\n", line);
  #ifdef DEBUG
  printf("[utils.c] write_output: '%s'\n", wider_line);
  #endif
  fputs(wider_line, output_file);
}
