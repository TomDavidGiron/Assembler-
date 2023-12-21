#ifndef MACRO_H
#define MACRO_H

#define MAX_LINE_LENGTH 1000

struct MacroNode {
    char macroName[MAX_LINE_LENGTH];
    char *macroContent;
    struct MacroNode *next;
};

int processFile(char *file_name);

#endif /** MACRO_H */





