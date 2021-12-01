CC = gcc

MTM_OBJS = matamikya_help.o matamikya.o matamikya_tests.o matamikya_main.o

AS_OBJS = amount_set_str.o amount_set_str_tests.o amount_set_str_main.o amount_set_str_list_str.o
AS_EXEC = amount_set_str_test_exe
DEBUG_FLAG = # now empty, assign -g for debug
AS_COMP_FLAG = -std=c99 --pedantic-errors -Wall -Werror

$(AS_EXEC) : $(AS_OBJS)	
	$(CC) $(DEBUG_FLAG) $(AS_OBJS) -o $@
amount_set_str.o : amount_set_str.c amount_set_str_list_str.c amount_set_str.h amount_set_str_list_str.h
	$(CC) -c $(DEBUG_FLAG) $(AS_COMP_FLAG) $*.c
amount_set_str_tests.o : tests/amount_set_str_tests.c amount_set_str.c amount_set_str_list_str.c amount_set_str.h amount_set_str_list_str.h tests/amount_set_str_tests.h
	$(CC) -c $(DEBUG_FLAG) $(AS_COMP_FLAG) $*.c
amount_set_str_tests.o : tests/amount_set_str_main.c tests/amount_set_str_tests.c amount_set_str.c amount_set_str_list_str.c amount_set_str.h amount_set_str_list_str.h tests/amount_set_str_tests.h
	$(CC) -c $(DEBUG_FLAG) $(AS_COMP_FLAG) $*.c
amount_set_str_list_str.o : amount_set_str_list_str.c amount_set_str_list_str.h
	$(CC) -c $(DEBUG_FLAG) $(AS_COMP_FLAG) $*.c
clean:
	rm -f $(AS_OBJS) $(AS_EXEC)