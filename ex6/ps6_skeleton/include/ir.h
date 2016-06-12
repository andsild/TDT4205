#ifndef IR_H
#define IR_H

typedef struct n {
    node_index_t type;
    void *data;
    struct s *entry;
    uint64_t n_children;
    struct n **children;
} node_t;

void node_init (
    node_t *n, node_index_t type, void *data, uint64_t n_children, ...
);
void node_print ( node_t *root, int nesting );
void node_finalize ( node_t *discard );

void destroy_subtree ( node_t *discard );
void simplify_tree ( node_t **simplified, node_t *root );


typedef enum {
    SYM_GLOBAL_VAR, SYM_FUNCTION, SYM_PARAMETER, SYM_LOCAL_VAR
} symtype_t;

typedef struct s {
    char *name;
    symtype_t type;
    node_t *node;
    size_t seq;
    size_t nparms;
    tlhash_t *locals;
} symbol_t;

extern tlhash_t *global_names;
extern symbol_t **global_list;
extern char **string_list;
extern size_t stringc;

void find_globals ( void );
void bind_names ( symbol_t *function, node_t *root );
void print_globals ( void );
void destroy_symtab ( void );
#endif
