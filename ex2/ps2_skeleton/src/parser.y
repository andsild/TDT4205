%{
#include <vslc.h>
#include <stdlib.h>
#include <stdint.h>
#define INT2VOIDP(i) (void*)(uintptr_t)(i)

node_t* createNode(node_index_t type, uint64_t numberOfChildren, ...)
{
    node_t* retNode = malloc(sizeof(node_t));
    va_list child_list;
    va_start(child_list, numberOfChildren);
    node_init(retNode, type, NULL, numberOfChildren, child_list );
    //printf("%d:\t", retNode->n_children);
    //printf("Returning create node for type %d:%s\n", type, node_string[retNode->type]);
    va_end(child_list);
    return retNode;
}

node_t* createNodeLabeled(node_index_t type, uint64_t numberOfChildren, void *label,
...)
{
    node_t* retNode = malloc(sizeof(node_t));
    va_list child_list;
    va_start(child_list, label);
    node_init(retNode, type, label, numberOfChildren, child_list);
    va_end(child_list);
    return retNode;
}
%}
%left '+' '-'
%left '*' '/'
%left '.'
%nonassoc UMINUS

%token FUNC PRINT RETURN CONTINUE IF THEN ELSE WHILE DO OPENBLOCK CLOSEBLOCK 
%token VAR NUMBER IDENTIFIER STRING ASSIGN
%token START 270 
%token FINISH 

%define parse.error verbose

%%
program: global_list { 
        root  = createNode(PROGRAM, 1, $1);
       }

global_list: global { $$ = createNode(GLOBAL_LIST, 1, $1); }
    | global_list global { $$ = createNode(GLOBAL_LIST, 2, $1, $2); }
    ;
global :
        function {
        //root = (node_t *) malloc ( sizeof(node_t) );
        $$  = createNode(GLOBAL, 1, $1);
        //node_init ( root, PROGRAM, NULL, 0 );
      }
      | declaration { $$ = createNode(GLOBAL, 1, $1); }
   ;

statement_list:
    statement               { $$ = createNode(STATEMENT_LIST, 1, $1); }
    | statement_list statement { $$ = createNode(STATEMENT_LIST, 2, $1, $2); }
    ;
print_list:
    print_item  {  $$ = createNode(PRINT_LIST, 1, $1); }
    | print_list ',' print_item { $$ = createNode(PRINT_LIST, 2, $1, $3); }
    ;
expression_list:
    expression { $$ = createNode(EXPRESSION_LIST, 1, $1); }
    | expression_list ',' expression { $$ = createNode(EXPRESSION_LIST, 2, $1, $3); }
   ;
variable_list:
    identifier          { $$ = createNode(VARIABLE_LIST, 1, $1); }
    | variable_list ',' identifier { $$ = createNode(VARIABLE_LIST, 2, $1, $3); }
    ;

argument_list:
    expression_list { $$ = createNode(ARGUMENT_LIST, 1, $1);}
    | /* epsilon */ {  $$ = NULL;   }
    ;
parameter_list:
    variable_list { $$ = createNode(PARAMETER_LIST, 1, $1); }
    | /* epsilon */ { $$ = NULL; }
    ;
declaration_list:
    declaration  { $$ = createNode(DECLARATION_LIST, 1, $1); }
    | declaration_list declaration { $$ = createNode(DECLARATION_LIST, 2, $1, $2);   }
    ;

function: FUNC identifier '(' parameter_list ')' statement
        {
            $$ = createNode(FUNCTION, 3, $2, $4, $6);
        }
        ;

statement:
    assignment_statement { $$ = createNode(STATEMENT, 1, $1); }
    | return_statement { $$ = createNode(STATEMENT, 1, $1); }
    | if_statement { $$ = createNode(STATEMENT, 1, $1); }
    | print_statement { $$ = createNode(STATEMENT, 1, $1); }
    | null_statement { $$ = createNode(STATEMENT, 1, $1); }
    | while_statement { $$ = createNode(STATEMENT, 1, $1); }
    | block { $$ = createNode(STATEMENT, 1, $1); }
    ;

block:
     START declaration_list statement_list FINISH 
            { $$ = createNode(BLOCK, 2, $2, $3); }
     | START statement_list FINISH { $$ = createNode(BLOCK, 1, $2); }
    ;

assignment_statement:
    identifier ASSIGN expression { $$ = createNode(ASSIGNMENT_STATEMENT, 2, $1, $3); }
    ;
return_statement:
    RETURN expression            { $$ = createNode(RETURN_STATEMENT, 1, $2); }
    ;
print_statement:
    PRINT print_list { $$ = createNode(PRINT_STATEMENT, 1, $2); }
    ;
null_statement:
    CONTINUE    { $$ = NULL; }
    ;
if_statement:
    IF relation THEN statement      { $$ = createNode(IF_STATEMENT, 2, $2, $4); }
    | IF relation THEN statement ELSE statement      {  $$ = createNode(IF_STATEMENT, 3, $2, $4, $6);  }
    ;
while_statement:
    WHILE relation DO statement { $$ = createNode(WHILE_STATEMENT, 2, $2, $4); }
    ;

relation:
    expression '=' expression { $$ = createNode(RELATION, 2, $1, $3); }
    | expression '<' expression { $$ = createNode(RELATION, 2, $1, $3); }
    | expression '>' expression { $$ = createNode(RELATION, 2, $1, $3); }
    ;

expression:
      expression '+' expression          { $$ = createNodeLabeled(EXPRESSION, 2, strdup("+"), $1, $3); }
    | expression '-' expression          { $$ = createNodeLabeled(EXPRESSION, 2, strdup("-"), $1, $3); }
    | expression '/' expression          { $$ = createNodeLabeled(EXPRESSION, 2, strdup("/"), $1, $3); }
    | expression '*' expression          { $$ = createNodeLabeled(EXPRESSION, 2, strdup("*"), $1, $3); }
    | '-' expression %prec UMINUS        { $$ = createNodeLabeled(EXPRESSION, 1, strdup("-"), $2);  }
    | '(' expression ')'                 { $$ = createNode(EXPRESSION, 1, $2); }
    | number                               { $$ = createNode(EXPRESSION, 1, $1); }
    | identifier                         { $$ = createNode(EXPRESSION, 1, $1); }
    | identifier '(' argument_list ')'   { $$ = createNode(EXPRESSION, 2, $1, $3); }
    ;

declaration:
        VAR variable_list       { $$ = createNode(DECLARATION, 1, $2); }
        ;

print_item:
    expression { $$ = createNode(PRINT_ITEM, 1, $1); }
    | string { $$ = createNode(PRINT_ITEM, 1, $1); }
    ;
identifier:
    IDENTIFIER          {
                    $$ = createNodeLabeled(IDENTIFIER_DATA, 0, strdup(yytext));
                          }
    ;
number:
    NUMBER                              {
                        void *data = INT2VOIDP(atoi(yytext));
                        $$ = createNodeLabeled(NUMBER_DATA, 0, data ); 
                                        }
                                        ;
string:
      STRING { $$ = createNodeLabeled(STRING_DATA, 0, strdup(yytext)); }
      ;


%%

int
yyerror ( const char *error )
{
    fprintf ( stderr, "%s on line %d\n", error, yylineno );
    exit ( EXIT_FAILURE );
}
