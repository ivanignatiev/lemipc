/*
** my_item.h for  in /home/couvig_v//ProjetsEnCours/LinkedList
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Mon Mar 25 23:51:34 2013 vincent couvignou
** Last update Mon Mar 25 23:51:35 2013 vincent couvignou
*/

#ifndef MY_ITEM_HH_
# define MY_ITEM_HH_

# include <stdlib.h>

typedef struct 	s_item
{
  void 		*content;
  int 		size;
  struct s_item *prev;
  struct s_item *next;
} 		t_item;
typedef struct s_item *my_item;

my_item 	new_item_default();
my_item 	new_item_params(void *src_content, const int src_size);
my_item 	new_item_cpy(my_item src_item);
void 		delete_item(my_item src_item);
#endif /* !MY_ITEM_HH_ */
