/*
** lemipc.c for lemipc in /home/ignati_i/projects/lemipc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Mon Mar 25 15:30:47 2013 ivan ignatiev
** Last update Tue Mar 26 14:28:07 2013 ivan ignatiev
*/

#include "lemipc.h"

void		display_field(t_ipc_res *ipc_res, unsigned char *field)
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

int		send_msg_to_team(t_ipc_res *ipc_res, t_player *player, const char *msg)
{
  t_msg		snd_msg;

  bzero(&snd_msg, sizeof(t_msg));
  strcpy(snd_msg.msg, msg);
  snd_msg.mtype = player->team_id;
  return (msgsnd(ipc_res->msg_id, (const void *)&snd_msg, sizeof(t_msg), 0));
}

int		recv_msg_from_team(t_ipc_res *ipc_res, t_player *player, t_msg *msg)
{
  return (msgrcv(ipc_res->msg_id, (void*)msg, sizeof(t_msg), player->team_id, IPC_NOWAIT));
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
  char		msg[100];

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
  sprintf(msg, "I moved to %d\n", player->sh_i);
  send_msg_to_team(ipc_res, player, msg);
  unlock_sem(ipc_res);
}

int		slave_process(t_ipc_res *ipc_res, t_player *player)
{
  t_msg		msg;
  int		msg_size;
  unsigned char *field;

  if ((field = (unsigned char*)shmat(ipc_res->field_id, NULL, SHM_R)) == NULL)
    return (EXIT_FAILURE);
  printf("SLAVE\n");
  printf("FIELD ID : %d\n", ipc_res->field_id);
  printf("SEM ID : %d\n", ipc_res->sem_id);
  printf("MSG ID : %d\n", ipc_res->msg_id);
  while (1)
    {
      place_player(ipc_res, player, field);
      usleep(10000);
      if ((msg_size = recv_msg_from_team(ipc_res, player, &msg)) > 0)
	printf("MESSAGE : %s", msg.msg);
      else if (msg_size == -1)
	return (EXIT_FAILURE);
    }
  return (EXIT_SUCCESS);
}

int		master_process(t_ipc_res * ipc_res)
{
  char		c;
  unsigned char *field;

 if (semctl(ipc_res->sem_id, 0, SETVAL, 1) == -1)
    return (EXIT_FAILURE);
  if ((field = (unsigned char*)shmat(ipc_res->field_id, NULL, SHM_R)) == NULL)
    return (EXIT_FAILURE);
  printf("MASTER\n");
  printf("FIELD ID : %d\n", ipc_res->field_id);
  printf("SEM ID : %d\n", ipc_res->sem_id);
  printf("MSG ID : %d\n", ipc_res->msg_id);
  while (1)
    {
      display_field(ipc_res, field);
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
