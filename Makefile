##
## Makefile for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
##
## Made by ivan ignatiev
## Login   <ignati_i@epitech.net>
##
## Started on  Tue Mar 26 14:10:36 2013 ivan ignatiev
## Last update Sun Mar 31 21:32:28 2013 ivan ignatiev
##

SRCS 	= 	lemipc.c			\
		handle_messages.c		\
		sems.c				\
		field.c				\
		player.c			\
		move_functions.c		\
		test_functions.c		\
		create_field.c			\
		go_functions.c			\
		ipcctl.c			\
		msgs.c				\
		run_away.c			\
		random_move.c			\
		attack.c			\
		battle_counter.c 		\
		./LinkedList/my_item.c		\
		./LinkedList/my_list.c

GSRCS	=	gui.c 				\
		sems.c				\
		ipcctl.c 			\
		init_gui.c

NAME 	= 	lemipc

GNAME	=	gui

CFLAGS  += 	-I./LinkedList
CFLAGS 	+= 	-Wall -Wextra

OBJS 	=	$(SRCS:.c=.o)
GOBJS 	=	$(GSRCS:.c=.o)

CC 	= cc

RM 	=	 rm -f

all 	: $(NAME) $(GNAME)

$(NAME) : $(OBJS)
	$(CC) $(OBJS) -o $(NAME) -lm

$(GNAME) : $(GOBJS)
	$(CC) $(GOBJS) -o $(GNAME) -lncurses

clean	:
	$(RM) $(OBJS)
	$(RM) $(GOBJS)

fclean 	: clean
	$(RM) $(NAME)
	$(RM) $(GNAME)

re 	: fclean all

.PHONY: all clean fclean re
