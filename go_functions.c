/*
** go_functions.c for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Sun Mar 31 19:26:17 2013 vincent couvignou
** Last update Sun Mar 31 19:28:30 2013 vincent couvignou
*/

#include "go_functions.h"

int	go_down(t_player *player, unsigned char d_field[HEIGHT][WIDTH])
{
  int	min_x;
  int	min_y;
  int	max_x;
  int	max_y;
  int	teams[MAX_TEAM_NUM];

  bzero(teams, sizeof(int) * MAX_TEAM_NUM);
  if (player->x == HEIGHT)
    return (false);
  min_x = player->x;
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

  bzero(teams, sizeof(int) * MAX_TEAM_NUM);
  if (player->x == 0)
    return (false);
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

  bzero(teams, sizeof(int) * MAX_TEAM_NUM);
  if (player->y == 0)
    return (false);
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

  bzero(teams, sizeof(int) * MAX_TEAM_NUM);
  if (player->y == WIDTH)
    return (false);
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

