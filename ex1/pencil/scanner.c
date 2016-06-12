#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "pencil.h"

char dfa_edges[15][4] = {
    {'t', 'm', 'd', 'e'}, // 0
    {'u', '0', '0', '0' }, // 1
    {'r', '0', '0', '0'}, // 2
    {'n', '0', '0', '0'}, // 3

    {'0', '0', '0', '0'}, // 4
    {'o', '0', '0', '0'}, // 5
    {'v', '0', '0', '0'}, // 6
    {'e', '0', '0', '0'}, // 7

    {'0', '0', '0', '0'}, // 8
    {'r', '0', '0', '0'}, // 9
    {'a', '0', '0', '0'}, // 10
    {'w', '0', '0', '0'}, // 11

    {'0', '0', '0', '0'}, // 12
    {'n', '0', '0', '0'}, // 13
    {'d', '0', '0', '0'}, // 14
};
int dfa_nav[15][4] = {
    {1,5,9,13}, // 0
    {2, -1, -1, -1}, // 1
    {3, -1, -1, -1}, // 2
    {4, -1, -1, -1}, // 3

    {0,0,0,0}, // 4
    {6, -1, -1, -1}, // 5
    {7, -1, -1, -1}, // 6
    {8, -1, -1, -1}, // 7

    {0,0,0,0}, // 8
    {10, -1, -1, -1}, // 9
    {11, -1, -1, -1}, // 10
    {12, -1, -1, -1}, // 11

    {0,0,0,0}, // 12
    {14, -1, -1, -1},
    {15, -1, -1, -1},
};


    
/*
 * This function is called before anything else, to initialize the
 * state machine. It is certainly possible to create implementations
 * which don't require any initialization, so just leave this blank if
 * you don't need it.
 */
void
init_transtab ( void )
{
}


/*
 * Return the next token from reading the given input stream.
 * The words to be recognized are 'turn', 'draw' and 'move',
 * while the returned tokens may be TURN, DRAW, MOVE or END (as
 * enumerated in 'pencil.h').
 */
command_t
next ( FILE *input )
{
    int character;
    int currentNodeIndex = 0;
    while(EOF != (character = fgetc(input)))
    {
        if(character >= 65 && character <= 90 )
            character += 32;
        char readCharacted = (char)character;
        int originalNodeIndex = currentNodeIndex;
        for(int iPos = 0; iPos < sizeof(dfa_edges[currentNodeIndex]); iPos++)
            if(dfa_edges[currentNodeIndex][iPos] == readCharacted)
                currentNodeIndex = dfa_nav[currentNodeIndex][iPos];

        if(currentNodeIndex == originalNodeIndex)
            currentNodeIndex = 0;
        else if(currentNodeIndex == 4)
            return TURN;
        else if (currentNodeIndex == 8)
            return MOVE;
        else if (currentNodeIndex == 12)
            return DRAW;
        else if (currentNodeIndex == 15)
            return END;
    }
    return END; // EOF
}
