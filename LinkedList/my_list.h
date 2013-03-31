/*
** my_list.h for LinkedList in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC/LinkedList
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Mon Mar 25 23:52:46 2013 vincent couvignou
** Last update Sun Mar 31 21:26:43 2013 vincent couvignou
*/

#ifndef t_my_list_H_
# define t_my_list_H_

# include <stdbool.h>
# include "my_item.h"

typedef struct s_list 	*t_my_list;
typedef struct	s_list
{
  int 		size_list;
  bool		is_empty;
  t_my_item	head;
  t_my_item	tail;

  void (*add_front_copy)(t_my_list this, t_my_item src_item);
  void (*add_front)(t_my_list this, void *data, int len);
  void (*add_back)(t_my_list this, void *data, int len);
} 		t_list;

t_my_list		new_list_default();
void		delete_list(t_my_list);

#endif /* !t_my_list_H_ */
