#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "pencil.h"


/* State variables - coordinates, angle and step length */
uint16_t x = 144, y = 720, angle = 0, step = 18;


/*
 * The turn, move and draw functions change the internal state
 * (coords, angle) to reflect their corresponding operations,
 * and outputs postscript to make the graphics device follow
 */
void
turn ( void )
{
    angle = (angle + 30) % 360;
}


void
move ( void )
{
    x += step * cos ( (angle * M_PI) / (double)180 );
    y -= step * sin ( (angle * M_PI) / (double)180 );
    printf ( "%d %d moveto\n", x, y );
}


void
draw ( void )
{
    x += step * cos ( angle * (M_PI / (double)180) );
    y -= step * sin ( angle * (M_PI / (double)180) );
    printf ( "%d %d lineto\n", x, y );
    printf ( "%d %d moveto\n", x, y );
}


/*
 * This function simply maps token values into operations.
 * Since the tokens and operations are 1-1 in this case,
 * a function like this one is really a bit unneccessary,
 * but it goes in for clarity, as it clearly marks the separation
 * between recognizing tokens and doing something with them.
 */
void
emit_instruction ( command_t word )
{
    switch ( word )
    {
        case TURN: turn(); break;
        case MOVE: move(); break;
        case DRAW: draw(); break;
        case END: break;
        default:
            fprintf ( stderr, "Unknown command; this should not happen!\n" );
            break;
    }
}


/*
 * Main function - glue everything together.
 */
int
main ( int argc, char **argv )
{
    // Initialize the state machine (if neccessary)
    init_transtab ();

    // Print a bit of startup, to start drawing somewhere sensible
    printf ( "newpath\n%d %d moveto\n", x, y );

    // Make sure the loop below runs at least once (the value given
    // here will be discarded in the first iteration) 
    command_t c = MOVE;

    // Until the end of the input stream,
    while ( c != END )
    {
        c = next ( stdin );     // Read until we have a token
        emit_instruction ( c ); // Emit the instruction it stands for
    }

    // Finalize by drawing the traced path, and exiting
    printf ( "stroke\nshowpage\n" );
    exit ( EXIT_SUCCESS );
}
