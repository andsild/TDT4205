#include <stdio.h>
#include <stdlib.h>
#include <vslc.h>

node_t *root;

int
main ( int argc, char **argv )
{
    yyparse();
    // node_print(root, 0);
    simplify_tree ( &root, root );

    /* Put the global names in the global symbol table */
    find_globals();
    //
    //
    /* Iterate over all global symbols, resolve uses of variables: */
    /* Obtain all global names */
    size_t n_globals = tlhash_size(global_names);
    symbol_t *global_list[n_globals];
    tlhash_values ( global_names, (void **)&global_list );
    
    /* Call bind_names on all those which are functions */
    for ( size_t i=0; i<n_globals; i++ )
        if ( global_list[i]->type == SYM_FUNCTION )
            bind_names ( global_list[i], global_list[i]->node );
    
    
    /* Print the final state of the symbol table(s) */
    print_symbols();
    printf ( "Bindings:\n" );
    print_bindings ( root );
    
    destroy_subtree ( root );
    destroy_symtab();
}
