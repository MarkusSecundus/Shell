all: lexical_analyzer.l syntax_analyzer.y mysh_main.h mysh_main.c   strings.h syntax_model.h envvars.h shell_parser_interop.h builtin_commands.h  strings.c shell_memory.c syntax_model.c envvars.c shell_parser_interop.c builtin_commands.c     datastructs/ultimate_list_on_heap.h datastructs/macro_magic.h datastructs/ultimate_list.h datastructs/ultimate_list.c
	  bison -d syntax_analyzer.y --verbose
	  flex lexical_analyzer.l
	  cc -o  mysh syntax_analyzer.tab.c lex.yy.c mysh_main.c strings.c shell_memory.c syntax_model.c envvars.c shell_parser_interop.c builtin_commands.c  datastructs/ultimate_list.c   -lreadline  -Wall -Wextra -std=c99



