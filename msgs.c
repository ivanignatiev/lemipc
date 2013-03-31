/*
** msgs.c for lemipc in /home/ignati_i//projects/lemipc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sun Mar 31 19:16:18 2013 ivan ignatiev
** Last update Sun Mar 31 20:38:11 2013 ivan ignatiev
*/

#include	"lemipc.h"

int		send_msg_to_team(t_ipc_res *ipc_res, t_player *player,
				 int count, const char *msg)
{
  t_msg		snd_msg;
  int		player_num;
  int		last_num;

  bzero(&snd_msg, sizeof(t_msg));
  strcpy(snd_msg.msg, msg);
  snd_msg.sender = player->num;
  player_num = player->team_id * MAX_TEAM_NUM;
  last_num = count + player_num;
  while (player_num <= last_num)
    {
      if (player_num != player->num)
	{
	  snd_msg.mtype = player_num;
	  msgsnd(ipc_res->msg_id, (const void *)&snd_msg, sizeof(t_msg), IPC_NOWAIT);
	}
      player_num = player_num + 1;
    }
  return (0);
}

int		send_msg_to_player(t_ipc_res *ipc_res, t_player *player,
				   int player_num, const char *msg)
{
  t_msg		snd_msg;

  bzero(&snd_msg, sizeof(t_msg));
  strcpy(snd_msg.msg, msg);
  snd_msg.sender = player->num;
  snd_msg.mtype = player_num;
  return (msgsnd(ipc_res->msg_id, (const void *)&snd_msg, sizeof(t_msg), IPC_NOWAIT));
}

int		send_msg_to_gui(t_ipc_res *ipc_res, t_player *player,
				const char *msg, ...)
{
  t_msg		snd_msg;
  va_list	ap;

  bzero(&snd_msg, sizeof(t_msg));
  va_start(ap, msg);
  vsprintf(snd_msg.msg, msg, ap);
  va_end(ap);
  snd_msg.sender = player->num;
  snd_msg.mtype = GUI_MSG_TYPE;
  return (msgsnd(ipc_res->msg_id, (const void *)&snd_msg, sizeof(t_msg), IPC_NOWAIT));
}

int		recv_msg_from_team(t_ipc_res *ipc_res,
    t_player *player, t_msg *msg)
{
  return (msgrcv(ipc_res->msg_id, (void*)msg,
		 sizeof(t_msg), player->num, IPC_NOWAIT));
}

void		parse_message(t_ipc_res *ipc_res, t_player *player,
			      const char *msg, t_fct_msgs *p_fct)
{
    int		i;

    i = 0;
    while (strncmp(msg, p_fct[i].name, strlen(p_fct[i].name)) && i < NB_KIND)
      i++;
    if (i != NB_KIND)
      p_fct[i].p_fct(ipc_res, player, msg);
}
