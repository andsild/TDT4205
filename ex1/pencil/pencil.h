#include <stdio.h>

typedef enum {
    TURN, MOVE, DRAW, END
} command_t;


// To be implemented in 'scanner.c'
void init_transtab ( void );
command_t next ( FILE *input );

// Control and command emission, implemented in 'pencil.c'
void turn ( void );
void move ( void );
void draw ( void );
void emit_instruction ( command_t word );
int main ( int argc, char **argv );
