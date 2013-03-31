/*
** random_move.c for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Fri Mar 29 16:46:11 2013 vincent couvignou
** Last update Sun Mar 31 15:24:30 2013 vincent couvignou
*/

#include "random_move.h"
#include "run_away.h"
#include "lemipc.h"

int             count_player(int *around, unsigned char *field)
{
  int           i;
  int           count;

  i = -1;
  count = 0;
  while (++i < 8)
  {
    printf("[%d] ", field[around[i]]);
   if (field[around[i]] > 0)
     count++;
  }
  printf("\n");
  return (count);
}

void	move_r(unsigned char *field, t_player *player,
    	t_ipc_res *ipc_res, int *around)
{
  int	p_case;
  int	previous_sh_i;

  p_case = rand() % 8;
  previous_sh_i = player->sh_i;
  while (around[p_case] < 0 || field[around[p_case]] != 0)
    p_case = rand() % 8;
  player->sh_i = around[p_case];
  printf("[%d][%d] ", player->x, player->y);
  player->x = around[p_case] / HEIGHT;
  player->y = around[p_case] % HEIGHT;
  printf("new[%d][%d]\n", player->x, player->y);
  lock_sem(ipc_res, previous_sh_i);
  lock_sem(ipc_res, player->sh_i);
  field[previous_sh_i] = 0;
  field[player->sh_i] = player->team_id;
  unlock_sem(ipc_res, previous_sh_i);
  unlock_sem(ipc_res, player->sh_i);
}

bool	test_move(unsigned char *field, t_player *player,
    		t_ipc_res *ipc_res, int *around)
{
  (void)field;
  (void)ipc_res;
  around[0] =  get_shm_index(player->x - 1, player->y - 1);
  around[1] =  get_shm_index(player->x, player->y - 1);
  around[2] =  get_shm_index(player->x + 1, player->y - 1);
  around[3] =  get_shm_index(player->x - 1, player->y);
  around[4] =  get_shm_index(player->x + 1, player->y);
  around[5] =  get_shm_index(player->x - 1, player->y + 1);
  around[6] =  get_shm_index(player->x, player->y + 1);
  around[7] =  get_shm_index(player->x + 1, player->y + 1);
  int count;
  if ((count = count_player(around, field)) < 8)
    return (true);
  printf("count_player[%ld][%d]\n", player->team_id, count);
  return (false);
}

int			random_move(t_player *player, unsigned char *field,
    t_ipc_res *ipc_res)
{
  unsigned char	d_field[HEIGHT][WIDTH];
  int           around[8];

  printf("In random move[%ld]\n", player->team_id);
  create_dfield(field, d_field, ipc_res);
  if (test_move(field, player, ipc_res, around) == false)
    return (1);
  move_r(field, player, ipc_res, around);
  return (1);
}
