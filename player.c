/*
** player.c for lemipc in /home/ignati_i//projects/lemipc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sun Mar 31 19:25:31 2013 ivan ignatiev
** Last update Sun Mar 31 19:26:31 2013 ivan ignatiev
*/

#include	"lemipc.h"

void		clear_player(t_ipc_res *ipc_res, t_player *player,
			     t_uchar *field)
{
  if (player->sh_i >= 0)
    {
      lock_sem(ipc_res, player->sh_i);
      field[player->sh_i] = 0;
      unlock_sem(ipc_res, player->sh_i);
    }
}

void		place_player(t_ipc_res *ipc_res, t_player *player,
			     t_uchar *field)
{
  int		sh_i;

  player->x = rand() % WIDTH;
  player->y = rand() % HEIGHT;
  sh_i = get_shm_index(player->x, player->y);
  while (field[sh_i] != 0 && semctl(ipc_res->sem_id, sh_i, GETVAL) > 0)
    {
      player->x = rand() % WIDTH;
      player->y = rand() % HEIGHT;
      sh_i = get_shm_index(player->x, player->y);
    }
  clear_player(ipc_res, player, field);
  lock_sem(ipc_res, sh_i);
  player->sh_i = sh_i;
  field[player->sh_i] = player->team_id;
  unlock_sem(ipc_res, sh_i);
}

int		player_kill(t_ipc_res *ipc_res, t_player *player,
			    t_uchar *field)
{
  int		around[8];
  char		die_msg[100];
  int		kill_team;

  around[0] = get_shm_cell(ipc_res, player->x - 1, player->y - 1, field);
  around[1] = get_shm_cell(ipc_res, player->x, player->y - 1, field);
  around[2] = get_shm_cell(ipc_res, player->x + 1, player->y - 1, field);
  around[3] = get_shm_cell(ipc_res, player->x - 1, player->y, field);
  around[4] = get_shm_cell(ipc_res, player->x + 1, player->y, field);
  around[5] = get_shm_cell(ipc_res, player->x - 1, player->y + 1, field);
  around[6] = get_shm_cell(ipc_res, player->x, player->y + 1, field);
  around[7] = get_shm_cell(ipc_res, player->x + 1, player->y + 1, field);
  if ((kill_team = count_aliens(player->team_id, around)))
    {
      clear_player(ipc_res, player, field);
      sprintf(die_msg, "DIEP:%d", player->num);
      send_msg_to_team(ipc_res, player,
		       count_players_in_team(ipc_res, player, field), die_msg);
      printf("Player %d : Team %d killed me !!!!! \n", player->num, kill_team);
      return (1);
    }
  return (0);
}

int		player_preplace(t_ipc_res *ipc_res, t_player *player,
				t_uchar *field)
{
  char		present_msg[100];

  player->num = count_players_in_team(ipc_res, player, field) + 1;
  place_player(ipc_res, player, field);
  if (player->num >= MAX_TEAM_NUM
      || count_players(ipc_res, field) >= (WIDTH * HEIGHT) / 2)
    {
      fprintf(stderr, "You're late, team doesn't love you -21\n");
      return (0);
    }
  player->num += (player->team_id * MAX_TEAM_NUM);
  printf("PLAYER NUM : %d\n", player->num);
  sprintf(present_msg, "NEWP:%d:%d:%d",
	  player->num, player->x, player->y);
  send_msg_to_team(ipc_res, player,
		   count_players_in_team(ipc_res, player, field) + 1,
		   present_msg);
  srand(time(NULL) + player->num);
  return (1);
}

int		player_die(t_ipc_res *ipc_res, t_player *player,
			   t_uchar *field)
{
  int		cnt_pl_in_team;
  int		cnt_pl;

  cnt_pl_in_team = count_players_in_team(ipc_res, player, field) + 1;
  cnt_pl =  count_players(ipc_res, field);
  if (cnt_pl_in_team == cnt_pl)
  {
      printf("Player %d : Team %ld won!\n", player->num, player->team_id);
      clear_player(ipc_res, player, field);
      clear_ressources(ipc_res);
      return (1);
  }
  if (cnt_pl_in_team <= 1)
  {
    printf("Player %d : I can't kill them, because I'm single"
        " I've just kill my self.\n", player->num);
    clear_player(ipc_res, player, field);
    return (1);
  }
  if (player_kill(ipc_res, player, field))
    return (1);
  return (0);
}
