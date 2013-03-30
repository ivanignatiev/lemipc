/*
** random_move.c for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Fri Mar 29 16:46:11 2013 vincent couvignou
** Last update Sat Mar 30 13:34:37 2013 vincent couvignou
*/

#include <stdlib.h>
#include "lemipc_structures.h"

int             count_player(int *around)
{
  int           i;
  int           count;

  i = -1;
  while (++i < 8)
   if (around[i] > 0)
     count++;
  return (count);
}

void	move(unsigned char *field, t_player *player,
    	t_ipc_res *ipc_res, int *around)
{
  int	p_case;

  p_case = rand() % 8;
  lock_sem(ipc_res, player->sh_i);
  field[player->sh_i] = 0;
  unlock_sem(ipc_res, player->sh_i);
  player->sh_i = p_case;
  lock_sem(ipc_res, player->sh_i);
  field[player->sh_i] = player->team_id;
  unlock_sem(ipc_res, player->sh_i);
}

bool	test_move(unsigned char *field, t_player *player,
    		t_ipc_res *ipc_res, int *around)
{
  char          die_msg[100];

  around[0] =  get_shm_cell(player->x - 1, player->y - 1, field);
  around[1] =  get_shm_cell(player->x, player->y - 1, field);
  around[2] =  get_shm_cell(player->x + 1, player->y - 1, field);
  around[3] =  get_shm_cell(player->x - 1, player->y, field);
  around[4] =  get_shm_cell(player->x + 1, player->y, field);
  around[5] =  get_shm_cell(player->x - 1, player->y + 1, field);
  around[6] =  get_shm_cell(player->x, player->y + 1, field);
  around[7] =  get_shm_cell(player->x + 1, player->y + 1, field);
  if (count_player(around) < 8)
    return (true);
  return (false);
}

int			random_move(t_player *player, unsigned char *field,
    t_ipc_res *ipc_res)
{
  unsigned char	d_field[HEIGHT][WIDTH];
  int           around[8];

  create_dfield(d_field);
  if (test_move(field, player, ipc_res, around) == false)
    return (1);
  move(field, player, ipc_res, around);
  return (1);
}
