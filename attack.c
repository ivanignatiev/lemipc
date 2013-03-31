/*
** attack.c for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Sat Mar 30 13:42:17 2013 vincent couvignou
** Last update Sun Mar 31 19:25:34 2013 vincent couvignou
*/

#include <math.h>
#include "lemipc_structures.h"
#include "lemipc.h"
#include "run_away.h"

#define ATK_RANGE	6

static int	range(t_player *player, int x, int y)
{
  int		range_x;
  int		range_y;

  range_x = (player->x - x > 0) ? player->x - x : x - player->x;
  range_y = (player->y - y > 0) ? player->y - y : y - player->y;
  return (sqrt(range_x * range_x + range_y * range_y));
}

static void	change_position(t_player *player, int x, int y)
{
  if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    return ;
  player->x = x;
  player->y = y;
}

static int	attack_ennemy(int range_player, t_player *player,
    			int min_x, int min_y)
{
  change_position(player, player->x + 1, player->y + 1);
  if (range_player >= range(player, min_x, min_y))
    return (1);
  change_position(player, player->x, player->y - 2);
  if (range_player >= range(player, min_x, min_y))
    return (1);
  change_position(player, player->x, player->y + 1);
  if (range_player >= range(player, min_x, min_y))
    return (1);
  change_position(player, player->x - 1, player->y - 1);
  if (range_player >= range(player, min_x, min_y))
    return (1);
  change_position(player, player->x, player->y + 2);
  if (range_player >= range(player, min_x, min_y))
    return (1);
  change_position(player, player->x - 1, player->y);
  if (range_player >= range(player, min_x, min_y))
    return (1);
  change_position(player, player->x, player->y - 2);
  if (range_player >= range(player, min_x, min_y))
    return (1);
  change_position(player, player->x, player->y + 1);
  if (range_player >= range(player, min_x, min_y))
    return (1);
  printf("No position...\n");
  return (1);
}

static int	find_ennemy(unsigned char d_field[HEIGHT][WIDTH], t_player *player,
    			t_ipc_res *ipc_res)
{
  int		min_x;
  int		min_y;
  int		max_x;
  int		max_y;
  int		teams[MAX_TEAM_NUM];

  (void)ipc_res;
  bzero(teams, sizeof(int) * MAX_TEAM_NUM);
  min_x = ((player->x - ATK_RANGE < 0) ? 0 : player->x - ATK_RANGE) - 1;
  min_y = ((player->y - ATK_RANGE < 0) ? 0 : player->y - ATK_RANGE) - 1;
  max_x = (player->x + ATK_RANGE > HEIGHT) ? HEIGHT : player->x + ATK_RANGE;
  max_y = (player->y + ATK_RANGE > WIDTH) ? WIDTH : player->y + ATK_RANGE;
  while (++min_x < max_x)
    while (++min_y < max_y)
    {
      if (d_field[min_x][min_y] != 0
	  && d_field[min_x][min_y] != player->team_id)
	   return (attack_ennemy(range(player, min_x, min_y),
	       			player, min_x, min_y));
    }
  return (0);
}

int		attack(t_player *player, unsigned char *field, t_ipc_res *ipc)
{
  unsigned char	d_field[HEIGHT][WIDTH];
  int		decision;
  int		current_x;
  int		current_y;

  current_x = player->x;
  current_y = player->y;
  create_dfield(field, d_field, ipc);
  if (!find_ennemy(d_field, player, ipc))
    return (0);
  decision = get_shm_index(player->x, player->y);
  if (field[decision] != 0 || semctl(ipc->sem_id, decision, GETVAL, 0) <= 0)
  {
    change_position(player, current_x, current_y);
    return (0);
  }
  lock_sem(ipc, get_shm_index(current_x, current_y));
  field[get_shm_index(current_x, current_y)] = 0;
  unlock_sem(ipc, get_shm_index(current_x, current_y));
  lock_sem(ipc, decision);
  player->x += 1;
  player->sh_i = decision;
  field[player->sh_i] = player->team_id;
  unlock_sem(ipc, decision);
  return (1);
}
