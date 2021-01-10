all: lexical_analyzer.l syntax_analyzer.y mysh_utils.h mysh_utils.c datastructs/ultimate_list_on_heap.h datastructs/macro_magic.h datastructs/ultimate_list.h datastructs/ultimate_list.c
	  bison -d syntax_analyzer.y --verbose
	  flex lexical_analyzer.l
	  cc -o  mysh syntax_analyzer.tab.c lex.yy.c mysh_utils.c  datastructs/ultimate_list.c   -lreadline  -lfl -Wall -Wextra



