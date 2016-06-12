#include <vslc.h>

/* Hash table for global variables and function names */
tlhash_t *global_names;

/* Dynamic array for string table */
char **string_list;
size_t
    n_string_list = 8,  /* Size of the table */
    stringc = 0;        /* Count of strings in it */

/* Stack of tables for local scopes */
static tlhash_t **scopes = NULL;        /* Dynamic array of tables*/
static size_t
    n_scopes = 1,       /* Size of the table */
    scope_depth = 0;    /* Number of scopes presently on stack */


/* Function to dump all the global symbols on stdout */
void
print_globals ( void )
{
    /* Get the number of symbols, size up a temporary list and fill it */
    size_t n_globals = tlhash_size(global_names);
    symbol_t *global_list[n_globals];
    tlhash_values ( global_names, (void **)&global_list );

    /* Iterate over the temporary list, printing entries */
    for ( size_t g=0; g<n_globals; g++ )
        fprintf ( stderr, "global: %s\n", global_list[g]->name );
}


/* Convenience function, to save repetitions of string key scheme */
static void
insert_symbol ( tlhash_t *tab, symbol_t *sym )
{
    tlhash_insert ( tab, sym->name, strlen(sym->name), sym );
}


/* Convenience function to insert strings in the dynamic table,
 * and resize (double) its capacity when/if it runs out
 */
static void
add_string ( node_t *string )
{
    /* Move string from node to table */
    string_list[stringc] = string->data;
    /* Put index in node instead */
    string->data = malloc ( sizeof(size_t) );
    *((size_t *)string->data) = stringc;
    stringc++;

    /* Resize the table if it is full */
    if ( stringc >= n_string_list )
    {
        n_string_list *= 2;
        string_list = realloc ( string_list, n_string_list * sizeof(char *) );
    }
        
}


/* Fill the table of global entries with appropriate symbol structures */
void
find_globals ( void )
{
    /* Initialize dynamic lists/tables */
    global_names = malloc ( sizeof(tlhash_t) );
    tlhash_init ( global_names, 32 );
    string_list = malloc ( n_string_list * sizeof(char * ) );
    size_t n_functions = 0;

    /* Go through the children of the root program node, i.e. the globals */
    node_t *global_list = root->children[0];
    for ( uint64_t g=0; g<global_list->n_children; g++ )
    {
        node_t
            *global = global_list->children[g],     /* List of nodes */
            *namelist;  /* Temp. list for declarations of multiple vars */
        symbol_t *symbol;
        switch ( global->type )
        {
            /* Functions: */
            case FUNCTION:
                /* Set up the entry for the function itself */
                symbol = malloc ( sizeof(symbol_t) );
                *symbol = (symbol_t) {
                    .type = SYM_FUNCTION,
                    .name = global->children[0]->data,
                    .node = global->children[2],
                    .seq = n_functions,
                    .nparms = 0,
                    .locals = malloc ( sizeof(tlhash_t) )
                };
                n_functions++;

                /* Initialize its local table, and fill in the parameters */
                tlhash_init ( symbol->locals, 32 );
                if ( global->children[1] != NULL )
                {
                    symbol->nparms = global->children[1]->n_children;
                    for ( int p=0; p<symbol->nparms; p++ )
                    {
                        node_t *param = global->children[1]->children[p];
                        symbol_t *psym = malloc ( sizeof(symbol_t) );
                        *psym = (symbol_t) {
                            .type = SYM_PARAMETER,
                            .name = param->data,
                            .node = NULL,
                            .seq = p,
                            .nparms = 0,
                            .locals = NULL
                        };
                        tlhash_insert (
                            symbol->locals, psym->name, strlen(psym->name), psym
                        );
                    }
                }
                insert_symbol ( global_names, symbol );
                break;
            /* Global variables */
            case DECLARATION:
                /* Go through all vars declared in this statement */
                namelist = global->children[0];
                for ( uint64_t d=0; d<namelist->n_children; d++ )
                {
                    /* Create symbol and insert in global nametab */
                    symbol = malloc ( sizeof(symbol_t) );
                    *symbol = (symbol_t) {
                        .type = SYM_GLOBAL_VAR,
                        .name = namelist->children[d]->data,
                        .node = NULL,
                        .seq = 0,
                        .nparms = 0,
                        .locals = NULL
                    };
                    insert_symbol ( global_names, symbol );
                }
                break;
        }
    }
}


/* Create a new table of names and push it on stack,
 * count the present nesting depth, and resize stack if it is full
 */
static void
push_scope ( void )
{
    if ( scopes == NULL )
        scopes = malloc ( n_scopes * sizeof(tlhash_t *) );
    tlhash_t *new_scope = malloc ( sizeof(tlhash_t) );
    tlhash_init ( new_scope, 32 );
    scopes[scope_depth] = new_scope;

    scope_depth += 1;
    if ( scope_depth >= n_scopes )
    {
        n_scopes *= 2;
        scopes = realloc ( scopes, n_scopes*sizeof(tlhash_t **) );
    }

}


/* Lookup a name (from the top down) on the local symbol stack */
static symbol_t *
lookup_local ( char *name )
{
    symbol_t *result = NULL;
    size_t depth = scope_depth;
    while ( result == NULL && depth > 0 )
    {
        depth -= 1;
        tlhash_lookup ( scopes[depth], name, strlen(name), (void **)&result );
    }
    return result;
}


/* Remove and finalize the table for a local scope */
static void
pop_scope ( void )
{
    scope_depth -= 1;
    tlhash_finalize ( scopes[scope_depth] );
    free ( scopes[scope_depth] );
    scopes[scope_depth] = NULL;
}


/* Look up and handle local definitions and name uses */
void
bind_names ( symbol_t *function, node_t *root )
{
    if ( root == NULL )
        return;
    else switch ( root->type )
    {
        node_t *namelist;
        symbol_t *entry;

        /* Blocks initiate a new nested scope, recur into its statements */
        case BLOCK:
            push_scope();
            for ( size_t c=0; c<root->n_children; c++ )
                bind_names ( function, root->children[c] );
            pop_scope();
            break;

        /* Declarations enter string-indexed symbols on the stack,
         * and retain pointers in the function's table as well
         */
        case DECLARATION:
            namelist = root->children[0];
            for ( uint64_t d=0; d<namelist->n_children; d++ )
            {
                node_t *varname = namelist->children[d];
                size_t local_num =
                    tlhash_size(function->locals) - function->nparms;
                symbol_t *symbol = malloc ( sizeof(symbol_t) );
                *symbol = (symbol_t) {
                    .type = SYM_LOCAL_VAR,
                    .name = varname->data,
                    .node = NULL,
                    .seq = local_num,
                    .nparms = 0,
                    .locals = NULL
                };
                /* Index function's table on index number instead of string,
                 * to avoid name clashes. This is to retain pointers to the
                 * symbols after all names are bound, the local scope tables
                 * disappear as the scopes end.
                 */
                tlhash_insert (
                    function->locals, &local_num, sizeof(size_t), symbol
                );
                insert_symbol ( scopes[scope_depth-1], symbol );
            }
            break;

        /* Since declarations don't recur, finding identifiers must correspond
         * to uses in expressions. Look up the symbol, and attach it to
         * the tree node.
         */
        case IDENTIFIER_DATA:
            /* Is it a local variable? */
            entry = lookup_local ( root->data );

            /* Otherwise, is it a parameter? */
            if ( entry == NULL )
                tlhash_lookup (
                    function->locals, root->data,
                    strlen(root->data), (void**)&entry
                );

            /* Otherwise, is it a global name? */
            if ( entry == NULL )
                tlhash_lookup (
                    global_names,root->data,strlen(root->data),(void**)&entry
                );

            /* Name wasn't found anywhere, crash and burn */
            if ( entry == NULL )
            {
                fprintf ( stderr, "Identifier '%s' does not exist in scope\n",
                    (char *)root->data
                );
                exit ( EXIT_FAILURE );
            }

            /* If we got this far, the symbol has been found, so attach it */
            root->entry = entry;
            break;

        /* Strings: put them in the string table */
        case STRING_DATA:
            add_string ( root );
            break;

        /* If this was not a node otherwise handled, recur into its children */
        default:
            for ( size_t c=0; c<root->n_children; c++ )
                bind_names ( function, root->children[c] );
            break;
    }
}


/* Remove everything set up above:
 * - the list of strings
 * - the local symbols in all function entries
 * - the global symbol table
 */
void
destroy_symtab ( void )
{
    for ( size_t i=0; i<stringc; i++ )
        free ( string_list[i] );
    free ( string_list );

    size_t n_globals = tlhash_size ( global_names );
    symbol_t *global_list[n_globals];
    tlhash_values ( global_names, (void **)&global_list );
    for ( size_t g=0; g<n_globals; g++ )
    {
        symbol_t *glob = global_list[g];
        if ( glob->locals != NULL )
        {
            size_t n_locals = tlhash_size ( glob->locals );
            symbol_t *locals[n_locals];
            tlhash_values ( glob->locals, (void **)&locals );
            for ( size_t l=0; l<n_locals; l++ )
                free ( locals[l] );
            tlhash_finalize ( glob->locals );
            free ( glob->locals );
        }
        free ( glob );
    }
    tlhash_finalize ( global_names );
    free ( global_names );
    free ( scopes );
}
