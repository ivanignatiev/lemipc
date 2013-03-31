/*
** test_functions.c for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Sun Mar 31 19:32:56 2013 vincent couvignou
** Last update Sun Mar 31 19:35:13 2013 vincent couvignou
*/

#include "test_functions.h"

int test_down(t_player *player, unsigned char *field, t_ipc_res *ipc_res,
    unsigned char d_field[HEIGHT][WIDTH])
{
  if (go_down(player, d_field) && player->x + 1 < HEIGHT
      && d_field[player->x + 1][player->y] == 0)
    return (move_down(player, field, ipc_res));
  return (0);
}

int test_up(t_player *player, unsigned char *field, t_ipc_res *ipc_res,
    unsigned char d_field[HEIGHT][WIDTH])
{
  if (go_up(player, d_field) && player->x - 1 > 0
      && d_field[player->x - 1][player->y] == 0)
    return (move_up(player, field, ipc_res));
  return (0);
}

int test_left(t_player *player, unsigned char *field, t_ipc_res *ipc_res,
    unsigned char d_field[HEIGHT][WIDTH])
{
  if (go_left(player, d_field) && player->y - 1 > 0
      && d_field[player->x][player->y - 1] == 0)
    return (move_left(player, field, ipc_res));
  return (0);
}

int test_right(t_player *player, unsigned char *field, t_ipc_res *ipc_res,
    unsigned char d_field[HEIGHT][WIDTH])
{
  if (go_right(player, d_field) && player->y + 1 < WIDTH
      && d_field[player->x][player->y + 1] == 0)
    return (move_right(player, field, ipc_res));
  return (0);
}

