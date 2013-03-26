/*
** my_list.c for LinkedList in /home/couvig_v/ProjetsEnCours/LinkedList
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Mon Mar 25 23:52:55 2013 vincent couvignou
** Last update Tue Mar 26 00:57:08 2013 vincent couvignou
*/

#include "my_list.h"

static void add_front_copy(my_list this, my_item src_item);
static void add_back_copy(my_list this, my_item src_item);
static void add_front(my_list this, void *data, int len);
static void add_back(my_list this, void *data, int len);
static bool exits(my_list this, void *data, int len);
static void iter(my_list this, void (*f)(void *, int));
static my_list map(my_list this, my_item (*f)(void *, int));
static void associate_functions(my_list src_list);

void associate_functions(my_list src_list)
{
  src_list->add_front_copy = &add_front_copy;
  src_list->add_back_copy = &add_back_copy;
  src_list->add_front = &add_front;
  src_list->add_back = &add_back;
  src_list->exits = &exits;
  src_list->iter = &iter;
  src_list->map = &map;
}

my_list		new_list_default()
{
  my_list	new_list;

  if ((new_list = (my_list)(malloc(sizeof(t_list)))) == NULL)
    return (NULL);
  new_list->size_list = 0;
  new_list->is_empty = true;
  new_list->head = NULL;
  new_list->tail = NULL;
  associate_functions(new_list);
  return (new_list);
}

void		delete_list(my_list src_list)
{
  my_item	tmp;

  tmp = src_list->head;
  while (src_list->head != NULL)
  {
    tmp = src_list->head->next;
    delete_item(src_list->head);
    src_list->head = tmp;
  }
  free(src_list);
}

static void	add_front_copy(my_list this, my_item src_item)
{
  my_item	new_item;

  new_item = new_item_cpy(src_item);
  if (new_item == NULL || this == NULL)
    return ;
  new_item->next = this->head;
  new_item->prev = NULL;
  if (this->head != NULL)
    this->head->prev = new_item;
  this->head = new_item;
  this->size_list++;
  this->is_empty = false;
  if (this->size_list == 1)
    this->tail = new_item;
}
static void	add_back_copy(my_list this, my_item src_item)
{
  my_item	new_item;
  my_item	tmp;

  new_item = new_item_cpy(src_item);
  if (new_item == NULL || this == NULL)
    return ;
  new_item->next = NULL;
  tmp = this->head;
  if (tmp != NULL)
  {
    while (tmp->next != NULL)
      tmp = tmp->next;
    new_item->prev = tmp;
    tmp->next = new_item;
    this->tail = new_item;
  }
  else
  {
    this->head = new_item;
    this->tail = new_item;
    new_item->prev = NULL;
  }
  this->size_list++;
  this->is_empty = false;
}
static void	add_front(my_list this, void *data, int len)
{
  my_item	new_item;

  new_item = new_item_params(data, len);
  if (new_item == NULL || this == NULL)
    return ;
  new_item->next = this->head;
  new_item->prev = NULL;
  if (this->head != NULL)
    this->head->prev = new_item;
  this->head = new_item;
  this->size_list++;
  this->is_empty = false;
  if (this->size_list == 1)
    this->tail = new_item;

}
static void	add_back(my_list this, void *data, int len)
{
  my_item	new_item;
  my_item	tmp;

  new_item = new_item_params(data, len);
  if (new_item == NULL || this == NULL)
    return ;
  new_item->next = NULL;
  tmp = this->head;
  if (tmp != NULL)
  {
    while (tmp->next != NULL)
      tmp = tmp->next;
    new_item->prev = tmp;
    tmp->next = new_item;
    this->tail = new_item;
  }
  else
  {
    this->head = new_item;
    this->tail = new_item;
    new_item->prev = NULL;
  }
  this->size_list++;
  this->is_empty = false;
}
static bool	exits(my_list this, void *data, int len)
{
  my_item	tmp;

  if (this == NULL)
    return (false);
  tmp = this->head;
  while (tmp != NULL)
  {
    if (tmp->size == len && tmp->content == data)
      return (true);
    tmp = tmp->next;
  }
  return (false);
}
static void iter(my_list this, void (*f)(void *, int))
{
  my_item	tmp;

  if (this == NULL || f == NULL)
    return ;
  tmp = this->head;
  while (tmp != NULL)
  {
    f(tmp->content, tmp->size);
    tmp = tmp->next;
  }
}
static my_list map(my_list this, my_item (*f)(void *, int))
{
  my_item	tmp;
  my_item 	new_item;
  my_list	new_list;

  new_list = new_list_default();
  if (this == NULL || f == NULL || new_list == NULL)
    return (NULL);
  tmp = this->head;
  while (tmp != NULL)
  {
    new_item = f(tmp->content, tmp->size);
    new_list->add_front_copy(new_list, new_item);
    free(new_item);
    new_item = NULL;
    tmp = tmp->next;
  }
  return (new_list);
}

