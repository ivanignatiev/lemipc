/*
** lemipc.c for lemipc in /home/ignati_i/projects/lemipc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Mon Mar 25 15:30:47 2013 ivan ignatiev
** Last update Sat Mar 30 20:53:25 2013 ivan ignatiev
*/

#include	"lemipc.h"

int		count_players(t_ipc_res *ipc_res,
			      unsigned char *field)
{
  int		count;
  int		i;

  count = 0;
  i = 0;
  while (i < WIDTH * HEIGHT)
    {
      lock_sem(ipc_res, i);
      if (field[i] > 0)
	count = count + 1;
      unlock_sem(ipc_res, i);
      i = i + 1;
    }
  return (count);
}

void		clear_ressources(t_ipc_res *ipc_res)
{
  shmctl(ipc_res->field_id, IPC_RMID, NULL);
  semctl(ipc_res->sem_id, 0, IPC_RMID, NULL);
  msgctl(ipc_res->msg_id, IPC_RMID, NULL);
}

int		count_players_in_team(t_ipc_res *ipc_res, t_player *player,
				      unsigned char *field)
{
  int		count;
  int		i;

  count = -1;
  i = 0;
  while (i < WIDTH * HEIGHT)
    {
      lock_sem(ipc_res, i);
      if (field[i] == player->team_id)
	count = count + 1;
      unlock_sem(ipc_res, i);
      i = i + 1;
    }
  return (count);
}

int		send_msg_to_team(t_ipc_res *ipc_res, t_player *player,
				 int count, const char *msg)
{
  t_msg		snd_msg;
  int		player_num;
  int		last_num;

  bzero(&snd_msg, sizeof(t_msg));
  strcpy(snd_msg.msg, msg);
  player_num = player->team_id * MAX_TEAM_NUM;
  last_num = count + player_num;
  while (player_num <= last_num)
    {
      if (player_num != player->num)
	{
	  snd_msg.mtype = player_num;
	  msgsnd(ipc_res->msg_id, (const void *)&snd_msg, sizeof(t_msg), 0);
	}
      player_num = player_num + 1;
    }
  return (0);
}

int		send_message_to_player(t_ipc_res *ipc_res, t_player *player,
				       int player_num, const char *msg)
{
  t_msg		snd_msg;

  (void)player;
  bzero(&snd_msg, sizeof(t_msg));
  strcpy(snd_msg.msg, msg);
  snd_msg.mtype = player_num;
  return (msgsnd(ipc_res->msg_id, (const void *)&snd_msg, sizeof(t_msg), 0));
}

int		recv_msg_from_team(t_ipc_res *ipc_res, t_player *player, t_msg *msg)
{
  return (msgrcv(ipc_res->msg_id, (void*)msg, sizeof(t_msg), player->num, IPC_NOWAIT));
}

void		lock_sem(t_ipc_res *ipc_res, int i)
{
  struct sembuf	sem;

  sem.sem_num = i;
  sem.sem_flg = 0;
  sem.sem_op = -1;
  semop(ipc_res->sem_id, &sem, 1);
}

void		unlock_sem(t_ipc_res *ipc_res, int i)
{
  struct sembuf	sem;

  sem.sem_num = i;
  sem.sem_flg = 0;
  sem.sem_op = 1;
  semop(ipc_res->sem_id, &sem, 1);
}

int		get_shm_index(int x, int y)
{
  int		sh_i;

  sh_i = (y * WIDTH + x);
  if (sh_i >= (WIDTH * HEIGHT) || sh_i < 0)
    return (-1);
  return (sh_i);
}

int		get_shm_cell(t_ipc_res *ipc_res, int x, int y,
			     unsigned char *field)
{
  int		sh_i;
  int		value;

  sh_i = (y * WIDTH + x);
  if (sh_i >= (WIDTH * HEIGHT) || sh_i < 0)
    return (-1);
  lock_sem(ipc_res, sh_i);
  value = field[sh_i];
  unlock_sem(ipc_res, sh_i);
  return (value);
}

void		clear_player(t_ipc_res *ipc_res, t_player *player,
			     unsigned char *field)
{
  if (player->sh_i >= 0)
    {
      lock_sem(ipc_res, player->sh_i);
      field[player->sh_i] = 0;
      unlock_sem(ipc_res, player->sh_i);
    }
}

void		place_player(t_ipc_res *ipc_res, t_player *player,
			     unsigned char *field)
{
  char		move_msg[100];
  int           sh_i;

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
  sprintf(move_msg, "MOVE:%d:%d:%d:%d", player->num, player->x, player->y, player->sh_i);
  send_msg_to_team(ipc_res, player, count_players_in_team(ipc_res, player, field), move_msg);
}

int		count_aliens(int my_team, int *around)
{
  int		i;
  int		j;
  int		count;

  i = 0;
  while (i < 8)
    {
      j = 0;
      count = 0;
      while (j < 8)
	{
	  if (around[j] == around[i] && around[j] != my_team && around[j] > 0)
	    count = count + 1;
	  j = j + 1;
	}
      if (count >= 2)
	return (around[i]);
      i = i + 1;
    }
  return (0);
}

int		player_kill(t_ipc_res *ipc_res, t_player *player, unsigned char *field)
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
  if (kill_team = count_aliens(player->team_id, around))
    {
      clear_player(ipc_res, player, field);
      sprintf(die_msg, "DIEP:%d", player->num);
      send_msg_to_team(ipc_res, player, count_players_in_team(ipc_res, player, field), die_msg);
      printf("Player %d : Team %d killed me !!!!! \n", player->num, kill_team);
      return (1);
    }
  return (0);
}

void		parse_message(t_ipc_res *ipc_res, t_player *player,
			      const char *msg, t_fct_messages *p_fct)
{
    int		i;

    i = 0;
    while (strncmp(msg, p_fct[i].name, strlen(p_fct[i].name)) && i < NB_KIND)
      i++;
    if (i != NB_KIND)
      p_fct[i].p_fct(ipc_res, player, msg);
}

void		ressources_info(t_ipc_res *ipc_res)
{
  printf("FIELD ID : %d\n", ipc_res->field_id);
  printf("SEM ID : %d\n", ipc_res->sem_id);
  printf("MSG ID : %d\n", ipc_res->msg_id);
}

int		player_preplace(t_ipc_res *ipc_res, t_player *player,
				unsigned char *field)
{
  char		present_msg[100];

  player->num = count_players_in_team(ipc_res, player, field) + 1;
  if (player->num >= MAX_TEAM_NUM || count_players(ipc_res, field) >= WIDTH * HEIGHT)
    {
      fprintf(stderr, "You're late, team doesn't love you -21\n");
      return (0);
    }
  player->num += (player->team_id * MAX_TEAM_NUM);
  printf("PLAYER NUM : %d\n", player->num);
  sprintf(present_msg, "NEWP:%d", player->num);
  send_msg_to_team(ipc_res, player, count_players_in_team(ipc_res, player, field) + 1, present_msg);
  srand(time(NULL) + player->num);
  place_player(ipc_res, player, field);
  return (1);
}

int		player_die(t_ipc_res *ipc_res, t_player *player,
			   unsigned char *field)
{
  int		cnt_pl_in_team;
  int		cnt_pl;

  if (player_kill(ipc_res, player, field))
    {
      clear_player(ipc_res, player, field);
      return (1);
    }
  cnt_pl_in_team = count_players_in_team(ipc_res, player, field) + 1;
  cnt_pl =  count_players(ipc_res, field);
  printf("Player %d : pl_in_t : %d / %d\n", player->num, cnt_pl_in_team, cnt_pl);
  if (cnt_pl_in_team == cnt_pl)
    {
      printf("Player %d : Team %ld won!\n", player->num, player->team_id);
      clear_player(ipc_res, player, field);
      clear_ressources(ipc_res);
      return (1);
    }
  if (cnt_pl_in_team == 1)
    {
      printf("Player %d : I can't kill them, because I'm single"
         " I've just kill my self.\n", player->num);
      clear_player(ipc_res, player, field);
      return (1);
    }
  return (0);
}

int			slave_process(t_ipc_res *ipc_res, t_player *player,
				      unsigned char *field)
{
  t_msg			ipc_msg;
  int			msg_size;
  t_fct_messages	p_fct[NB_KIND];

  init_fct(p_fct);
  printf("Player %d : Waiting for battle!\n", player->num);
  lock_sem(ipc_res, AVAILBL_PLACES_SEM);
  printf("Player %d : Battle begun!\n", player->num);
  while (1)
    {

      if (!run_away(player, field, ipc_res))
      {
	printf("Random move %d[%d]!!\n", player->team_id, player->num);
	random_move(player, field, ipc_res);
      }
      if ((msg_size = recv_msg_from_team(ipc_res, player, &ipc_msg)) > 0)
	parse_message(ipc_res, player, ipc_msg.msg, p_fct);
      else if (msg_size == -1 && errno != ENOMSG)
	{
	  fprintf(stderr, "Shared ressources closed\n");
	  return (EXIT_FAILURE);
	}
      if (player_die(ipc_res, player, field))
	return (EXIT_SUCCESS);
      sleep(2);
    }
  return (EXIT_SUCCESS);
}

int		battle_begini_ctl(t_ipc_res *ipc_res, unsigned char *field)
{
  int		i;

  i = FIGTH_TIMEOUT;
  while (i > 0)
  {
    printf("%d sec. to battle!\n", i);
    sleep(1);
    i = i - 1;
  }
  printf("Fight!\n");
  while (i < WIDTH * HEIGHT)
  {
    unlock_sem(ipc_res, AVAILBL_PLACES_SEM);
    i = i + 1;
  }
  return (count_players(ipc_res, field));
}

void		sems_init(t_ipc_res *ipc_res)
{
  int		i;

  i = 0;
  while (i < WIDTH * HEIGHT)
    {
      semctl(ipc_res->sem_id, i, SETVAL, 0);
      unlock_sem(ipc_res, i);
      i = i + 1;
    }
  semctl(ipc_res->sem_id, AVAILBL_PLACES_SEM, SETVAL, 1);
  lock_sem(ipc_res, AVAILBL_PLACES_SEM);
}

int		master_process(t_ipc_res *ipc_res, t_player *player)
{
  unsigned char	*field;

  if ((field = (unsigned char*)shmat(ipc_res->field_id, NULL, SHM_R | SHM_W)) == NULL)
    return (EXIT_FAILURE);
  memset(field, 0, WIDTH * HEIGHT);
  sems_init(ipc_res);
  ressources_info(ipc_res);
  if (!player_preplace(ipc_res, player, field)
      || !battle_begini_ctl(ipc_res, field))
    {
      printf("Game can't begin!\n");
      clear_ressources(ipc_res);
      return (EXIT_FAILURE);
    }
  return (slave_process(ipc_res, player, field));
}

int		run_game(key_t key, t_player *player)
{
  t_ipc_res	ipc_res;
  unsigned char	*field;

  if ((ipc_res.field_id = shmget(key, WIDTH * HEIGHT, SHM_R | SHM_W)) != -1)
    {
      if ((ipc_res.sem_id = semget(key, WIDTH * HEIGHT + 1, SHM_R | SHM_W)) == -1
	  || (ipc_res.msg_id = msgget(key, SHM_R | SHM_W)) == -1)
        return (EXIT_FAILURE);
      if ((field = (unsigned char*)shmat(ipc_res.field_id, NULL, SHM_R | SHM_W)) == NULL)
	return (EXIT_FAILURE);
      ressources_info(&ipc_res);
      if (!player_preplace(&ipc_res, player, field))
	return (EXIT_FAILURE);
      return (slave_process(&ipc_res, player, field));
    }
  else
    {
      if ((ipc_res.field_id = shmget(key, WIDTH * HEIGHT, IPC_CREAT | SHM_R | SHM_W)) == -1
	  || (ipc_res.sem_id = semget(key, WIDTH * HEIGHT + 1, IPC_CREAT | SHM_R | SHM_W)) == -1
	  || (ipc_res.msg_id = msgget(key, IPC_CREAT | SHM_R | SHM_W)) == -1)
	return (EXIT_FAILURE);
      return (master_process(&ipc_res, player));
    }
  return (EXIT_SUCCESS);
}

int		main(int argc, char **argv)
{
  char		*pwd;
  key_t		key;
  t_player	player;

  if (argc < 2)
    {
      fprintf(stderr, "Too few arguments. Use : ./lemipc [team_number]\n");
      return (EXIT_FAILURE);
    }
  player.player_list = new_list_default();
  player.team_id = atoi(argv[1]);
  player.sh_i = -1;
  if ((pwd = getenv("PWD")) == NULL)
    return (EXIT_FAILURE);
  if ((key = ftok(pwd, 1)) >= 0)
    return (run_game(key, &player));
  else
    return (EXIT_FAILURE);
}
