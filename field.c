/*
** field.c for lemipc in /home/ignati_i//projects/lemipc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sun Mar 31 19:22:15 2013 ivan ignatiev
** Last update Sun Mar 31 19:23:40 2013 ivan ignatiev
*/

#include	"lemipc.h"

int		count_aliens(int my_team, int *around)
{
  int		i;
  int		j;
  int		count;

  i = 0;
  while (i < 8)
    {
      j = 0;
      count = 0;
      while (j < 8)
	{
	  if (around[j] == around[i] && around[j] != my_team && around[j] > 0)
	    count = count + 1;
	  j = j + 1;
	}
      if (count >= 2)
	return (around[i]);
      i = i + 1;
    }
  return (0);
}

int		count_players(t_ipc_res *ipc_res,
			      t_uchar *field)
{
  int		count;
  int		i;

  count = 0;
  i = 0;
  while (i < WIDTH * HEIGHT)
    {
      lock_sem(ipc_res, i);
      if (field[i] > 0)
	count = count + 1;
      unlock_sem(ipc_res, i);
      i = i + 1;
    }
  return (count);
}

int		count_players_in_team(t_ipc_res *ipc_res, t_player *player,
				      t_uchar *field)
{
  int		count;
  int		i;

  count = -1;
  i = 0;
  while (i < WIDTH * HEIGHT)
    {
      lock_sem(ipc_res, i);
      if (field[i] == player->team_id)
	count = count + 1;
      unlock_sem(ipc_res, i);
      i = i + 1;
    }
  return (count);
}

int		get_shm_index(int x, int y)
{
  int		sh_i;

  sh_i = (y * WIDTH + x);
  if (sh_i >= (WIDTH * HEIGHT) || sh_i < 0)
    return (-1);
  return (sh_i);
}

int		get_shm_cell(t_ipc_res *ipc_res, int x, int y,
			     t_uchar *field)
{
  int		sh_i;
  int		value;

  sh_i = (y * WIDTH + x);
  if (sh_i >= (WIDTH * HEIGHT) || sh_i < 0)
    return (-1);
  lock_sem(ipc_res, sh_i);
  value = field[sh_i];
  unlock_sem(ipc_res, sh_i);
  return (value);
}
