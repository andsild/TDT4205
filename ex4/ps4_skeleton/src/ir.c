#include <vslc.h>

tlhash_t *global_names;
char **string_list;
size_t n_string_list = 8, stringc = 0;

/**
  Given node_t, return symbol_t with sane defaults
*/
symbol_t  *mapNodeToSymbol(node_t *node, int counter, symtype_t type)
{
    char *name = strdup(node->data);

    symbol_t *symbol = malloc(sizeof(symbol_t));
    *symbol = (symbol_t ) {
        .name   = name,
            .type   = type,
            .seq    = counter,
            .nparms = 0,
            .node = node,
            .locals = NULL,
    };

    return symbol;
}

/**
  Insert a hash block to another hash table
  its key is the level of nesting
  */
void insertHashBlock(int nesting, tlhash_t *table)
{
    tlhash_t *hashTable;
    hashTable = malloc(sizeof(tlhash_t*));
    tlhash_init(hashTable, 1);

    char *nestingString = malloc(sizeof(char *));
    sprintf(nestingString, "%d", nesting);

    tlhash_insert(table, nestingString, strlen(nestingString), hashTable);
}

void removeHashBlock(int nesting, tlhash_t *root)
{
    char *nestingString ;
    sprintf(nestingString, "%d", nesting);

    tlhash_remove(root, nestingString, strlen(nestingString));
}

/**
  There are tlhash_t tables nested under each other, but each table can hold variables
  This method inserts a symbol_t to tlhash_t
*/
void insertSymbolToNestBlock(int nesting, tlhash_t *table, symbol_t *symbol)
{
    void *value;
    char *lookupKey = malloc(sizeof(char *));
    sprintf(lookupKey, "%d", nesting);
    if(tlhash_lookup(table, lookupKey,
                strlen(lookupKey), &value) == TLHASH_SUCCESS)
        tlhash_insert(value, strdup(symbol->name), strlen(symbol->name), symbol);
}

// Create a function symbol and insert it to global_names
void createFunctionSymbol(node_t *node, int counter)
{
    char *name = strdup(node->children[0]->data);
    int nparms = 0;
    symbol_t *functionSymbol = malloc(sizeof(symbol_t));
    tlhash_t *hashTable;
    hashTable = malloc(sizeof(tlhash_t));
    tlhash_init(hashTable, 1);

    if(node->children[1] != NULL) // are there parameter nodes?
    {
        node_t *blockNode = node->children[1];
        for (; nparms < blockNode->n_children; nparms++ )
        {
            symbol_t *parameterSymbol = mapNodeToSymbol(
                                            blockNode->children[nparms], nparms, SYM_PARAMETER);
            tlhash_insert(hashTable, parameterSymbol->name,
                          strlen(parameterSymbol->name), parameterSymbol);
        }
    }
    *functionSymbol = (symbol_t)
    {
        .name   = name,
        .type   = SYM_FUNCTION,
        .seq    = counter,
        .nparms = nparms,
        .node   = node,
        .locals = hashTable
    };
    if (tlhash_insert(global_names, name, strlen(name), functionSymbol)
            != TLHASH_SUCCESS )
        perror("WARNING WARNING\n");
}


void shallowTraverse(node_t *currentNode, int counter)
{
    if(currentNode->type == IDENTIFIER_DATA)
    {
        symbol_t *globalSymbol = mapNodeToSymbol(currentNode, 0, SYM_GLOBAL_VAR);
        if (tlhash_insert(global_names, globalSymbol->name, strlen(globalSymbol->name), globalSymbol)
                != TLHASH_SUCCESS )
            perror("WARNING WARNING\n");
    }
    else if(currentNode->type == FUNCTION)
        createFunctionSymbol(currentNode, counter);

    else 
        for ( size_t i=0; i < currentNode->n_children; i++ )
            if(currentNode->children[i] != NULL)
            {
                shallowTraverse(currentNode->children[i], counter);
                if(currentNode->children[i]->type == FUNCTION)
                    counter++;
            }
}

void processStringNode(node_t *node)
{
    string_list = realloc(string_list, (stringc+1)*sizeof(*string_list));
    string_list[stringc] = node->data;

    int *newData = malloc(sizeof(int));
    newData[0] = stringc;
    node->data = newData;

    stringc++;
}

// Process a symbol that is of type VARIABLE
int processVariableNode(node_t *node, tlhash_t *rootTable, tlhash_t *stack, int counter, bool nodeIsDeclaration, int nesting)
{
    void *value, *checkValue;
    char *lookupKey  = malloc(sizeof(char *));
    // get the current table, or check previous ones
    sprintf(lookupKey, "%d", nesting);
    // declaration = first time declaring variable. 
    //If not, we need to lookup the node and map it (we know we have seen before)
    if(!nodeIsDeclaration) 
    {
        // For each level of nesting
        for(int i = nesting; i > -1; i--)
        {
            sprintf(lookupKey, "%d", i);
            // lookup the tlhash_t
            if(tlhash_lookup(stack, lookupKey,
                        strlen(lookupKey), &value) == TLHASH_SUCCESS
            &&
            // lookup the symbol in the tlhash_t
                tlhash_lookup(value, node->data,
                            strlen(node->data), &value) == TLHASH_SUCCESS)
                {
                    node->entry = value;
                    break;
                }
        }
        if(node->entry == NULL  // if we didnt find a link in the previous for loop
        &&
        (
            tlhash_lookup(rootTable, node->data, 
                    strlen(node->data), &value) == TLHASH_SUCCESS
            ??!??!  // syntactic humour with C
            tlhash_lookup( global_names, node->data,
                    strlen(node->data), &value) == TLHASH_SUCCESS
         ))
            node->entry = value;
    }
    else // first time we are seeing the variable
    {
        symbol_t *localVariableSymbol = mapNodeToSymbol(node, counter++, SYM_LOCAL_VAR);
        tlhash_lookup(stack, lookupKey, strlen(lookupKey), &value);
        tlhash_insert(value, localVariableSymbol->name, strlen(localVariableSymbol->name), localVariableSymbol);
        sprintf(lookupKey, "%s%d", localVariableSymbol->name, counter);
        if(tlhash_lookup(rootTable, lookupKey, strlen(lookupKey), &checkValue) == TLHASH_SUCCESS)
            printf("ERROR\n");
        tlhash_insert(rootTable, lookupKey, strlen(lookupKey), localVariableSymbol);
        // printf("Inserting key '%s' at level %d\n", localVariableSymbol->name,  nesting);
    }
    free(lookupKey);
    return counter;
}


// DFS from bind_names, to traverse a function block
int traverseBlockDFS(node_t *currentNode, tlhash_t *rootTable, tlhash_t *stack, int counter,
        bool nodeIsDeclaration, int nesting)
{
    if(currentNode->type == BLOCK)
    {
        nesting++;
        // printf("Inserting table at nest level %d\n", nesting);
        insertHashBlock(nesting, stack);
        for ( size_t i=0; i<currentNode->n_children; i++ )
            if(currentNode->children[i] != NULL)
                counter = traverseBlockDFS(currentNode->children[i], rootTable, stack,  counter, currentNode->type == VARIABLE_LIST , nesting);

        removeHashBlock(nesting, stack);
        // printf("Removing table at nest level %d\n", nesting);
        nesting--;
    }
    else if(currentNode->type == IDENTIFIER_DATA)
        counter = processVariableNode(currentNode, rootTable, stack,  counter, nodeIsDeclaration, nesting);
    else if(currentNode->type == STRING_DATA)
        processStringNode(currentNode);
    else
        for ( size_t i=0; i<currentNode->n_children; i++ )
            if(currentNode->children[i] != NULL)
                counter = traverseBlockDFS(currentNode->children[i], rootTable, stack, counter, currentNode->type == VARIABLE_LIST , nesting);
    return counter;
}


void
bind_names ( symbol_t *function, node_t *root )
{
    tlhash_t *hashTable;
    hashTable = malloc(sizeof(tlhash_t*));
    tlhash_init(hashTable, 1);
    if(string_list == NULL)
        string_list = malloc(0); 
    for ( size_t i=0; i<root->n_children; i++ )
        if(root->children[i] != NULL && root->children[i]->type == BLOCK)
            traverseBlockDFS(root->children[i], function->locals, hashTable, 0, false, 0);

    //TODO: recursively
    free(hashTable);
}

void
find_globals ( void )
{
    global_names = malloc ( sizeof(tlhash_t) );
    tlhash_init ( global_names, 32 );
    shallowTraverse(root, 0);
}


void
destroy_symtab ( void )
{
    tlhash_finalize ( global_names );
    free ( global_names );
}


void
print_symbols ( void )
{
    printf ( "String table:\n" );
    for ( size_t s=0; s<stringc; s++ )
        printf  ( "%zu: %s\n", s, string_list[s] );
    printf ( "-- \n" );

    printf ( "Globals:\n" );
    size_t n_globals = tlhash_size(global_names);
    symbol_t *global_list[n_globals];
    tlhash_values ( global_names, (void **)&global_list );
    for ( size_t g=0; g<n_globals; g++ )
    {
        switch ( global_list[g]->type )
        {
            case SYM_FUNCTION:
                printf (
                    "%s: function %zu:\n",
                    global_list[g]->name, global_list[g]->seq
                );
                if ( global_list[g]->locals != NULL )
                {
                    size_t localsize = tlhash_size( global_list[g]->locals );
                    printf (
                        "\t%zu local variables, %zu are parameters:\n",
                        localsize, global_list[g]->nparms
                    );
                    symbol_t *locals[localsize];
                    tlhash_values(global_list[g]->locals, (void **)locals );
                    for ( size_t i=0; i<localsize; i++ )
                    {
                        printf ( "\t%s: ", locals[i]->name );
                        switch ( locals[i]->type )
                        {
                            case SYM_PARAMETER:
                                printf ( "parameter %zu\n", locals[i]->seq );
                                break;
                            case SYM_LOCAL_VAR:
                                printf ( "local var %zu\n", locals[i]->seq );
                                break;
                        }
                    }
                }
                break;
            case SYM_GLOBAL_VAR:
                printf ( "%s: global variable\n", global_list[g]->name );
                break;
        }
    }
    printf ( "-- \n" );
}


void
print_bindings ( node_t *root )
{
    if ( root == NULL )
        return;
    else if ( root->entry != NULL )
    {
        switch ( root->entry->type )
        {
            case SYM_GLOBAL_VAR: 
                printf ( "Linked global var '%s'\n", root->entry->name );
                break;
            case SYM_FUNCTION:
                printf ( "Linked function %zu ('%s')\n",
                    root->entry->seq, root->entry->name
                );
                break; 
            case SYM_PARAMETER:
                printf ( "Linked parameter %zu ('%s')\n",
                    root->entry->seq, root->entry->name
                );
                break;
            case SYM_LOCAL_VAR:
                printf ( "Linked local var %zu ('%s')\n",
                    root->entry->seq, root->entry->name
                );
                break;
        }
    } else if ( root->type == STRING_DATA ) {
        size_t string_index = *((size_t *)root->data);
        if ( string_index < stringc )
            printf ( "Linked string %zu\n", *((size_t *)root->data) );
        else
            printf ( "(Not an indexed string)\n" );
    }
    for ( size_t c=0; c<root->n_children; c++ )
        print_bindings ( root->children[c] );
}
