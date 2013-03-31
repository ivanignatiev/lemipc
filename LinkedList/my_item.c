/*
** my_item.c for  in /home/couvig_v//ProjetsEnCours/LemIPC/LemIPC/LinkedList
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Sun Mar 31 21:24:09 2013 vincent couvignou
** Last update Sun Mar 31 21:24:11 2013 vincent couvignou
*/

#include "my_item.h"

t_my_item 	new_item_default()
{
  t_my_item 	new_item;

  if ((new_item = (t_my_item)(malloc(sizeof(t_item)))) == NULL)
    return (NULL);
  new_item->content = NULL;
  new_item->size = 0;
  new_item->prev = NULL;
  new_item->next = NULL;
  return (new_item);
}

t_my_item 	new_item_params(void *src_content, const int src_size)
{
  t_my_item 	new_item;

  if ((new_item = (t_my_item)(malloc(sizeof(t_item)))) == NULL)
    return (NULL);
  new_item->content = src_content;
  new_item->size = src_size;
  new_item->prev = NULL;
  new_item->next = NULL;
  return (new_item);
}

t_my_item 	new_item_cpy(t_my_item src_item)
{
  t_my_item 	new_item;

  if ((new_item = (t_my_item)(malloc(sizeof(t_item)))) == NULL
      || src_item == NULL)
    return (NULL);
  new_item->content = src_item->content;
  new_item->size = src_item->size;
  new_item->prev = src_item->prev;
  new_item->next = src_item->next;
  return (new_item);
}

void 		delete_item(t_my_item src_item)
{
  if (src_item == NULL)
    return ;
 if (src_item->content != NULL)
   free(src_item->content);
 free(src_item);
}
