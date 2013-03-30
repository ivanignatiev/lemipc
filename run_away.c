/*
** run_away.c for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Fri Mar 29 13:35:05 2013 vincent couvignou
** Last update Sat Mar 30 19:08:56 2013 vincent couvignou
*/

# include "run_away.h"
#include "lemipc.h"

unsigned char debug_field[HEIGHT][WIDTH];

void	print_field(unsigned char d_field[HEIGHT][WIDTH])
{
  int	i = -1;
  int	j = -1;

  while (++i < HEIGHT)
  {
    j = -1;
    while (++j < WIDTH)
     printf("%d ", d_field[i][j]);
    printf("\n");
  }
  printf("\n\n");
  sleep(2);
}

void		create_dfield(unsigned char *field,
    unsigned char d_field[HEIGHT][WIDTH], t_ipc_res *res)
{
  int		i;
  int		j;
  int		z;

  i = 0;
  j = 0;
  z = 0;
  while (z < WIDTH * HEIGHT)
  {
    lock_sem(res, z);
    d_field[i][j] = field[z];
    unlock_sem(res, z);
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

static int		move_right(t_player *player, unsigned char *field,
    			t_ipc_res *ipc_res)
{
  int			previous_sh_i;

  print_field(debug_field);
  previous_sh_i = player->sh_i;
  lock_sem(ipc_res, player->sh_i);
  printf("right : player[%d][%d]\t", player->x, player->y);
  field[player->sh_i] = 0;
  unlock_sem(ipc_res, player->sh_i);
  player->sh_i = get_shm_cell(ipc_res, player->x, player->y + 1, field);
  player->y += 1;
  player->sh_i = get_shm_index(player->x, player->y);
  lock_sem(ipc_res, previous_sh_i);
  lock_sem(ipc_res, player->sh_i);
  field[player->sh_i] = player->team_id;
  printf("player[%d][%d]\n", player->x, player->y);
  unlock_sem(ipc_res, player->sh_i);
  unlock_sem(ipc_res, previous_sh_i);
  return (1);
}

static int		move_left(t_player *player, unsigned char *field,
    			t_ipc_res *ipc_res)
{
  int			previous_sh_i;

  print_field(debug_field);
  previous_sh_i = player->sh_i;
  player->sh_i = get_shm_cell(ipc_res, player->x, player->y - 1, field);
  printf("left : player[%d][%d]\t", player->x, player->y);
  player->y -= 1;
  player->sh_i = get_shm_index(player->x, player->y);
  lock_sem(ipc_res, previous_sh_i);
  lock_sem(ipc_res, player->sh_i);
  field[previous_sh_i] = 0;
  field[player->sh_i] = player->team_id;
  printf("player[%d][%d]\n", player->x, player->y);
  unlock_sem(ipc_res, player->sh_i);
  unlock_sem(ipc_res, previous_sh_i);
  return (1);
}

static int		move_up(t_player *player, unsigned char *field,
    			t_ipc_res *ipc_res)
{
  int			previous_sh_i;

  print_field(debug_field);
  previous_sh_i = player->sh_i;
  field[player->sh_i] = 0;
  printf("up : player[%d][%d]\t", player->x, player->y);
  unlock_sem(ipc_res, player->sh_i);
  player->sh_i = get_shm_cell(ipc_res, player->x - 1, player->y, field);
  player->x -= 1;
  player->sh_i = get_shm_index(player->x, player->y);
  lock_sem(ipc_res, previous_sh_i);
  lock_sem(ipc_res, player->sh_i);
  field[player->sh_i] = player->team_id;
  printf("player[%d][%d]\n", player->x, player->y);
  unlock_sem(ipc_res, player->sh_i);
  unlock_sem(ipc_res, previous_sh_i);
  return (1);
}

static int		move_down(t_player *player, unsigned char *field,
    			t_ipc_res *ipc_res)
{
  int			previous_sh_i;

  print_field(debug_field);
  previous_sh_i = player->sh_i;
  field[player->sh_i] = 0;
  printf("down : player[%d][%d]\t", player->x, player->y);
  unlock_sem(ipc_res, player->sh_i);
  player->sh_i = get_shm_cell(ipc_res, player->x + 1, player->y, field);
  player->x += 1;
  player->sh_i = get_shm_index(player->x, player->y);
  lock_sem(ipc_res, previous_sh_i);
  lock_sem(ipc_res, player->sh_i);
  field[player->sh_i] = player->team_id;
  printf("player[%d][%d]\n", player->x, player->y);
  unlock_sem(ipc_res, player->sh_i);
  unlock_sem(ipc_res, previous_sh_i);
  return (1);
}


int		run_away(t_player *player, unsigned char *field,
    			t_ipc_res *ipc_res)
{
  unsigned char	d_field[HEIGHT][WIDTH];
  int		nb_allies;
  int		nb_ennemies;

  create_dfield(field, d_field, ipc_res);
  create_dfield(field, debug_field, ipc_res);
  nb_allies = count_ally(d_field, player->x, player->y, player->team_id);
  nb_ennemies = count_ennemy(d_field, player->x, player->y, player->team_id);
  sleep(2);
  if (nb_allies > nb_ennemies)
    return (0);
  if (go_up(player, d_field) && player->x - 1 > 0)
    return (move_up(player, field, ipc_res));
  else if (go_right(player, d_field) && player->y + 1 < WIDTH)
    return (move_right(player, field, ipc_res));
  else if (go_left(player, d_field) && player->y - 1 > 0)
    return (move_left(player, field, ipc_res));
  else if (go_down(player, d_field) && player->x + 1 < HEIGHT)
    return (move_down(player, field, ipc_res));
  return (0);
}
