/*
** handle_messages.h for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Tue Mar 26 16:00:23 2013 vincent couvignou
** Last update Sun Mar 31 20:36:57 2013 ivan ignatiev
*/

#ifndef HANDLE_MESSAGES_H_
# define HANDLE_MESSAGES_H_

# include <string.h>
# include <strings.h>
# include "lemipc_structures.h"
# define MESSAGE_SIZE	128
# define KIND_MESSAGE_S	5
# define NB_KIND	4

typedef struct	s_fct_msgs
{
  char		name[KIND_MESSAGE_S];
  void		(*p_fct)();
}		t_fct_msgs;

void	newp_messages(t_ipc_res *res, t_player *player, const char *msg);
void	oldp_messages(t_ipc_res *ipc_res, t_player *player, const char *msg);
void	move_messages(t_ipc_res *ipc_res, t_player *player, const char *msg);
void	init_fct(t_fct_msgs *fct_array);

#endif /* !HANDLE_MESSAGES_H_ */
