#include <stdio.h>
#include <stdlib.h>
#include <vslc.h>

node_t *root;

int
main ( int argc, char **argv )
{
    yyparse();
    simplify_tree ( &root, root );
    // node_print(root, 0);
    find_globals();
    size_t n_globals = tlhash_size(global_names);
    symbol_t *global_list[n_globals];
    tlhash_values ( global_names, (void **)&global_list );
    for ( size_t i=0; i<n_globals; i++ )
        if ( global_list[i]->type == SYM_FUNCTION )
            bind_names ( global_list[i], global_list[i]->node );

//    print_globals();

    generate_program ();    

    destroy_subtree ( root );
    destroy_symtab();
}
