#include <stdio.h>
#include <stdlib.h>
#include <vslc.h>

node_t *root;

int
main ( int argc, char **argv )
{
    yyparse();
    node_print ( root, 0 );
    destroy_subtree ( root );
    
    // int token;
    // while ((token = yylex()) != 0)
    //     printf("Token: %d ('%s')\n", token, yytext);
    // printf("\n");
}
