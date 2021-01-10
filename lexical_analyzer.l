%{
    #include "mysh_utils.h"
    #include "syntax_analyzer.tab.h"



    string_buffer_t buffer;

    #define ret_IDENTIFIER(text) { yylval.string = (text); *current_token() = yylval.string.str; buffer = string_buffer_empty; return IDENTIFIER; }
    #define ret_SEMIC { *current_token() = ";";return SEMIC; }
    #define ret_NEWL { *line_count()+=1; *current_token()="\\n"; return NEWL; }
%}

%option noyywrap nounput noinput


BLANK_CHAR [\0- ]

SINGLE_LINE_COMMENT ("#"[^\n]*)


IDENTIFIER_CHAR (!|([$-~]{-}[;']))

%x string_reader
%x string_reader_apostrophe
%x reading_comment

%%

{SINGLE_LINE_COMMENT} {}


";" { ret_SEMIC; }

{IDENTIFIER_CHAR}+ { ret_IDENTIFIER( str_copy(yytext)); }

\" { buffer = make_string_buffer(8); BEGIN(string_reader); }

<string_reader>{
    \\\" { append_char_to_buffer(&buffer, '"'); }

    \" { BEGIN(INITIAL); ret_IDENTIFIER(buffer.str); }
    
    \n { *line_count()+=1; append_char_to_buffer(&buffer, *yytext); }

    . { append_char_to_buffer(&buffer, *yytext); }
}

' { buffer = make_string_buffer(8); BEGIN(string_reader_apostrophe); }

<string_reader_apostrophe>{
    \\' { append_char_to_buffer(&buffer, '\''); }

    ' { BEGIN(INITIAL); ret_IDENTIFIER(buffer.str); }

    \n { *line_count()+=1; append_char_to_buffer(&buffer, *yytext); }

    . { append_char_to_buffer(&buffer, *yytext); }
}

\n { *line_count()+=1; return NEWL; }

{BLANK_CHAR} {}

. { printf("Unknown token: '%s'", yytext); }

%%


void flex_cleanup(void);


int scan_string(const char* to_scan)
{
    yy_scan_string(to_scan);

    int ret = yyparse();

    yy_delete_buffer(YY_CURRENT_BUFFER);
    return ret;
}

int scan_file(FILE *file)
{
    YY_BUFFER_STATE buf = yy_create_buffer(file, YY_BUF_SIZE);
    yy_switch_to_buffer(buf);

    int ret = yyparse();

    yy_delete_buffer(buf);
    return ret;
}



