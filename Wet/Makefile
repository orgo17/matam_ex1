CC = gcc

MTM_OBJS = matamikya_help.o matamikya.o matamikya_tests.o matamikya_main.o matamikya_print.o
MTM_EXEC = matamikya
MTM_LIB_FLAG = -L. -lmtm -lm -las

AS_OBJS = amount_set_str.o amount_set_str_tests.o amount_set_str_main.o amount_set_str_list_str.o
AS_EXEC = amount_set_str

DEBUG_FLAG = # now empty, assign -g for debug
COMP_FLAG = -std=c99 --pedantic-errors -Wall -Werror


$(MTM_EXEC) : $(MTM_OBJS)
	$(CC) $(DEBUG_FLAG) $(MTM_OBJS) -o $@ $(MTM_LIB_FLAG)
matamikya_help.o : matamikya_help.c matamikya.c matamikya_help.h matamikya.h set.h amount_set.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
matamikya.o : matamikya.c matamikya_help.c matamikya_print.c matamikya.h \
					matamikya_help.h matamikya_print.h set.h amount_set.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
matamikya_print.o : matamikya_print.c matamikya_print.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
matamikya_tests.o : matamikya_tests.c matamikya.c test_utilities.h matamikya_tests.h matamikya.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c 
matamikya_main.o : matamikya_main.c matamikya_tests.c matamikya_tests.h test_utilities.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

$(AS_EXEC) : $(AS_OBJS)	
	$(CC) $(DEBUG_FLAG) $(AS_OBJS) -o $@
amount_set_str.o : amount_set_str.c amount_set_str_list_str.c amount_set_str.h amount_set_str_list_str.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
amount_set_str_tests.o : amount_set_str_tests.c amount_set_str.c amount_set_str_list_str.c \
						amount_set_str.h amount_set_str_list_str.h amount_set_str_tests.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
amount_set_str_main.o : amount_set_str_main.c amount_set_str_tests.c amount_set_str.c amount_set_str_list_str.c \
						amount_set_str.h amount_set_str_list_str.h amount_set_str_tests.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c
amount_set_str_list_str.o : amount_set_str_list_str.c amount_set_str_list_str.h
	$(CC) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.c

clean:
	rm -f $(AS_OBJS) $(AS_EXEC) $(MTM_OBJS) $(MTM_EXEC)