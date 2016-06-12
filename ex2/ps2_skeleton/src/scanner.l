%{
#include <vslc.h>
%}
%option noyywrap
%option array
%option yylineno

%x LINE_COMMENT
DIGIT [0-9]
LETTER [a-zA-Z_]
WHITESPACE [ \n\t\r\v]
%%
"//"                { BEGIN LINE_COMMENT; }
<LINE_COMMENT>.     { }
<LINE_COMMENT>"\n"  { BEGIN INITIAL; /* do nothing */ }
L?\"(\\.|[^\\"])*\" { return STRING; }
"PRINT"             { return PRINT; }
"END"               { return FINISH; }
"DO"                { return DO; }
"WHILE"             { return WHILE; }
"IF"                { return IF; }
"ELSE"              { return ELSE; }
"CONTINUE"          { return CONTINUE; }
"THEN"              { return THEN; }
":="                { return ASSIGN; }
{DIGIT}+            { return NUMBER; }
"BEGIN"             { return START; }
"RETURN"            { return RETURN; }

"VAR"               { return VAR; }

{WHITESPACE}*       {        }

FUNC                { return FUNC; }
{LETTER}({LETTER}|{DIGIT})* { return IDENTIFIER; }
[-()<>=+*/;{}.,]    { return *yytext; }
.                   { printf("Unrecognized token %s", yytext); }
%%
