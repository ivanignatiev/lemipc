/*
** my_list.c for LinkedList in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC/LinkedList
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Mon Mar 25 23:52:55 2013 vincent couvignou
** Last update Tue Mar 26 17:00:40 2013 vincent couvignou
*/

#include "my_list.h"

static void add_front_copy(t_my_list this, t_my_item src_item);
static void add_back_copy(t_my_list this, t_my_item src_item);
static void add_front(t_my_list this, void *data, int len);
static void add_back(t_my_list this, void *data, int len);
static bool exits(t_my_list this, void *data, int len);
static void iter(t_my_list this, void (*f)(void *, int));
static t_my_list map(t_my_list this, t_my_item (*f)(void *, int));
static void associate_functions(t_my_list src_list);

void associate_functions(t_my_list src_list)
{
  src_list->add_front_copy = &add_front_copy;
  src_list->add_back_copy = &add_back_copy;
  src_list->add_front = &add_front;
  src_list->add_back = &add_back;
  src_list->exits = &exits;
  src_list->iter = &iter;
  src_list->map = &map;
}

t_my_list		new_list_default()
{
  t_my_list	new_list;

  if ((new_list = (t_my_list)(malloc(sizeof(t_list)))) == NULL)
    return (NULL);
  new_list->size_list = 0;
  new_list->is_empty = true;
  new_list->head = NULL;
  new_list->tail = NULL;
  associate_functions(new_list);
  return (new_list);
}

void		delete_list(t_my_list src_list)
{
  t_my_item	tmp;

  tmp = src_list->head;
  while (src_list->head != NULL)
  {
    tmp = src_list->head->next;
    delete_item(src_list->head);
    src_list->head = tmp;
  }
  free(src_list);
}

static void	add_front_copy(t_my_list this, t_my_item src_item)
{
  t_my_item	new_item;

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
static void	add_back_copy(t_my_list this, t_my_item src_item)
{
  t_my_item	new_item;
  t_my_item	tmp;

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
static void	add_front(t_my_list this, void *data, int len)
{
  t_my_item	new_item;

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
static void	add_back(t_my_list this, void *data, int len)
{
  t_my_item	new_item;
  t_my_item	tmp;

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
static bool	exits(t_my_list this, void *data, int len)
{
  t_my_item	tmp;

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
static void iter(t_my_list this, void (*f)(void *, int))
{
  t_my_item	tmp;

  if (this == NULL || f == NULL)
    return ;
  tmp = this->head;
  while (tmp != NULL)
  {
    f(tmp->content, tmp->size);
    tmp = tmp->next;
  }
}
static t_my_list map(t_my_list this, t_my_item (*f)(void *, int))
{
  t_my_item	tmp;
  t_my_item 	new_item;
  t_my_list	new_list;

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

