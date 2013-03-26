/*
** my_list.h for LinkedList in /home/couvig_v/ProjetsEnCours/LinkedList
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Mon Mar 25 23:52:46 2013 vincent couvignou
** Last update Tue Mar 26 00:50:21 2013 vincent couvignou
*/

#ifndef MY_LIST_H_
# define MY_LIST_H_

# include "my_item.h"
# include <stdbool.h>

typedef struct s_list 	*my_list;
typedef struct	s_list
{
  int 		size_list;
  bool		is_empty;
  my_item	head;
  my_item	tail;

  void (*add_front_copy)(my_list this, my_item src_item);
  void (*add_back_copy)(my_list this, my_item src_item);
  void (*add_front)(my_list this, void *data, int len);
  void (*add_back)(my_list this, void *data, int len);
  bool (*exits)(my_list this, void *data, int len);
  void (*iter)(my_list this, void (*f)(void *, int));
  my_list (*map)(my_list this, my_item (*f)(void *, int));
} 		t_list;

my_list		new_list_default();
void		delete_list(my_list);

#endif /* !MY_LIST_H_ */
