/*
** my_item.h for LinkedList in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC/LinkedList
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Mon Mar 25 23:51:34 2013 vincent couvignou
** Last update Tue Mar 26 16:59:59 2013 vincent couvignou
*/

#ifndef _MY_ITEM_HH_
# define _MY_ITEM__HH_

# include <stdlib.h>

typedef struct 	s_item
{
  void 		*content;
  int 		size;
  struct s_item *prev;
  struct s_item *next;
} 		t_item;
typedef struct s_item *t_my_item;

t_my_item 	new_item_default();
t_my_item 	new_item_params(void *src_content, const int src_size);
t_my_item 	new_item_cpy(t_my_item src_item);
void 		delete_item(t_my_item src_item);
#endif /* !_MY_ITEM_HH_ */
