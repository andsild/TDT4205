#include "vslc.h"

#define MIN(a,b) (((a)<(b)) ? (a):(b))

#define LBL(name) puts ( #name":" )
#define ASM0(op) puts ( "\t"#op )
#define ASM1(op,a) puts ( "\t"#op"\t"#a )
#define ASM2(op,a,b) puts ( "\t"#op"\t"#a", "#b )

static const char *record[6] = {
    "%rdi", "%rsi", "%rdx", "%rcx", "%r8", "%r9"
};

static void generate_node ( node_t *node, size_t stackSpace );

const int Offset = 8;
int LabelCount = 1;

static void
generate_stringtable ( void )
{
    puts ( ".section .rodata" );
    puts ( "intout: .string \"\%ld \"" );
    puts ( "strout: .string \"\%s \"" );
    puts ( "errout: .string \"Wrong number of arguments\"" );
    for ( size_t s=0; s<stringc; s++ )
        printf ( "STR%zu: .string %s\n", s, string_list[s] );
}


static void
generate_global_variables ( void )
{
    puts ( ".section .data" );
    size_t nsyms = tlhash_size ( global_names );
    symbol_t *syms[nsyms];
    tlhash_values ( global_names, (void **)&syms );
    for ( size_t n=0; n<nsyms; n++ )
    {
        if ( syms[n]->type == SYM_GLOBAL_VAR )
            printf ( "_%s: .zero 8\n", syms[n]->name );
    }
}


static void
generate_main ( symbol_t *first )
{
    puts ( ".globl main" );
    puts ( ".section .text" );
    puts ( "main:" );
    puts ( "\tpushq %rbp" );
    puts ( "\tmovq %rsp, %rbp" );

    puts ( "\tsubq $1, %rdi" );
    printf ( "\tcmpq\t$%zu,%%rdi\n", first->nparms );
    puts ( "\tjne ABORT" );
    puts ( "\tcmpq $0, %rdi" );
    puts ( "\tjz SKIP_ARGS" );

    puts ( "\tmovq %rdi, %rcx" );
    printf ( "\taddq $%zu, %%rsi\n", 8*first->nparms );
    puts ( "PARSE_ARGV:" );
    puts ( "\tpushq %rcx" );
    puts ( "\tpushq %rsi" );

    puts ( "\tmovq (%rsi), %rdi" );
    puts ( "\tmovq $0, %rsi" );
    puts ( "\tmovq $10, %rdx" );
    puts ( "\tcall strtol" );

    /*  Now a new argument is an integer in rax */
    puts ( "\tpopq %rsi" );
    puts ( "\tpopq %rcx" );
    puts ( "\tpushq %rax" );
    puts ( "\tsubq $8, %rsi" );
    puts ( "\tloop PARSE_ARGV" );

    /* Now the arguments are in order on stack */
    for ( int arg=0; arg<MIN(6,first->nparms); arg++ )
        printf ( "\tpopq\t%s\n", record[arg] );

    puts ( "SKIP_ARGS:" );
    printf ( "\tcall\t_%s\n", first->name );
    puts ( "\tjmp END" );
    puts ( "ABORT:" );
    puts ( "\tmovq $errout, %rdi" );
    puts ( "\tcall puts" );

    puts ( "END:" );
    puts ( "\tmovq %rax, %rdi" );
    puts ( "\tcall exit" );
}


static int generate_identifier ( node_t *ident )
{
    symbol_t *symbol = ident->entry;
    switch ( symbol->type )
    {
        case SYM_GLOBAL_VAR:
            printf ( "_%s", symbol->name );
            break;
        case SYM_PARAMETER:
            if ( symbol->seq > 5 )
                    printf ( "%ld(%%rbp)", 8+8*(symbol->seq-5) );
            else
                printf ( "%ld(%%rbp)", -8*(symbol->seq+1) );
            break;
        case SYM_LOCAL_VAR:
            if ( symbol->seq > 5 ) // TODO: handle more than 5 entries
                    printf ( "%ld(%%rbp)", 8+8*(symbol->seq-5) );
            else
                printf ( "%ld(%%rbp)", (-8*(symbol->seq+1)));
            break;
    }
}

// void enumerate_sequence_number(node_t *node, size_t sequenceNumber)
// {
//     if(node->type == VARIABLE_LIST)
//         for(size_t i = 0; i < node->n_children; i++)
//             if(node->entry != NULL)
//                 node->entry->seq = sequenceNumber++;
//     else
//         for(size_t i = 0; i < node->n_children; i++)
//             enumerate_sequence_number(node->children[i], sequenceNumber);
// }


static void generate_expression ( node_t *expr )
{
    if ( expr->type == IDENTIFIER_DATA )
    {
        printf ( "\tmovq\t" );
        generate_identifier ( expr );
        printf ( ", %%rax\n" );
    }
    else if ( expr->type == NUMBER_DATA )
    {
        printf ( "\tmovq\t$%ld, %%rax\n", *(int64_t *)expr->data );
    }
    else if ( expr->n_children == 1 )
    {
        generate_expression ( expr->children[0] );
        ASM1 ( negq, %rax );
    }
    else if ( expr->n_children == 2 )
    {
        if ( expr->data != NULL )
        {
            switch ( *((char *)expr->data) )
            {
                case '+':
                    generate_expression ( expr->children[0] );
                    ASM1 ( pushq, %rax );
                    generate_expression ( expr->children[1] );
                    ASM2 ( addq, %rax, (%rsp) );
                    ASM1 ( popq, %rax );
                    break;
                case '-':
                    generate_expression ( expr->children[0] );
                    ASM1 ( pushq, %rax );
                    generate_expression ( expr->children[1] );
                    ASM2 ( subq, %rax, (%rsp) );
                    ASM1 ( popq, %rax );
                    break;
                case '*':
                    ASM1 ( pushq, %rdx );
                    generate_expression ( expr->children[1] );
                    ASM1 ( pushq, %rax );
                    generate_expression ( expr->children[0] );
                    ASM0 ( cqo );
                    ASM1 ( imulq, (%rsp) );
                    ASM1 ( popq, %rdx );
                    ASM1 ( popq, %rdx );
                    break;
                case '/':
                    ASM1 ( pushq, %rdx );
                    generate_expression ( expr->children[1] );
                    ASM1 ( pushq, %rax );
                    generate_expression ( expr->children[0] );
                    ASM0 ( cqo );
                    ASM1 ( idivq, (%rsp) );
                    ASM1 ( popq, %rdx );
                    ASM1 ( popq, %rdx );
                    break;
            }
        }
        else // function call
        {
            char *functionName = expr->children[0]->data;

            int localVariableIndex = 1;
            if(expr->children[1] != NULL)
            {
                for(size_t iterator = 0; iterator < MIN(6,expr->children[1]->n_children); iterator++)
                {
                    node_t *child = expr->children[1]->children[iterator];
                    const char *dest =  record[iterator];
                    if(child->type == NUMBER_DATA)
                        printf("\tmovq\t$%ld,%s\n", *((int64_t *)child->data), dest);
                    else if(child->type == IDENTIFIER_DATA)
                    {
                        char *variableName = child->data;
                        char *localSource = malloc(sizeof(char*));
                        sprintf(localSource, "-%d(%%rbp)", Offset*(child->entry->seq+1));
                        printf("\tmovq\t%s, %s\n", localSource, dest);
                        localVariableIndex++;
                    }
                    else if(child->type == EXPRESSION)
                    {
                        generate_expression(child);
                        printf("\tmovq\t%%rax, %s\n", dest);
                    }
                }
            }
            printf("\tcall\t_%s\n", functionName);
        }
    }
}


static void generate_assignment_statement ( node_t *statement, size_t stackSpace )
{
    generate_expression ( statement->children[1] );
    printf ( "\tmovq\t%%rax, " );
    generate_identifier ( statement->children[0] );
    printf ( "\n" );
}


static void generate_print_statement ( node_t *statement )
{
    for ( size_t i=0; i<statement->n_children; i++ )
    {
        printf("\n");
        node_t *item = statement->children[i];
        switch ( item->type )
        {
            case STRING_DATA:
                printf ( "\tmovq\t$STR%zu, %%rsi\n", *((size_t *)item->data) );
                ASM2 ( movq, $strout, %rdi );
                ASM1 ( call, printf );
                break;
            case NUMBER_DATA:
                printf ("\tmovq\t$%ld, %%rsi\n", *((int64_t *)item->data) );
                ASM2 ( movq, $intout, %rdi );
                ASM1 ( call, printf );
                break;
            case IDENTIFIER_DATA:
                printf ( "\tmovq\t" );
                generate_identifier ( item );
                printf ( ", %%rsi\n" );
                ASM2 ( movq, $intout, %rdi );
                ASM1 ( call, printf );
                break;
            case EXPRESSION:
                generate_expression ( item );
                ASM2 ( movq, %rax, %rsi );
                ASM2 ( movq, $intout, %rdi );
                ASM1 ( call, printf );
                break;
        }
    }
    ASM2 ( movq, $'\n', %rdi );
    ASM1 ( call, putchar );
    printf("\n");
}

static void generate_while_statement(node_t *statement)
{
    int inner_label_count = LabelCount; // make a predictable counter for continue statements
    LabelCount += 2;
    printf("\tjmp\t.L%d\n", (inner_label_count)+1);
    printf(".L%d:\n", inner_label_count);
    inner_label_count++;
    for(size_t i = 1; i < statement->n_children; i++)
        generate_node(statement->children[i], 0);
    printf(".L%d:\n", inner_label_count);

    const char *operation;
    if(strcmp(statement->children[0]->data, "<") == 0)
        operation = "jg";
    else if(strcmp(statement->children[0]->data, ">") == 0)
        operation = "jle";
    else
        operation = "jne";

    node_t *lhs = statement->children[0]->children[0];
    node_t *rhs = statement->children[0]->children[1];
    if(lhs->type != NUMBER_DATA && rhs->type == NUMBER_DATA)
    {
        node_t *swap = lhs;
        lhs = rhs;
        rhs = swap;
        if(strcmp(operation, "jg") == 0)
            operation = "jle";
        else if(strcmp(operation, "jle") == 0)
            operation = "jg";
        else
            operation = "je";
    }

    char *strLhs = malloc(sizeof(char*));
    char *strRhs = malloc(sizeof(char *));
    char *midSection = "";

    if(lhs->type == NUMBER_DATA && rhs->type == NUMBER_DATA)
    {
        int64_t numLhs = *((int64_t *)lhs->data);
        int64_t numRhs = *((int64_t *)rhs->data);

        if(strcmp(statement->children[0]->data, "<") == 0 && numLhs < numRhs)
            generate_node(statement->children[1], 0);
        else if(strcmp(statement->children[0]->data, ">") == 0 && numLhs > numRhs)
            generate_node(statement->children[1], 0);
        else if(strcmp(statement->children[0]->data, "=") == 0 && numLhs == numRhs)
            generate_node(statement->children[1], 0);

        return;
    }


    if(lhs->type == EXPRESSION)
    {
        generate_expression( lhs );
        strLhs = "%rax";
    }
    else {
        if(lhs->type == NUMBER_DATA)
            sprintf(strLhs, "$%d", *((int64_t *)lhs->data));
        if(lhs->type == IDENTIFIER_DATA)
            sprintf(strLhs, "%ld(%%rbp)", -8*(lhs->entry->seq+1) );
    }

    if(lhs->type == EXPRESSION && rhs->type == EXPRESSION)
    {
        midSection = "\tmovq\t%rax, %rbx\n";
        strLhs = "%rbx";
    }

    if(rhs->type == EXPRESSION)
    {
        printf("%s", midSection);
        generate_expression( rhs );
        strRhs = "%rax";
    }
    else {
        if(rhs->type == NUMBER_DATA)
            sprintf(strRhs, "$%d", *((int64_t *)rhs->data));
        if(rhs->type == IDENTIFIER_DATA)
            sprintf(strRhs, "%ld(%%rbp)", -8*(rhs->entry->seq+1) );
    }

    if(lhs->type == IDENTIFIER_DATA && rhs->type == IDENTIFIER_DATA)
    {
        printf("\tmovq %s, %%rax\n", strRhs);
        strRhs = "%rax";
    }

    printf("\tcmpq\t%s, %s\n", strLhs, strRhs); 

    printf("\t%s\t.L%d\n", operation, inner_label_count-1); 
}

static void generate_if_statement(node_t *statement)
{
    // Q: what happens when you code and you are stressed?
    // A: the monster of if statements

    const char *operation;
    if(strcmp(statement->children[0]->data, "<") == 0)
        operation = "jge";
    else if(strcmp(statement->children[0]->data, ">") == 0)
        operation = "jle";
    else
        operation = "jne";

    node_t *lhs = statement->children[0]->children[0];
    node_t *rhs = statement->children[0]->children[1];
    if(lhs->type != NUMBER_DATA && rhs->type == NUMBER_DATA)
    {
        node_t *swap = lhs;
        lhs = rhs;
        rhs = swap;
    }

    char *strLhs = malloc(sizeof(char*));
    char *strRhs = malloc(sizeof(char *));
    char *midSection = "";

    if(lhs->type == NUMBER_DATA && rhs->type == NUMBER_DATA)
    {
        int64_t numLhs = *((int64_t *)lhs->data);
        int64_t numRhs = *((int64_t *)rhs->data);

        if(strcmp(statement->children[0]->data, "<") == 0 && numLhs < numRhs)
            generate_node(statement->children[1], 0);
        else if(strcmp(statement->children[0]->data, ">") == 0 && numLhs > numRhs)
            generate_node(statement->children[1], 0);
        else if(strcmp(statement->children[0]->data, "=") == 0 && numLhs == numRhs)
            generate_node(statement->children[1], 0);

        return;
    }


    if(lhs->type == EXPRESSION)
    {
        generate_expression( lhs );
        strLhs = "%rax";
    }
    else {
        if(lhs->type == NUMBER_DATA)
            sprintf(strLhs, "$%d", *((int64_t *)lhs->data));
        if(lhs->type == IDENTIFIER_DATA)
            sprintf(strLhs, "%ld(%%rbp)", -8*(lhs->entry->seq+1) );
    }

    if(lhs->type == EXPRESSION && rhs->type == EXPRESSION)
    {
        midSection = "\tmovq\t%rax, %rbx\n";
        strLhs = "%rbx";
    }

    if(rhs->type == EXPRESSION)
    {
        printf("%s", midSection);
        generate_expression( rhs );
        strRhs = "%rax";
    }
    else {
        if(rhs->type == NUMBER_DATA)
            sprintf(strRhs, "$%d", *((int64_t *)rhs->data));
        if(rhs->type == IDENTIFIER_DATA)
            sprintf(strRhs, "%ld(%%rbp)", -8*(rhs->entry->seq+1) );
    }

    if(lhs->type == IDENTIFIER_DATA && rhs->type == IDENTIFIER_DATA)
    {
        printf("\tmovq %s, %%rax\n", strRhs);
        strRhs = "%rax";
    }

    if(statement->n_children == 3)
    {
        printf("\tcmpq\t%s, %s\n", strLhs, strRhs); 

        printf("\t%s\t.L%d\n", operation, LabelCount); 
        generate_node(statement->children[1], 0);
        printf("\tjmp\t.L%d\n", LabelCount+1); 

        printf(".L%d:\n", LabelCount++);
        generate_node(statement->children[2], 0);
        // printf("\tjmp\t.L%d\n", ++LabelCount);

        printf(".L%d:\n", LabelCount++); // rest of program
    }
    else if(statement->n_children == 2) // no else statement
    {
        printf("\tcmpq\t%s, %s\n", strLhs, strRhs); 

        printf("\t%s\t.L%d\n", operation, LabelCount); 
        generate_node(statement->children[1], 0);

        printf(".L%d:\n", LabelCount++); 
    }
}


static void generate_node ( node_t *node, size_t stackSpace )
{
    switch (node->type)
    {
        case PRINT_STATEMENT:
            generate_print_statement ( node );
            break;
        case ASSIGNMENT_STATEMENT:
            generate_assignment_statement ( node, stackSpace );
            break;
        case RETURN_STATEMENT:
            generate_expression ( node->children[0]  );
            ASM0 ( leave );
            ASM0 ( ret );
            break;
        case IF_STATEMENT:
            generate_if_statement(node);
            break;
        case WHILE_STATEMENT:
            generate_while_statement(node);
            break;
        case NULL_STATEMENT:
            printf("\tjmp\t.L%d\n", LabelCount-1);
        default:
            for ( size_t i=0; i<(node)->n_children; i++ ) \
                generate_node ( (node)->children[i], stackSpace );    \
            break;
    }
}

size_t findNumberOfVariablesInContext(node_t *node)
{
    size_t accumulator = 0;
    if(node->type == VARIABLE_LIST)
        for(size_t i = 0; i < node->n_children; i++)
            accumulator++;
    else
        for(size_t i = 0; i < node->n_children; i++)
    {
        accumulator += findNumberOfVariablesInContext(node->children[i]);
    }

    return accumulator;
}


static void
generate_function ( symbol_t *function )
{
    printf ( "_%s:\n", function->name );
    ASM1 ( pushq, %rbp );
    ASM2 ( movq, %rsp, %rbp );

    // Save arguments in local stack frame
    // for ( size_t arg=1; arg<=MIN(6,function->nparms); arg++ )
    //         printf ( "\tpushq\t%s\n", record[arg-1] );

    size_t localsize = tlhash_size( function->locals );
    symbol_t *locals[localsize];
    tlhash_values(function->locals, (void **)locals );
    int localVariableIndex = 1;

    tlhash_t *variableTable;
    variableTable = malloc(sizeof(tlhash_t*));
    tlhash_init(variableTable, 1);

    size_t numberOfVariablesInContext = findNumberOfVariablesInContext(function->node);
    size_t numberOfParameters = 0;


    for ( size_t i=0; i<localsize; i++ )
        if(locals[i]->type == SYM_PARAMETER )
            numberOfParameters++;
    numberOfVariablesInContext += numberOfParameters;
    size_t stackSpace = (numberOfVariablesInContext & 1) == 0 ? numberOfVariablesInContext * 8 : (numberOfVariablesInContext*8) + 8;

    printf("\tsubq\t$%d, %%rsp\n", stackSpace);


    for ( size_t i=0; i<MIN(6,localsize); i++ )
    {
        if(locals[i]->type == SYM_PARAMETER )
        {
            // locals[i]->seq = localVariableIndex-1; 
            char *registerName = strdup(record[localVariableIndex-1]);
            char *variableName = locals[i]->name;
            char *localDestination = malloc(sizeof(char*));
            sprintf(localDestination, "-%d(%%rbp)", Offset*(localVariableIndex));
       
            tlhash_insert(variableTable, variableName, strlen(variableName), localDestination);
            printf("\tmovq\t%s, %s\n", registerName, localDestination);
            localVariableIndex++;
        }
    }
    for ( size_t i=0; i<MIN(6,localsize); i++ )
        if(locals[i]->type == SYM_LOCAL_VAR)
            locals[i]->seq = (localVariableIndex++ - 1); 


    //TODO: nasty > 6 parameters

    generate_node ( function->node, stackSpace );
}


void
generate_program ( void )
{
    size_t n_globals = tlhash_size(global_names);
    symbol_t *global_list[n_globals];
    tlhash_values ( global_names, (void **)&global_list );

    symbol_t *first_function;
    for ( size_t i=0; i<tlhash_size(global_names); i++ )
        if ( global_list[i]->type == SYM_FUNCTION && global_list[i]->seq == 0 )
        {
            first_function = global_list[i];
            break;
        }

    generate_stringtable();
    generate_global_variables();
    generate_main ( first_function );
    for ( size_t i=0; i<tlhash_size(global_names); i++ )
        if ( global_list[i]->type == SYM_FUNCTION )
            generate_function ( global_list[i] );
}


