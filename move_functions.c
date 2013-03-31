/*
** move_functions.c for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Sun Mar 31 19:29:13 2013 vincent couvignou
** Last update Sun Mar 31 21:21:01 2013 vincent couvignou
*/

#include "move_functions.h"

int		move_right(t_player *player, unsigned char *field,
    			t_ipc_res *ipc_res)
{
  int			decision;

  decision = get_shm_index(player->x, player->y + 1);
  if (field[decision] != 0
      || semctl(ipc_res->sem_id, decision, GETVAL, 0) <= 0)
    return (0);
  lock_sem(ipc_res, player->sh_i);
  field[player->sh_i] = 0;
  unlock_sem(ipc_res, player->sh_i);
  lock_sem(ipc_res, decision);
  player->y += 1;
  player->sh_i = decision;
  field[player->sh_i] = player->team_id;
  unlock_sem(ipc_res, decision);
  return (1);
}

int		move_left(t_player *player, unsigned char *field,
    t_ipc_res *ipc_res)
{
  int			decision;

  decision = get_shm_index(player->x, player->y - 1);
  if (field[decision] != 0
      || semctl(ipc_res->sem_id, decision, GETVAL, 0) <= 0)
    return (0);
  lock_sem(ipc_res, player->sh_i);
  field[player->sh_i] = 0;
  unlock_sem(ipc_res, player->sh_i);
  lock_sem(ipc_res, decision);
  player->y -= 1;
  player->sh_i = decision;
  field[player->sh_i] = player->team_id;
  unlock_sem(ipc_res, decision);
  return (1);
}

int		move_up(t_player *player, unsigned char *field,
    t_ipc_res *ipc_res)
{
  int			decision;

  decision = get_shm_index(player->x - 1, player->y);
  if (field[decision] != 0
      || semctl(ipc_res->sem_id, decision, GETVAL, 0) <= 0)
    return (0);
  lock_sem(ipc_res, player->sh_i);
  field[player->sh_i] = 0;
  unlock_sem(ipc_res, player->sh_i);
  lock_sem(ipc_res, decision);
  player->sh_i = decision;
  player->x -= 1;
  field[player->sh_i] = player->team_id;
  unlock_sem(ipc_res, decision);
  return (1);
}

int		move_down(t_player *player, unsigned char *field,
    t_ipc_res *ipc_res)
{
  int			decision;

  decision = get_shm_index(player->x + 1, player->y);
  if (field[decision] != 0
      || semctl(ipc_res->sem_id, decision, GETVAL, 0) <= 0)
    return (0);
  lock_sem(ipc_res, player->sh_i);
  field[player->sh_i] = 0;
  unlock_sem(ipc_res, player->sh_i);
  lock_sem(ipc_res, decision);
  player->x += 1;
  player->sh_i = decision;
  field[player->sh_i] = player->team_id;
  unlock_sem(ipc_res, decision);
  return (1);
}

