#ifndef VSLC_H
#define VSLC_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

#include "tlhash.h"
#include "nodetypes.h"
#include "ir.h"
#include "generator.h"

#include "y.tab.h"
int yyerror ( const char *error );
extern int yylineno;
extern int yylex ( void );
extern char yytext[];

extern node_t *root;

#endif
