/*
** battle_counter.c for lemipc in /home/ignati_i//projects/lemipc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sun Mar 31 20:33:24 2013 ivan ignatiev
** Last update Sun Mar 31 20:33:36 2013 ivan ignatiev
*/

#include	"lemipc.h"

int		battle_begini_ctl(t_ipc_res *ipc_res, t_player *player,
				  t_uchar *field)
{
  int		i;

  i = FIGTH_TIMEOUT;
  while (i > 0)
    {
      send_msg_to_gui(ipc_res, player, "%d sec. to battle!", i);
      sleep(1);
      i = i - 1;
    }
  while (i < WIDTH * HEIGHT)
    {
      unlock_sem(ipc_res, AVAILBL_PLACES_SEM);
      i = i + 1;
  }
  return (count_players(ipc_res, field));
}
