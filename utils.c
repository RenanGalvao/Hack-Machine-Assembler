#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "dynamic-string-array.h"

void rstrip(char *s) {
  int lastIndex = strlen(s) - 1;
  // set all spaces with 0 from end to start
  while (isblank(s[lastIndex]) || isspace(s[lastIndex]) ||
         s[lastIndex] == '\n') {
    s[lastIndex--] = '\0';
  }
}

void lstrip(char *s) {
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

void strip(char *s) {
  rstrip(s);
  lstrip(s);
}

int isEmptyString(char *s) { return s[0] == '\0'; }

void reverse(char *s) {
  int i, j, c, len;
  len = strlen(s);
  for (i = 0, j = len - 1; i < (len / 2); i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

void decimalToBinaryString(int d, char *s, int size, int pad) {
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

int isNumericString(char *s) {
  int len = strlen(s);
  for(int i = 0; i < len; i++) {
    // not a digit
    if(isdigit(s[i]) == 0){
      return 0;
    }
  }
  return 1;
}


void removeSpaces(char *s) {
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

void clearLine(char *s) {
  // remove comment
  char *c = strchr(s, '/');
  if(c != NULL) {
    *c = '\0';
  }

  // remove spaces around
  strip(s);

  // remove spaces withing instruction
  removeSpaces(s);
}

void getSourceCode(
  char *filePath, 
  struct DynamicStringArray *source, 
  struct DynamicStringArray *cleanSource
) {
    FILE *sourceFile = fopen(filePath, "r");
    if(sourceFile == NULL) {
        perror("Error opening source file");
    }

    int buffSize = 256;
    char line[buffSize];
    while(fgets(line, buffSize, sourceFile) != NULL) {
        source->put(source, line);

        clearLine(line);

        // reject empty lines
        if(isEmptyString(line))
            continue;

        // pure instruction
        cleanSource->put(cleanSource, line);
    }

    fclose(sourceFile);
}

void saveHackCode(char *sourceFile, struct DynamicStringArray *output) {
  char out[128];
    char *c = strchr(sourceFile, '.');
    if (c) {
        *c = '\0';
    }
    snprintf(out, 128, "%s.hack", sourceFile);

    FILE *outFile = fopen(out, "w");
    if(outFile == NULL) {
        perror("Error opening output file");
    }

    // write to file
    for(int i = 0; i < output->count; i++) {
        #ifdef DEBUG
        printf("[utils.c] %d: '%s'\n", i, output->stringArray[i]);
        #endif // DEBUG
        char line[18]; // 16 bits + \n + \0
        snprintf(line, 18, "%s\n", output->stringArray[i]);
        fputs(line, outFile);
    }
    fclose(outFile);
}
