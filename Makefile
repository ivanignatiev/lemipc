##
## Makefile for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
##
## Made by ivan ignatiev
## Login   <ignati_i@epitech.net>
##
## Started on  Tue Mar 26 14:10:36 2013 ivan ignatiev
## Last update Tue Mar 26 16:34:14 2013 vincent couvignou
##


SRCS 	= 	lemipc.c			\
			handle_messages.c	\
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
	$(CC) $(OBJS) -o $(NAME)

clean	:
	$(RM) $(OBJS)

fclean 	: clean
	$(RM) $(NAME)

re 	: fclean all

.PHONY: all clean fclean re
