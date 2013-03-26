/*
** handle_messages.c for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Tue Mar 26 15:57:22 2013 vincent couvignou
** Last update Tue Mar 26 16:23:13 2013 vincent couvignou
*/

#include "handle_messages.h"

void	newp_messages(t_ipc_res *ipc_res, t_player *player, const char *msg)
{
  int	player_num;
  char	resp[MESSAGE_SIZE];

  sscanf(msg, "NEWP:%d", &player_num);
  sprintf(resp, "OLDP:%d", player->num);
  send_message_to_player(ipc_res, player, player_num, resp);
  printf("New player came to our team (%d)!\n", player_num);
  /* TODO : Add player to our list ! */
}

void	oldp_messages(t_ipc_res *ipc_res, t_player *player, const char *msg)
{
  int	player_num;
  char	resp[MESSAGE_SIZE];

  sscanf(msg, "OLDP:%d", &player_num);
  sprintf(resp, "OLDP:%d", player->num);
  /* TODO : Add player to our list ! */
}

void	move_messages(t_ipc_res *ipc_res, t_player *player, const char *msg)
{
  int	player_num;
  char	resp[MESSAGE_SIZE];

  sscanf(msg, "MOVE:%d", &player_num);
  sprintf(resp, "MOVE:%d", player->num);
  /* Find where to move and move one case... */
}

void	init_fct(t_fct_messages *fct_array)
{
  int	i;

  i = -1;
  while (++i < 3)
    bzero(fct_array[i].name, KIND_MESSAGE_S);
  strcpy(fct_array[0].name, "NEWP:");
  fct_array[0].p_fct = &newp_messages;
  strcpy(fct_array[1].name, strdup("OLDP:"));
  fct_array[1].p_fct = &oldp_messages;
  strcpy(fct_array[2].name, strdup("MOVE:"));
  fct_array[2].p_fct = &move_messages;
}
