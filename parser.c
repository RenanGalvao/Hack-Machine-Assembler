#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "regex.h"
#include "utils.h"
#include "tables.h"

static int nextAddress = 16;

// first scan - get labels
void firstScan(struct DynamicStringArray *cleanSource, struct HashMap *symbolsTable) {
    char reIsLabel[] = "\\(.+\\)";
    regmatch_t pmatch[1];

    // compesate for labels
    int labelCount = 0;

    for(int i = 0; i < cleanSource->count; i++) {
        re_match(reIsLabel, cleanSource->stringArray[i], pmatch);

        int off = pmatch[0].rm_so;
        int len = pmatch[0].rm_eo - pmatch[0].rm_so;

        #ifdef DEBUG
        printf("[parser.c] original string: '%s'; off: %d; length: %d; cleanSource line:%d;\n", cleanSource->stringArray[i], off, len, i);
        #endif // DEBUG

        if(off != -1) {
            char tmp[128];
            char binary[17];

            // strip '(' and ')'
            snprintf(tmp, 128, "%.*s", len - 2, cleanSource->stringArray[i] + 1);

            decimalToBinaryString(i - labelCount, binary, 17, 1);
            symbolsTable->put(symbolsTable, tmp, binary);
            labelCount++;

            #ifdef DEBUG
            printf("[parser.c] label: '%s'; address: '%s'\n", tmp, binary);
            #endif // DEBUG
        }
    }
}

// second scan - translate instructions and find errors
// ignore label instructions (xxx), just A and C get translated
int secondScan(struct DynamicStringArray *source, struct DynamicStringArray *cleanSource, struct HashMap *symbolsTable, struct DynamicStringArray *output) {
    char reIsAinstruction[] = "@";
    char reIsCinstruction[] = "[;=]";
    regmatch_t pmatch[1];

    struct HashMap *compTable = getCompTable();
    struct HashMap *destTable = getDestTable();
    struct HashMap *jumpTable = getJumpTable();

    for(int i = 0; i < cleanSource->count; i++) {
        char tmp[128];
        char binary[17];

        char original[128];
        strcpy(original, cleanSource->stringArray[i]);

        char *tableValue = NULL;

        // A Instruction
        re_match(reIsAinstruction, cleanSource->stringArray[i], pmatch);
        if(pmatch[0].rm_so != -1) {
            // strip @
            snprintf(tmp, 128, "%s", cleanSource->stringArray[i] + 1);

            if(strlen(tmp) == 0) {
                badInstruction(source, original);
                cleanUp(compTable, destTable, jumpTable);

                return -1;
            }

            tableValue = symbolsTable->get(symbolsTable, tmp);
            if(!tableValue) {
                if(isNumericString(tmp)) {
                    decimalToBinaryString(atoi(tmp), binary, 17, 1);
                } else {
                    decimalToBinaryString(nextAddress++, binary, 17, 1);
                    // save new symbol
                    symbolsTable->put(symbolsTable, tmp, binary);
                }
            } else {
                strcpy(binary, tableValue);
            }

            #ifdef DEBUG
            printf("[parser.c] instruction #%d; original string: '%s'; binary value: '%s';\n", i, original, binary);
            #endif // DEBUG

            output->put(output, binary);

            // next instruction
            continue;
        }

        // C Instruction
        re_match(reIsCinstruction, cleanSource->stringArray[i], pmatch);
        if(pmatch[0].rm_so != -1) {
            char dest[4];
            char comp[8]; // 7 with 'a' bit
            char jump[4];
            char out[17];
            decimalToBinaryString(0, dest, 4, 1);
            decimalToBinaryString(0, comp, 8, 1);
            decimalToBinaryString(0, jump, 4, 1);

            // jump
            if(strchr(cleanSource->stringArray[i], ';')) {
                char *token = strtok(cleanSource->stringArray[i], ";");

                if(token == NULL) {
                    badInstruction(source, original);
                    cleanUp(compTable, destTable, jumpTable);

                    return -1;
                }

                // memory computation a = 1
                comp[0] = strchr(token, 'M') ? '1' : '0'; 

                tableValue = compTable->get(compTable, token);
                if(!tableValue) {
                    badInstruction(source, original);
                    cleanUp(compTable, destTable, jumpTable);

                    return -1;
                }
                strcpy(comp+1, tableValue);

                token = strtok(NULL, ";");
                tableValue = jumpTable->get(jumpTable, token);
                if(!tableValue) {
                    badInstruction(source, original);
                    cleanUp(compTable, destTable, jumpTable);

                    return -1;
                }
                strcpy(jump, tableValue);

                #ifdef DEBUG
                printf("[parser.c] instruction #%d; original string: '%s'; comp: %s; dest: %s; jump:%s;\n", i, original, comp, dest, jump);
                #endif // DEBUG

                snprintf(out, 17, "111%s%s%s", comp, dest, jump);
                output->put(output, out);
            }
            // assignment
            else {
                char *token = strtok(cleanSource->stringArray[i], "=");
                if(token == NULL) {
                    badInstruction(source, original);
                    cleanUp(compTable, destTable, jumpTable);

                    return -1;
                }

                // set bits for destination
                // M = [2]; D = [1]; A = [0]
                // dest[2] = strchr(token, 'M') ? '1' : '0'; 
                // dest[1] = strchr(token, 'D') ? '1' : '0'; 
                // dest[0] = strchr(token, 'A') ? '1' : '0'; 
                tableValue = destTable->get(destTable, token);
                if(!tableValue) {
                    badInstruction(source, original);
                    cleanUp(compTable, destTable, jumpTable);

                    return -1;
                }
                strcpy(dest, tableValue);

                token = strtok(NULL, "=");
                 // memory computation a = 1
                comp[0] = strchr(token, 'M') ? '1' : '0';
                tableValue = compTable->get(compTable, token);
                if(!tableValue) {
                    badInstruction(source, original);
                    cleanUp(compTable, destTable, jumpTable);

                    return -1;
                }
                strcpy(comp+1,  tableValue);

                #ifdef DEBUG
                printf("[parser.c] instruction #%d; original string: '%s'; comp: %s; dest: %s; jump:%s;\n", i, original, comp, dest, jump);
                #endif // DEBUG

                snprintf(out, 17, "111%s%s%s", comp, dest, jump);
                output->put(output, out);
            }

            // next instruction
            continue;
        }
    }

    cleanUp(compTable, destTable, jumpTable);

    return 0;
}

void badInstruction(struct DynamicStringArray *source, const char *s) {
    int line = source->find(source, s, 1);
    strip(source->stringArray[line]);
    fprintf(stderr, "Bad instruction at line %d: '%s'\n", line+1, source->stringArray[line]);
}

void cleanUp(struct HashMap *compTable, struct HashMap *destTable, struct HashMap *jumpTable) {
    compTable->del(compTable);
    destTable->del(destTable);
    jumpTable->del(jumpTable);

}
