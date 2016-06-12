%{
#include <vslc.h>
%}
%option noyywrap
%option array
%option yylineno

WHITESPACE [\ \t\v\r\n]
COMMENT \/\/[^\n]+
QUOTED \"([^\"\n]|\\\")*\"
%%
{WHITESPACE}+           { /* Eliminate whitespace */ }
{COMMENT}               { /* Eliminate comments */ }
FUNC                    { return FUNC; }
PRINT                   { return PRINT; }
RETURN                  { return RETURN; }
CONTINUE                { return CONTINUE; }
IF                      { return IF; }
THEN                    { return THEN; }
ELSE                    { return ELSE; }
WHILE                   { return WHILE; }
DO                      { return DO; }
BEGIN                   { return OPENBLOCK; }
END                     { return CLOSEBLOCK; }
VAR                     { return VAR; }
[0-9]+                  { return NUMBER; }
[A-Za-z_][0-9A-Za-z_]*  { return IDENTIFIER; }
{QUOTED}                { return STRING; }
.                       { return yytext[0]; }
%%
