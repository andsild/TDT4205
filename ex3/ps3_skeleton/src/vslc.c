#include <stdio.h>
#include <stdlib.h>
#include <vslc.h>

node_t *root;

int
main ( int argc, char **argv )
{
    yyparse();
    simplify_tree ( &root, root );
    node_print ( root, 0 );
    destroy_subtree ( root );
}
