/*
** handle_messages.c for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Tue Mar 26 15:57:22 2013 vincent couvignou
** Last update Fri Mar 29 11:47:10 2013 vincent couvignou
*/

#include "handle_messages.h"

void			newp_messages(t_ipc_res *ipc_res, t_player *player, const char *msg)
{
  int			player_num;
  t_player_list		*newp;
  char			resp[MESSAGE_SIZE];

  if ((newp = malloc(sizeof(*newp))) == NULL)
    return ;
  sscanf(msg, "NEWP:%d", &player_num);
  sprintf(resp, "OLDP:%d", player->num);
  send_message_to_player(ipc_res, player, player_num, resp);
  printf("New player came to our team (%d)!\n", player_num);
  newp->player_number = player_num;
  newp->activated = true;
  newp->player_x = 0;
  newp->player_y = 0;
  player->player_list->add_front(player->player_list, newp, sizeof(*newp));
}

void	oldp_messages(t_ipc_res *ipc_res, t_player *player, const char *msg)
{
  int	player_num;
  t_player_list		*newp;
  char	resp[MESSAGE_SIZE];

if ((newp = malloc(sizeof(*newp))) == NULL)
    return ;
  sscanf(msg, "OLDP:%d", &player_num);
  sprintf(resp, "OLDP:%d", player->num);
  printf("Old player %d\n", player_num);
  newp->player_number = player_num;
  newp->activated = true;
  newp->player_x = 0;
  newp->player_y = 0;
  player->player_list->add_front(player->player_list, newp, sizeof(*newp));
}

void	move_messages(t_ipc_res *ipc_res, t_player *player, const char *msg)
{
  int	player_num;
  char	resp[MESSAGE_SIZE];

  sscanf(msg, "MOVE:%d", &player_num);
  sprintf(resp, "MOVE:%d", player->num);
  /* Find where to move and move one case... */
}

void		diep_messages(t_ipc_res *ipc_res, t_player *player, const char *msg)
{
  int		player_num;
  char		resp[MESSAGE_SIZE];
  t_my_item	tmp;
  t_player_list	*tmp_player;

  tmp_player = NULL;
  sscanf(msg, "DIEP:%d", &player_num);
  sprintf(resp, "DIEP:%d", player->num);
  tmp = player->player_list->head;
  while (tmp != NULL)
  {
    bcopy(tmp_player, tmp->content, tmp->size);
    if (tmp_player->player_number == player_num)
    {
      tmp_player->activated = false;
      return ;
    }
    tmp = tmp->next;
  }
}

void	init_fct(t_fct_messages *fct_array)
{
  int	i;

  i = -1;
  while (++i < NB_KIND)
    bzero(fct_array[i].name, KIND_MESSAGE_S);
  strcpy(fct_array[0].name, "NEWP:");
  fct_array[0].p_fct = &newp_messages;
  strcpy(fct_array[1].name, strdup("OLDP:"));
  fct_array[1].p_fct = &oldp_messages;
  strcpy(fct_array[2].name, strdup("MOVE:"));
  fct_array[2].p_fct = &move_messages;
  strcpy(fct_array[3].name, strdup("DIEP:"));
  fct_array[3].p_fct = &diep_messages;
}
