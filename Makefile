##
## Makefile for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
##
## Made by ivan ignatiev
## Login   <ignati_i@epitech.net>
##
## Started on  Tue Mar 26 14:10:36 2013 ivan ignatiev
## Last update Sat Mar 30 16:57:38 2013 ivan ignatiev
##


SRCS 	= 	lemipc.c			\
			handle_messages.c	\
			run_away.c		\
			random_move.c		\
			attack.c		\
			./LinkedList/my_item.c	\
			./LinkedList/my_list.c

GSRCS	=	gui.c

NAME 	= 	lemipc

GNAME	=	gui

CFLAGS  += 	-I./LinkedList
CFLAGS 	+= 	-W -Wextra

OBJS 	=	$(SRCS:.c=.o)
GOBJS 	=	$(GSRCS:.c=.o)

CC 	= cc

RM 	=	 rm -f

all 	: $(NAME) $(GNAME)

$(NAME) : $(OBJS)
	$(CC) $(OBJS) -o $(NAME) -lm

$(GNAME) : $(GOBJS)
	$(CC) $(GOBJS) -o $(GNAME) -lm


clean	:
	$(RM) $(OBJS)
	$(RM) $(GOBJS)

fclean 	: clean
	$(RM) $(NAME)
	$(RM) $(GNAME)

re 	: fclean all

.PHONY: all clean fclean re
