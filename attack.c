/*
** attack.c for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Sat Mar 30 13:42:17 2013 vincent couvignou
** Last update Sat Mar 30 18:35:18 2013 vincent couvignou
*/

#include <math.h>
#include "lemipc_structures.h"
#include "run_away.h"

#define ATK_RANGE	4

int	count_ally_atk(unsigned char d_field[HEIGHT][WIDTH], t_player *player,
    		int range)
{
  int	min_x;
  int	min_y;
  int	max_x;
  int	max_y;
  int	count;

  min_x = ((player->x - range + 1< 0) ? 0 : player->x - range) + 1;
  min_y = ((player->y - range + 1< 0) ? 0 : player->y - range) + 1;
  max_x = (player->x + range + 1 > HEIGHT)
    		? HEIGHT : player->x + range + 1;
  max_y = (player->y + range + 1 > WIDTH)
    		? WIDTH : player->y + range + 1;
  count = 0;
  while (min_x < max_x)
  {
    while (min_y < max_y)
    {
      if (player->team_id == d_field[min_x][min_y])
	count++;
      min_y++;
    }
    min_x++;
  }
  return (count);
}

static	int	max_tab(int teams[MAX_TEAM_NUM], int team_number)
{
  int		max;
  int		i;

  max = -1;
  i = -1;
  while (++i < MAX_TEAM_NUM)
    if ((i != team_number - 1) && teams[i] > max)
      max = i;
  return (i + 1);
}

int	count_ennemy_atk(unsigned char d_field[HEIGHT][WIDTH], t_player *player,
    			int range)
{
  int	min_x;
  int	min_y;
  int	max_x;
  int	max_y;
  int	teams[MAX_TEAM_NUM];

  bzero(teams, sizeof(int) * MAX_TEAM_NUM);
  min_x = ((player->x - range < 0) ? 0 : player->x - range) - 1;
  min_y = ((player->y - range < 0) ? 0 : player->y - range) - 1;
  max_x = (player->x + range > HEIGHT) ? HEIGHT : player->x + range;
  max_y = (player->y + range > WIDTH) ? WIDTH : player->y + range;
  while (++min_x < max_x)
    while (++min_y < max_y)
      if (d_field[min_x][min_y] != 0)
	teams[d_field[min_x][min_y] - 1]++;
  return (max_tab(teams, player->team_id));
}

int	range(t_player *player, int x, int y)
{
  int	range_x;
  int	range_y;

  range_x = (player->x - x > 0) ? player->x - x : x - player->x;
  range_y = (player->y - y > 0) ? player->y - y : y - player->y;
  return (sqrt(range_x * range_x + range_y * range_y));
}

int	find_ennemy(unsigned char d_field[HEIGHT][WIDTH], t_player *player)
{
  int	min_x;
  int	min_y;
  int	max_x;
  int	max_y;
  int	teams[MAX_TEAM_NUM];

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
      {
	 if (count_ally_atk(d_field, player, range(player, min_x, min_y)) >=
	       count_ennemy_atk(d_field, player, range(player, min_x, min_y)))
	   return (1);
      }
    }
  return (0);
}

int		attack(t_player *player, unsigned char *field, t_ipc_res *ipc)
{
  unsigned char	d_field[HEIGHT][WIDTH];

  create_dfield(field, d_field, ipc);
  return (find_ennemy(d_field, player));
}
