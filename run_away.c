/*
** run_away.c for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Fri Mar 29 13:35:05 2013 vincent couvignou
** Last update Sat Mar 30 14:57:21 2013 ivan ignatiev
*/

# include "run_away.h"

void		create_dfield(unsigned char *field,
    unsigned char d_field[HEIGHT][WIDTH])
{
  int		i;
  int		j;
  int		z;

  while (z < WIDTH * HEIGHT)
  {
    d_field[i][j] = field[z];
    if ((j + 1) % HEIGHT  == 0)
    {
      i++;
      j = 0;
    }
    else
      j++;
    z++;
  }
}
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

int	go_down(t_player *player, unsigned char d_field[HEIGHT][WIDTH])
{
  int	min_x;
  int	min_y;
  int	max_x;
  int	max_y;
  int	teams[MAX_TEAM_NUM];
  int	max_othert;

  bzero(teams, sizeof(int) * MAX_TEAM_NUM);
  min_x = player->x - 1;
  min_y = ((player->y - DEF_RANGE < 0) ? 0 : player->y - DEF_RANGE) - 1;
  max_x = (player->x + DEF_RANGE > HEIGHT) ? HEIGHT : player->x + DEF_RANGE;
  max_y = (player->y + DEF_RANGE > WIDTH) ? WIDTH : player->y + DEF_RANGE;
  while (++min_x < max_x)
    while (++min_y < max_y)
      if (d_field[min_x][min_y] != 0)
	teams[d_field[min_x][min_y] - 1]++;
  min_x = -1;
  max_x = -1;
  while (++min_x < MAX_TEAM_NUM)
    if (min_x != player->team_id - 1 && teams[min_x] > max_x)
      max_x = teams[min_x];
  return ((teams[player->team_id - 1] >= max_x) ? true : false);
}

int	go_up(t_player *player, unsigned char d_field[HEIGHT][WIDTH])
{
  int	min_x;
  int	min_y;
  int	max_x;
  int	max_y;
  int	teams[MAX_TEAM_NUM];
  int	max_othert;

  bzero(teams, sizeof(int) * MAX_TEAM_NUM);
  min_x = ((player->x - DEF_RANGE < 0) ? 0 : player->x - DEF_RANGE) - 1;
  min_y = ((player->y - DEF_RANGE < 0) ? 0 : player->y - DEF_RANGE) - 1;
  max_x = player->x;
  max_y = (player->y + DEF_RANGE > WIDTH) ? WIDTH : player->y + DEF_RANGE;
  while (++min_x < max_x)
    while (++min_y < max_y)
      if (d_field[min_x][min_y] != 0)
	teams[d_field[min_x][min_y] - 1]++;
  min_x = -1;
  max_x = -1;
  while (++min_x < MAX_TEAM_NUM)
    if (min_x != player->team_id - 1 && teams[min_x] > max_x)
      max_x = teams[min_x];
  return ((teams[player->team_id - 1] >= max_x) ? true : false);
}

int	go_left(t_player *player, unsigned char d_field[HEIGHT][WIDTH])
{
  int	min_x;
  int	min_y;
  int	max_x;
  int	max_y;
  int	teams[MAX_TEAM_NUM];
  int	max_othert;

  bzero(teams, sizeof(int) * MAX_TEAM_NUM);
  min_x = ((player->x - DEF_RANGE < 0) ? 0 : player->x - DEF_RANGE) - 1;
  min_y = ((player->y - DEF_RANGE < 0) ? 0 : player->y - DEF_RANGE) - 1;
  max_x = (player->x + DEF_RANGE > HEIGHT) ? HEIGHT : player->x + DEF_RANGE;
  max_y = player->y;
  while (++min_x < max_x)
    while (++min_y < max_y)
      if (d_field[min_x][min_y] != 0)
	teams[d_field[min_x][min_y] - 1]++;
  min_x = -1;
  max_x = -1;
  while (++min_x < MAX_TEAM_NUM)
    if (min_x != player->team_id - 1 && teams[min_x] > max_x)
      max_x = teams[min_x];
  return ((teams[player->team_id - 1] >= max_x) ? true : false);
}

int	go_right(t_player *player, unsigned char d_field[HEIGHT][WIDTH])
{
  int	min_x;
  int	min_y;
  int	max_x;
  int	max_y;
  int	teams[MAX_TEAM_NUM];
  int	max_othert;

  bzero(teams, sizeof(int) * MAX_TEAM_NUM);
  min_x = ((player->x - DEF_RANGE < 0) ? 0 : player->x - DEF_RANGE) - 1;
  min_y = player->y;
  max_x = (player->x + DEF_RANGE > HEIGHT) ? HEIGHT : player->x + DEF_RANGE;
  max_y = (player->y + DEF_RANGE > WIDTH) ? WIDTH : player->y + DEF_RANGE;
  while (++min_x < max_x)
    while (++min_y < max_y)
      if (d_field[min_x][min_y] != 0)
	teams[d_field[min_x][min_y] - 1]++;
  min_x = -1;
  max_x = -1;
  while (++min_x < MAX_TEAM_NUM)
    if (min_x != player->team_id - 1 && teams[min_x] > max_x)
      max_x = teams[min_x];
  return ((teams[player->team_id - 1] >= max_x) ? true : false);
}

int		run_away(t_player *player, unsigned char *field)
{
  unsigned char	d_field[HEIGHT][WIDTH];
  int		nb_allies;
  int		nb_ennemies;

  create_dfield(field, d_field);
  nb_allies = count_ally(d_field, player->x, player->y, player->team_id);
  nb_ennemies = count_ennemy(d_field, player->x, player->y, player->team_id);
  if (nb_allies > nb_ennemies)
    return (0);
  if (go_up(player, d_field) && player->x - 1 > 0)
    return (1);
  else if (go_right(player, d_field) && player->y + 1 < WIDTH)
    return (1);
  else if (go_left(player, d_field) && player->y - 1 > 0)
    return (1);
  else if (go_down(player, d_field) && player->x + 1 < HEIGHT)
    return (1);
  return (0);
}
