/*
** lemipc.c for lemipc in /home/ignati_i/projects/lemipc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Mon Mar 25 15:30:47 2013 ivan ignatiev
** Last update Thu Mar 28 12:16:27 2013 ivan ignatiev
*/

#include "lemipc.h"

static t_fct_messages p_fct[NB_KIND];

void		display_field(unsigned char *field)
{
  int		i;

  i = 0;
  while (i < WIDTH * HEIGHT)
    {
      printf("%u ", field[i]);
      if ((i + 1) % WIDTH == 0)
	printf("\n");
      i = i + 1;
    }
}

int		count_players(unsigned char *field)
{
  int		count;
  int		i;

  count = 0;
  i = 0;
  while (i < WIDTH * HEIGHT)
  {
    if (field[i] > 0)
      count = count + 1;
    i = i + 1;
  }
  return (count);
}

void            clear_ressources(t_ipc_res *ipc_res)
{
  shmctl(ipc_res->field_id, IPC_RMID, NULL);
  semctl(ipc_res->sem_id, 0, IPC_RMID, NULL);
  msgctl(ipc_res->msg_id, IPC_RMID, NULL);
}

int		count_players_in_team(t_player *player, unsigned char *field)
{
  int		count;
  int		i;

  count = -1;
  i = 0;
  while (i < WIDTH * HEIGHT)
  {
    if (field[i] == player->team_id)
      count = count + 1;
    i = i + 1;
  }
  return (count);
}

int		send_msg_to_team(t_ipc_res *ipc_res, t_player *player, int count, const char *msg)
{
  t_msg		snd_msg;
  int           player_num;
  int           last_num;

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

int             send_message_to_player(t_ipc_res *ipc_res, t_player *player, int player_num, const char *msg)
{
  t_msg		snd_msg;

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
  struct sembuf sem;

  sem.sem_num = i;
  sem.sem_flg = 0;
  sem.sem_op = -1;
  semop(ipc_res->sem_id, &sem, 1);
}

void		unlock_sem(t_ipc_res *ipc_res, int i)
{
  struct sembuf sem;
  int           avsem;

  sem.sem_num = i;
  sem.sem_flg = 0;
  sem.sem_op = 1;
  semop(ipc_res->sem_id, &sem, 1);
}

int             get_shm_index(int x, int y)
{
  int           sh_i;

  sh_i = (y * WIDTH + x);
  if (sh_i >= (WIDTH * HEIGHT) || sh_i < 0)
    return (-1);
  return (sh_i);
}

int             get_shm_cell(int x, int y, unsigned char *field)
{
  int           sh_i;

  sh_i = (y * WIDTH + x);
  if (sh_i >= (WIDTH * HEIGHT) || sh_i < 0)
    return (-1);
  return (field[sh_i]);
}

void		place_player(t_ipc_res *ipc_res, t_player *player, unsigned char *field)
{
  char		move_msg[100];

  if (player->sh_i >= 0)
    {
      lock_sem(ipc_res, player->sh_i);
      field[player->sh_i] = 0;
      unlock_sem(ipc_res, player->sh_i);
    }
  player->x = rand() % WIDTH;
  player->y = rand() % HEIGHT;
  player->sh_i = get_shm_index(player->x, player->y);
  while (field[player->sh_i] != 0 && semctl(ipc_res->sem_id, 0, GETVAL) > 0)
    {
      player->x = rand() % WIDTH;
      player->y = rand() % HEIGHT;
      player->sh_i = get_shm_index(player->x, player->y);
    }
  lock_sem(ipc_res, player->sh_i);
  field[player->sh_i] = player->team_id;
  sprintf(move_msg, "MOVE:%d:%d:%d:%d", player->num, player->x, player->y, player->sh_i);
  send_msg_to_team(ipc_res, player, count_players_in_team(player, field), move_msg);
  unlock_sem(ipc_res, player->sh_i);
}

int             count_aliens(int my_team, int *around)
{
  int           i;
  int           j;
  int           count;

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
      return (1);
    i = i + 1;
  }
  return (0);
}

int             player_kill(t_ipc_res *ipc_res, t_player *player, unsigned char *field)
{
  int           around[8];
  char          die_msg[100];

  around[0] =  get_shm_cell(player->x - 1, player->y - 1, field);
  around[1] =  get_shm_cell(player->x, player->y - 1, field);
  around[2] =  get_shm_cell(player->x + 1, player->y - 1, field);
  around[3] =  get_shm_cell(player->x - 1, player->y, field);
  around[4] =  get_shm_cell(player->x + 1, player->y, field);
  around[5] =  get_shm_cell(player->x - 1, player->y + 1, field);
  around[6] =  get_shm_cell(player->x, player->y + 1, field);
  around[7] =  get_shm_cell(player->x + 1, player->y + 1, field);
  if (count_aliens(player->team_id, around))
  {
    lock_sem(ipc_res, player->sh_i);
    field[player->sh_i] = 0;
    unlock_sem(ipc_res, player->sh_i);
    sprintf(die_msg, "DIEP:%d", player->num);
    send_msg_to_team(ipc_res, player, count_players_in_team(player, field), die_msg);
    printf("They killed me!\n");
    return (1);
  }
  return (0);
}

void            parse_message(t_ipc_res *ipc_res, t_player *player, const char *msg)
{
    int         i;

    i = 0;
    while (strncmp(msg, p_fct[i].name, strlen(p_fct[i].name)) && i < NB_KIND)
      i++;
    if (i != NB_KIND)
      p_fct[i].p_fct(ipc_res, player, msg);
}

int		slave_process(t_ipc_res *ipc_res, t_player *player)
{
  t_msg		ipc_msg;
  int		msg_size;
  unsigned char *field;
  char		present_msg[100];
  int           sem;

  if ((field = (unsigned char*)shmat(ipc_res->field_id, NULL, SHM_R | SHM_W)) == NULL)
    return (EXIT_FAILURE);
  printf("SLAVE\n");
  printf("FIELD ID : %d\n", ipc_res->field_id);
  printf("SEM ID : %d\n", ipc_res->sem_id);
  printf("MSG ID : %d\n", ipc_res->msg_id);
  player->num = count_players_in_team(player, field) + 1;
  if (player->num >= MAX_TEAM_NUM || count_players(field) >= WIDTH * HEIGHT)
  {
    fprintf(stderr, "You're late, team doesn't love you -21\n");
    return (EXIT_FAILURE);
  }
  player->num += (player->team_id * MAX_TEAM_NUM);
  printf("PLAYER NUM : %d\n", player->num);
  sprintf(present_msg, "NEWP:%d", player->num);
  send_msg_to_team(ipc_res, player, count_players_in_team(player, field) + 1, present_msg);
  place_player(ipc_res, player, field);
  printf("Waiting for battle!\n");
  lock_sem(ipc_res, AVAILBL_PLACES_SEM);
  printf("Battle begun!\n");
  while (1)
    {
     if ((msg_size = recv_msg_from_team(ipc_res, player, &ipc_msg)) > 0)
	parse_message(ipc_res, player, ipc_msg.msg);
      else if (msg_size == -1 && errno != ENOMSG)
      {
        fprintf(stderr, "Shared ressources closed\n");
	return (EXIT_FAILURE);
      }
     if (player_kill(ipc_res, player, field))
        return (EXIT_SUCCESS);
      if ((count_players_in_team(player, field) + 1) == count_players(field))
      {
        printf("Team %d won!\n", player->team_id);
        return (EXIT_SUCCESS);
      }
      usleep(10000);
    }
  return (EXIT_SUCCESS);
}

int             battle_begini_ctl(t_ipc_res *ipc_res, unsigned char *field)
{
  t_msg         recv_msg;
  int           i;

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
  return (count_players(field));
}

void            sems_init(t_ipc_res *ipc_res)
{
  int           i;

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

int		master_process(t_ipc_res *ipc_res)
{
  char		c;
  unsigned char *field;

  if ((field = (unsigned char*)shmat(ipc_res->field_id, NULL, SHM_R | SHM_W)) == NULL)
    return (EXIT_FAILURE);
  memset(field, 0, WIDTH * HEIGHT);
  sems_init(ipc_res);
  printf("MASTER\n");
  printf("FIELD ID : %d\n", ipc_res->field_id);
  printf("SEM ID : %d\n", ipc_res->sem_id);
  printf("MSG ID : %d\n", ipc_res->msg_id);
  if (!battle_begini_ctl(ipc_res, field))
  {
    printf("All players are late!\n");
    clear_ressources(ipc_res);
    return (EXIT_FAILURE);
  }
  while (1)
  {
    display_field(field);
    read(1, &c, 1);
    if (c == 'q')
    {
      clear_ressources(ipc_res);
      return (EXIT_SUCCESS);
    }
    read(1, &c, 1);
  }
  return (EXIT_SUCCESS);
}

int		run_game(key_t key, t_player *player)
{
  t_ipc_res	ipc_res;

  if ((ipc_res.field_id = shmget(key, WIDTH * HEIGHT, SHM_R | SHM_W)) != -1)
    {
      if ((ipc_res.sem_id = semget(key, WIDTH * HEIGHT + 1, SHM_R | SHM_W)) == -1
	    || (ipc_res.msg_id = msgget(key, SHM_R | SHM_W)) == -1)
        return (EXIT_FAILURE);
      return (slave_process(&ipc_res, player));
    }
  else
    {
      if ((ipc_res.field_id = shmget(key, WIDTH * HEIGHT, IPC_CREAT | SHM_R | SHM_W)) == -1
            || (ipc_res.sem_id = semget(key, WIDTH * HEIGHT + 1, IPC_CREAT | SHM_R | SHM_W)) == -1
            || (ipc_res.msg_id = msgget(key, IPC_CREAT | SHM_R | SHM_W)) == -1)
      {
        perror("");
        printf("FIELD ID : %d\n", ipc_res.field_id);
        printf("SEM ID : %d\n", ipc_res.sem_id);
        printf("MSG ID : %d\n", ipc_res.msg_id);
	return (EXIT_FAILURE);
      }
      return (master_process(&ipc_res));
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
  init_fct(p_fct);
  player.player_list = new_list_default();
  srand(time(NULL));
  player.team_id = atoi(argv[1]);
  player.sh_i = -1;
  if ((pwd = getenv("PWD")) == NULL)
    return (EXIT_FAILURE);
  if ((key = ftok(pwd, 1)) >= 0)
    return (run_game(key, &player));
  else
    return (EXIT_FAILURE);
}
