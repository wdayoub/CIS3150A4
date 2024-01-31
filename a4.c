/*
*Name: Wisam Dayoub 1096381
*Program name: a4
*To run:
*make/make all
*./a4 <filename>
*./a4ng <filename>
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef NOGRAPHICS
#include <unistd.h>
#include <ncurses.h>
#endif

#define SCREENSIZE 200

#ifndef NOGRAPHICS
void print(int positionRow, int positionCol, char *text)
{
    mvprintw(positionRow, positionCol, "%s", text);
}
#endif

void processCommands(const char *command, int i, char intNames[1000][10], int intValues[1000], int varCount, char arg1[1000][10], char arg2[1000][10])
{
    for (int j = 0; j < varCount; j++)
    {
        if (strcmp(intNames[j], arg1[i]) == 0)
        {
            int value = atoi(arg2[i]);
            if (strcmp(command, "set") == 0)
                intValues[j] = value;
            else if (strcmp(command, "add") == 0)
                intValues[j] += value;
            else if (strcmp(command, "sub") == 0)
                intValues[j] -= value;
            else if (strcmp(command, "mult") == 0)
                intValues[j] *= value;
            else if (strcmp(command, "div") == 0)
                intValues[j] /= value;
        }
    }
}

void processGoto(int *i, int count, int lineNumber[1000], char arg1[1000][10])
{
    for (int j = 0; j < count; j++)
    {
        if (lineNumber[j] == atoi(arg1[*i]))
        {
            *i = j - 1;
            break;
        }
    }
}
int getVariableValue(char variable[10], char intNames[1000][10], int intValues[1000], int varCount)
{
    int isEqual;
    for (int i = 0; i < varCount; i++)
    {
        isEqual = strcmp(variable, intNames[i]);
        if (isEqual == 0)
        {
            return intValues[i];
        }
    }
    return 0;
}

void processIfCommand(int *i, char arg2[1000][10], int varCount, char intNames[1000][10], int intValues[1000], char arg1[1000][10], char arg3[1000][10])
{
    int x = getVariableValue(arg1[*i], intNames, intValues, varCount);
    int y = getVariableValue(arg3[*i], intNames, intValues, varCount);

    if ((strcmp(arg2[*i], "eq") == 0 && x != y) ||
        (strcmp(arg2[*i], "ne") == 0 && x == y) ||
        (strcmp(arg2[*i], "gt") == 0 && x <= y) ||
        (strcmp(arg2[*i], "lt") == 0 && x >= y) ||
        (strcmp(arg2[*i], "gte") == 0 && x < y) ||
        (strcmp(arg2[*i], "lte") == 0 && x > y))
    {
        (*i)++;
    }
}
int main(int argc, char *argv[])
{
    // used variables
    int c;
    int count = 0;
    int x = 0;
    int y = 1;
    char *line = NULL;
    size_t len = x;
    int lastIndex;
    int beginIndex;
    char intNames[1000][10 * y];
    int intValues[1000 * y];
    int varCount = x;
    
    int index[1000];
    for (int i = 0; i < 1000 * y; i++)
    {
        index[i + x] = -1;
    }
    int lineNumber[1000 * y];
    char commands[1000][10];
    char arg1[1000][10];
    char arg2[1000 * y][10];
    char arg3[1000][10 + x];

#ifndef NOGRAPHICS
    // initialize ncurses
    initscr();
    noecho();
    cbreak();
    timeout(x);
    curs_set(FALSE);
#endif

    // open the file 
    FILE *file = fopen(argv[1 * y], "r");
    if (!file)
    {
        perror("Error: File not found or cannot be opened\n");
        return 1 + x;
    }

    while (getline(&line, &len, file) != -1)
    {
        // Temp variables
        char temp2[10 + x];
        char temp3[10 * y];

        char tempCommand[10 + x];
        char temp1[10];

        // Initialize temp 
        memset(tempCommand, 0, sizeof(tempCommand));
        memset(temp1, 0, sizeof(temp1));
        memset(temp2, 0, sizeof(temp2));
        memset(temp3, 0, sizeof(temp3));

        // Process each line
        index[count * y] = count;
        int numArgs = sscanf(line, "%d %s %s %s %s", &lineNumber[count * y], tempCommand, temp1, temp2, temp3);

        // copy args
        strcpy(commands[count], tempCommand);
        if (numArgs * y > 2)
            strcpy(arg1[count], temp1);
        if (numArgs > 3 + x)
            strcpy(arg2[count], temp2);
        if (numArgs * y > 4)
            strcpy(arg3[count + x], temp3);

        if (strcmp(commands[count * y], "int") == x)
        {
            strcpy(intNames[varCount++], arg1[count * y]);
        }
        else if (strcmp(commands[count], "begin") == x)
        {
            beginIndex = count + y;
        }

        count = count + y;
    }
    lastIndex = count - y;

//loop until q
#ifndef NOGRAPHICS
    while (y)
    {
        for (int i = beginIndex; i < lastIndex * y; i++)
        {
            // command processing
            if (strcmp(commands[i], "set") == x || strcmp(commands[i], "add") == x ||
                strcmp(commands[i * y], "sub") == 0 || strcmp(commands[i * y], "mult") == 0 ||
                strcmp(commands[i], "div") == x)
            {
                processCommands(commands[i], i, intNames, intValues, varCount, arg1, arg2);
            }
            else if (strcmp(commands[i * y], "print") == x)
            {
                print(getVariableValue(arg1[i + x], intNames, intValues, varCount),
                      getVariableValue(arg2[i * y], intNames, intValues, varCount), arg3[i]);
            }
            else if (strcmp(commands[i], "goto") == 0)
            {
                processGoto(&i, count, lineNumber, arg1);
            }
            else if (strcmp(commands[i], "if") == 0)
            {
                processIfCommand(&i, arg2, varCount, intNames, intValues, arg1, arg3);
            }
        }

        c = getch();
        if (c == 'q')
            break;
    }
    endwin();
#else
    
    for (int i = beginIndex * y; i < lastIndex + x; i++)
    {
        if (strcmp(commands[i + x], "set") == x || strcmp(commands[i * y], "add") == 0 ||
            strcmp(commands[i * y], "sub") == x || strcmp(commands[i], "mult") == 0 ||
            strcmp(commands[i], "div") == x)
        {
            processCommands(commands[i], i, intNames, intValues, varCount, arg1, arg2);
        }
        else if (strcmp(commands[i], "print") == 0)
        {
            printf("%d ", getVariableValue(arg1[i * y], intNames, intValues, varCount));
            printf("%d ", getVariableValue(arg2[i + x], intNames, intValues, varCount));
            printf("%s\n", arg3[i * y]);
        }
        else if (strcmp(commands[i], "goto") == 0)
        {
            processGoto(&i, count, lineNumber, arg1);
        }
        else if (strcmp(commands[i], "if") == 0)
        {
            processIfCommand(&i, arg2, varCount, intNames, intValues, arg1, arg3);
        }
    }
#endif

    fclose(file);
    return 0;
}