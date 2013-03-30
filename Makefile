##
## Makefile for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
##
## Made by ivan ignatiev
## Login   <ignati_i@epitech.net>
##
## Started on  Tue Mar 26 14:10:36 2013 ivan ignatiev
## Last update Sat Mar 30 14:47:16 2013 vincent couvignou
##


SRCS 	= 	lemipc.c			\
			handle_messages.c	\
			run_away.c			\
			random_move.c		\
			attack.c			\
			./LinkedList/my_item.c	\
			./LinkedList/my_list.c

NAME 	= 	lemipc

OBJS 	=	$(SRCS:.c=.o)

CFLAGS  += 	-I./LinkedList
CFLAGS 	+= 	-W -Wextra

CC 	= cc

RM 	=	 rm -f

all 	: $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(OBJS) -o $(NAME) -lm

clean	:
	$(RM) $(OBJS)

fclean 	: clean
	$(RM) $(NAME)

re 	: fclean all

.PHONY: all clean fclean re
