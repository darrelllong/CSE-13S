%option noinput
%option nounput
%option noyywrap

LETTER          [A-Za-z_]
DIGIT           [0-9]
WHITESPACE      [ \t\r\n]
PUNCTUATION     [~`!@#$%^&*()_+-={}|\\\[\]:;\"\'<>,./?]

WORD            ({LETTER}+)
IDENTIFIER      ({LETTER}+{DIGIT}+)
CONTRACTION     ({LETTER}+\'{LETTER}+)
HYPHENATION     ({LETTER}+\-{LETTER}+)

%%

{WORD}                  { return 0; }
{IDENTIFIER}            { return 0; }
{CONTRACTION}           { return 0; }
{HYPHENATION}           { return 0; }
{PUNCTUATION}+          {}
{WHITESPACE}+           {}
<<EOF>>                 { return -1; }
.                       {}

%%
