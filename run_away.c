/*
** run_away.c for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Fri Mar 29 13:35:05 2013 vincent couvignou
** Last update Sun Mar 31 19:38:19 2013 vincent couvignou
*/

#include "run_away.h"
#include "lemipc.h"
#include "lemipc_structures.h"
#include "test_functions.h"
#include "create_field.h"

int  (*t_fct[4])();


int	count_ally(unsigned char d_field[HEIGHT][WIDTH],
    	int position_x, int position_y, int team_number)
{
  int	min_x;
  int	min_y;
  int	max_x;
  int	max_y;
  int	count;

  min_x = (position_x - DEF_RANGE < 0) ? 0 : position_x - DEF_RANGE;
  min_y = (position_y - DEF_RANGE < 0) ? 0 : position_y - DEF_RANGE;
  max_x = (position_x + DEF_RANGE > HEIGHT) ? HEIGHT : position_x + DEF_RANGE;
  max_y = (position_y + DEF_RANGE > WIDTH) ? WIDTH : position_y + DEF_RANGE;
  count = 0;
  while (min_x < max_x)
  {
    while (min_y < max_y)
    {
      if (team_number == d_field[min_x][min_y])
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

int	count_ennemy(unsigned char d_field[HEIGHT][WIDTH],
    int position_x, int position_y, int team_number)
{
  int	min_x;
  int	min_y;
  int	max_x;
  int	max_y;
  int	teams[MAX_TEAM_NUM];

  bzero(teams, sizeof(int) * MAX_TEAM_NUM);
  min_x = ((position_x - DEF_RANGE < 0) ? 0 : position_x - DEF_RANGE) - 1;
  min_y = ((position_y - DEF_RANGE < 0) ? 0 : position_y - DEF_RANGE) - 1;
  max_x = (position_x + DEF_RANGE > HEIGHT) ? HEIGHT : position_x + DEF_RANGE;
  max_y = (position_y + DEF_RANGE > WIDTH) ? WIDTH : position_y + DEF_RANGE;
  while (++min_x < max_x)
    while (++min_y < max_y)
      if (d_field[min_x][min_y] != 0)
	teams[d_field[min_x][min_y] - 1]++;
  return (max_tab(teams, team_number));
}



bool		test_random(t_player *player, unsigned char *field, t_ipc_res *ipc_res,
    unsigned char d_field[HEIGHT][WIDTH])

{
  int		i;
  int		value;

  t_fct[0] = &test_up;
  t_fct[1] = &test_down;
  t_fct[2] = &test_right;
  t_fct[3] = &test_left;
  while (++i < 4)
  {
    value = rand() % 4;
    while (t_fct[value] == NULL)
      value = rand() % 4;
    if (t_fct[value](player, field, ipc_res, d_field))
      return (true);
    t_fct[value] = NULL;
  }
  return (false);
}

int		run_away(t_player *player, unsigned char *field,
    			t_ipc_res *ipc_res)
{
  unsigned char	d_field[HEIGHT][WIDTH];
  int		nb_allies;
  int		nb_ennemies;

  create_dfield(field, d_field, ipc_res);
  nb_allies = count_ally(d_field, player->x, player->y, player->team_id);
  nb_ennemies = count_ennemy(d_field, player->x, player->y, player->team_id);
  sleep(2);
  if (nb_allies > nb_ennemies)
    return (0);
  return (test_random(player, field, ipc_res, d_field));
}
