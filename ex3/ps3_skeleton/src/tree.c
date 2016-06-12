/// Ops, there is no memory management in this code. 
/// I have it as a late priority
#include <vslc.h>

void
node_print ( node_t *root, int nesting )
{
    if ( root != NULL )
    {
        printf ( "%*c%s", nesting, ' ', node_string[root->type] );
        if ( root->type == IDENTIFIER_DATA ||
             root->type == STRING_DATA ||
             root->type == RELATION ||
             root->type == EXPRESSION ) 
            printf ( "(%s)", (char *) root->data );
        else if ( root->type == NUMBER_DATA )
            printf ( "(%ld)", *((int64_t *)root->data) );
        putchar ( '\n' );
        for ( int64_t i=0; i<root->n_children; i++ )
            node_print ( root->children[i], nesting+1 );
    }
    else
        printf ( "%*c%p\n", nesting, ' ', root );
}

void
node_init (node_t *nd, node_index_t type, void *data, uint64_t n_children, ...)
{
    va_list child_list;
    *nd = (node_t) {
        .type = type,
        .data = data,
        .entry = NULL,
        .n_children = n_children,
        .children = (node_t **) malloc ( n_children * sizeof(node_t *) )
    };
    va_start ( child_list, n_children );
    for ( uint64_t i=0; i<n_children; i++ )
        nd->children[i] = va_arg ( child_list, node_t * );
    va_end ( child_list );
}

void
node_finalize ( node_t *discard )
{
    if ( discard != NULL )
    {
        free ( discard->data );
        free ( discard->children );
        free ( discard );
    }
}

void
destroy_subtree ( node_t *discard )
{
    if ( discard != NULL )
    {
        for ( uint64_t i=0; i<discard->n_children; i++ )
            destroy_subtree ( discard->children[i] );
        node_finalize ( discard );
    }
}

bool nodeIsExcessive(node_t *parentNode, node_t *childNode)
{
    if(
      // Global or statement node
            (childNode->type == GLOBAL ??!??! childNode->type == STATEMENT
             ??!??! childNode->type == PRINT_ITEM 
             ??!??! childNode->type == PARAMETER_LIST
             ??!??! childNode->type == ARGUMENT_LIST
            ))
        return true;

    if(
            // No data, and is type expression
            (childNode->data == NULL && childNode->type == EXPRESSION)
        &&
            childNode->n_children == 1)
        return true;

    if(     // parent and child are the same
            (parentNode->type == childNode->type)  
      &&     // Has only 1 child
            (parentNode->n_children == 1 ??!??! childNode->n_children == 1)
      &&    // 
            (parentNode->data == NULL)
    )
        return true;
    return false;
}

void processArithmetic(node_t **currentNode) {
    if((*currentNode)->type != EXPRESSION || (*currentNode)->data == NULL
        || (*currentNode)->children[0]->type != NUMBER_DATA)
        return;

    const char *data = (const char*)(*currentNode)->data;
    int64_t *number_data = (int64_t*)(*currentNode)->children[0]->data;

    if((*currentNode)->n_children == 1 
    && strcmp(data, (const char*)"-") == 0)
    {
        *number_data = -(*number_data);
        node_t *copy = malloc(sizeof(node_t));
        *copy = (node_t) {
            .type = NUMBER_DATA,
            .data = number_data,
            .entry = NULL,
            .n_children = 0,
            .children = (node_t **) malloc ( 0 * sizeof(node_t *) )
        };
        *currentNode = copy;
    }
    if((*currentNode)->n_children == 2
    && (*currentNode)->children[1]->type == NUMBER_DATA)
    {
        int64_t *node1_number_data = (int64_t*)(*currentNode)->children[0]->data;
        int64_t *node2_number_data = (int64_t*)(*currentNode)->children[1]->data;
        int64_t *result = malloc ( sizeof(int64_t) );
        if(strcmp(data, (const char*)"-") == 0)
            *result = ((*node1_number_data) - (*node2_number_data));
        else if (strcmp(data, (const char*)"*") == 0)
            *result = ((*node1_number_data) * (*node2_number_data));
        else if (strcmp(data, (const char*)"/") == 0)
            *result = ((*node1_number_data) / (*node2_number_data));
        else if (strcmp(data, (const char*)"+") == 0)
            *result = ((*node1_number_data) + (*node2_number_data));

        node_t *copy = malloc(sizeof(node_t));
        *copy = (node_t) {
            .type = NUMBER_DATA,
            .data = result,
            .entry = NULL,
            .n_children = 0,
            .children = (node_t **) malloc ( (*currentNode)->n_children * sizeof(node_t *) )
        };
        *currentNode = copy;
    }
}

void shuffleUpParent(node_t **currentNode, node_t **childNode, int childNodeIndex) {
    int64_t numberOfChildren = 
        (*childNode)->n_children + (*currentNode)->n_children - 1;
    node_t *copy = malloc(sizeof(node_t));
    *copy = (node_t) {
        .type = (*currentNode)->type,
            .data = NULL,
            .entry = NULL,
            .n_children = numberOfChildren,
            .children = (node_t **) malloc ( numberOfChildren * sizeof(node_t *) )
    };
    int copyNodeIndex = 0;
    // Copy over all from the original node to the copy
    for (int childNodeIndex = 0 ;
            childNodeIndex < (*childNode)->n_children;
            childNodeIndex++ )
        copy->children[copyNodeIndex++] = (*childNode)->children[childNodeIndex];
    // Copy over from the sub/childnode to the copynode
    for ( int j = 0; j < (*currentNode)->n_children; j++ )
        if( j != childNodeIndex)
            copy->children[copyNodeIndex++] =
                (*currentNode)->children[j];
    *currentNode = copy;
}


void recurse(node_t **currentNode, int nesting)
{
    for ( int64_t i=0; i < (*currentNode)->n_children; i++ ) 
    {
        if((*currentNode)->children[i] == NULL)
            continue;
        node_t **childNode = &(*currentNode)->children[i];

        recurse(childNode, nesting+1 );

        if(((*currentNode)->type == (*childNode)->type && (*currentNode)->data == NULL)
        ||
        ((*currentNode)->type == PRINT_STATEMENT 
        && (*childNode)->type == PRINT_LIST))
            shuffleUpParent(currentNode, childNode, i);
        else if(nodeIsExcessive(*currentNode, *childNode) == true)
            (*currentNode)->children[i] = (*childNode)->children[0];
    }

    processArithmetic(currentNode);
}


void
simplify_tree ( node_t **simplified, node_t *root )
{
    *simplified = root;
    node_t *ptr = root;

    recurse (simplified, 0);

    // simplify: 
    // node_t **new_array = realloc ( old_array, (n+1)*sizeof(node_t*) );
}

