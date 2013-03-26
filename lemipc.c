/*
** lemipc.c for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Mon Mar 25 15:30:47 2013 ivan ignatiev
** Last update Tue Mar 26 16:33:23 2013 vincent couvignou
*/

#include "lemipc.h"

static t_fct_messages p_fct[3];

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

int         count_players_in_team(t_player *player, unsigned char *field)
{
  int       count;
  int       i;

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
  player_num = player->team_id * 10;
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

void		lock_sem(t_ipc_res *ipc_res)
{
  struct sembuf sem;

  sem.sem_num = 0;
  sem.sem_flg = 0;
  sem.sem_op = -1;
  semop(ipc_res->sem_id, &sem, 1);
}

void		unlock_sem(t_ipc_res *ipc_res)
{
  struct sembuf sem;

  sem.sem_num = 0;
  sem.sem_flg = 0;
  sem.sem_op = 1;
  semop(ipc_res->sem_id, &sem, 1);
}

void		place_player(t_ipc_res *ipc_res, t_player *player, unsigned char *field)
{
  char		move_msg[100];

  if (player->sh_i >= 0)
    {
      lock_sem(ipc_res);
      field[player->sh_i] = 0;
      unlock_sem(ipc_res);
    }
  player->x = rand() % WIDTH;
  player->y = rand() % HEIGHT;
  player->sh_i = player->y * WIDTH + player->x;
  while (field[player->sh_i] != 0 && semctl(ipc_res->sem_id, 0, GETVAL) > 0)
    {
      player->x = rand() % WIDTH;
      player->y = rand() % HEIGHT;
      player->sh_i = player->y * WIDTH + player->x;
    }
  lock_sem(ipc_res);
  field[player->sh_i] = player->team_id;
  sprintf(move_msg, "MOVE:%d:%d:%d:%d", player->num, player->x, player->y, player->sh_i);
  send_msg_to_team(ipc_res, player, count_players_in_team(player, field), move_msg);
  unlock_sem(ipc_res);
}

void            parse_message(t_ipc_res *ipc_res, t_player *player, const char *msg)
{
    int         i;

    i = 0;
    while (strncmp(msg, p_fct[i].name, strlen(p_fct[i].name)) && i < 3)
      i++;
}

int		slave_process(t_ipc_res *ipc_res, t_player *player)
{
  t_msg		ipc_msg;
  int		msg_size;
  unsigned char *field;
  char		present_msg[100];

  if ((field = (unsigned char*)shmat(ipc_res->field_id, NULL, SHM_R | SHM_W)) == NULL)
    return (EXIT_FAILURE);
  printf("SLAVE\n");
  printf("FIELD ID : %d\n", ipc_res->field_id);
  printf("SEM ID : %d\n", ipc_res->sem_id);
  printf("MSG ID : %d\n", ipc_res->msg_id);
  player->num = count_players_in_team(player, field) + 1;
  if (player->num >= MAX_TEAM_NUM)
  {
    fprintf(stderr, "You're late, team don't love you -21\n");
    return (EXIT_FAILURE);
  }
  player->num += (player->team_id * 10);
  printf("PLAYER NUM : %d\n", player->num);
  sprintf(present_msg, "NEWP:%d", player->num);
  send_msg_to_team(ipc_res, player, count_players_in_team(player, field) + 1, present_msg);
  while (1)
    {
     if ((msg_size = recv_msg_from_team(ipc_res, player, &ipc_msg)) > 0)
	parse_message(ipc_res, player, ipc_msg.msg);
      else if (msg_size == -1 && errno != ENOMSG)
      {
        perror("");
        fprintf(stderr, "Shared ressources closed\n");
	return (EXIT_FAILURE);
      }
      place_player(ipc_res, player, field);
      usleep(10000);
    }
  return (EXIT_SUCCESS);
}

int		master_process(t_ipc_res * ipc_res)
{
  char		c;
  unsigned char *field;

 if (semctl(ipc_res->sem_id, 0, SETVAL, 1) == -1)
    return (EXIT_FAILURE);
  if ((field = (unsigned char*)shmat(ipc_res->field_id, NULL, SHM_R | SHM_W)) == NULL)
    return (EXIT_FAILURE);
  printf("MASTER\n");
  printf("FIELD ID : %d\n", ipc_res->field_id);
  printf("SEM ID : %d\n", ipc_res->sem_id);
  printf("MSG ID : %d\n", ipc_res->msg_id);
  while (1)
    {
      display_field(field);
      read(1, &c, 1);
      if (c == 'q')
	{
	  shmctl(ipc_res->field_id, IPC_RMID, NULL);
	  semctl(ipc_res->sem_id, 0, IPC_RMID, NULL);
	  msgctl(ipc_res->msg_id, IPC_RMID, NULL);
	  return (EXIT_SUCCESS);
	}
      read(1, &c, 1);
    }
  return (EXIT_SUCCESS);
}

int		run_game(key_t key, t_player *player)
{
  t_ipc_res	ipc_res;

  printf("IPC Key : %d\n", key);
  if ((ipc_res.field_id = shmget(key, WIDTH * HEIGHT, SHM_R | SHM_W)) != -1)
    {
      if ((ipc_res.sem_id = semget(key, 1, SHM_R | SHM_W)) == -1)
	return (EXIT_FAILURE);
      if ((ipc_res.msg_id = msgget(key, SHM_R | SHM_W)) == -1)
	return (EXIT_FAILURE);
      return (slave_process(&ipc_res, player));
    }
  else
    {
      if ((ipc_res.field_id = shmget(key, WIDTH * HEIGHT, IPC_CREAT | SHM_R | SHM_W)) == -1)
	return (EXIT_FAILURE);
      if ((ipc_res.sem_id = semget(key, 1, IPC_CREAT | SHM_R | SHM_W)) == -1)
	return (EXIT_FAILURE);
      if ((ipc_res.msg_id = msgget(key, IPC_CREAT | SHM_R | SHM_W)) == -1)
	return (EXIT_FAILURE);
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
