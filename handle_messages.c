/*
** handle_messages.c for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Tue Mar 26 15:57:22 2013 vincent couvignou
** Last update Sun Mar 31 21:41:01 2013 ivan ignatiev
*/

#include "handle_messages.h"
#include "lemipc.h"

void			newp_messages(t_ipc_res *ipc_res, t_player *player,
				      const char *msg)
{
  t_player_list		*newp;
  char			resp[MESSAGE_SIZE];

  if ((newp = malloc(sizeof(t_player_list))) == NULL)
    return ;
  sscanf(msg, "NEWP:%d:%d:%d",
        &newp->player_number, &newp->player_x, &newp->player_y);
  sprintf(resp, "OLDP:%d:%d:%d", player->num, player->x, player->y);
  send_msg_to_player(ipc_res, player, newp->player_number, resp);
  newp->activated = true;
  player->player_list->add_front(player->player_list, newp, sizeof(*newp));
}

void			oldp_messages(t_ipc_res *ipc_res, t_player *player,
				      const char *msg)
{
  t_player_list		*newp;

  (void)ipc_res;
  if ((newp = malloc(sizeof(t_player_list))) == NULL)
    return ;
  sscanf(msg, "OLDP:%d:%d:%d",
      &newp->player_number, &newp->player_x, &newp->player_y);
  newp->activated = true;
  player->player_list->add_front(player->player_list, newp, sizeof(*newp));
}

void	move_messages(t_ipc_res *ipc_res, t_player *player, const char *msg)
{
  int	player_num;
  char	resp[MESSAGE_SIZE];

  (void)ipc_res;
  sscanf(msg, "MOVE:%d", &player_num);
  sprintf(resp, "MOVE:%d", player->num);
}

void		diep_messages(t_ipc_res *ipc_res, t_player *player,
			      const char *msg)
{
  int		player_num;
  char		resp[MESSAGE_SIZE];
  t_my_item	tmp;
  t_player_list	*tmp_player;

  (void)ipc_res;
  tmp_player = NULL;
  sscanf(msg, "DIEP:%d", &player_num);
  sprintf(resp, "DIEP:%d", player->num);
  tmp = player->player_list->head;
  while (tmp != NULL)
  {
    tmp_player = (t_player_list *)tmp->content;
    if (tmp_player->player_number == player_num)
    {
      tmp_player->activated = false;
      return ;
    }
    tmp = tmp->next;
  }
}

void	init_fct(t_fct_msgs *fct_array)
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
