/*
** gui.c for lemipc in /home/ignati_i//projects/lemipc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sat Mar 30 15:19:36 2013 ivan ignatiev
** Last update Sat Mar 30 17:00:32 2013 ivan ignatiev
*/

#include	"lemipc.h"
#include	"lemipc_structures.h"


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
  int		avsem;

  sem.sem_num = i;
  sem.sem_flg = 0;
  sem.sem_op = 1;
  semop(ipc_res->sem_id, &sem, 1);
}

void		display_field(t_ipc_res *ipc_res, unsigned char *field)
{
  int		i;

  i = 0;
  while (i < WIDTH * HEIGHT)
    {
      lock_sem(ipc_res, i);
      printf("%u ", field[i]);
      unlock_sem(ipc_res, i);
      if ((i + 1) % WIDTH == 0)
	printf("\n");
      i = i + 1;
    }
}

void		clear_ressources(t_ipc_res *ipc_res)
{
  shmctl(ipc_res->field_id, IPC_RMID, NULL);
  semctl(ipc_res->sem_id, 0, IPC_RMID, NULL);
  msgctl(ipc_res->msg_id, IPC_RMID, NULL);
}

int		gui_field(t_ipc_res *ipc_res, unsigned char *field)
{
  char c;

  while (1)
  {
    display_field(ipc_res, field);
    read(1, &c, 1);
    if (c == 'q')
    {
      clear_ressources(ipc_res);
      return (EXIT_SUCCESS);
    }
    read(1, &c, 1);
  }
}

void		ressources_info(t_ipc_res *ipc_res)
{
  printf("FIELD ID : %d\n", ipc_res->field_id);
  printf("SEM ID : %d\n", ipc_res->sem_id);
  printf("MSG ID : %d\n", ipc_res->msg_id);
}


int		main(int argc, char **argv)
{
  key_t		key;
  t_ipc_res	ipc_res;
  unsigned char	*field;
  char		*pwd;

  if ((pwd = getenv("PWD")) == NULL)
    return (EXIT_FAILURE);
  if ((key = ftok(pwd, 1)) < 0)
    return (EXIT_FAILURE);
  if ((ipc_res.field_id = shmget(key, WIDTH * HEIGHT, SHM_R | SHM_W)) != -1)
    {
      if ((ipc_res.sem_id = semget(key, WIDTH * HEIGHT + 1, SHM_R | SHM_W)) == -1
	  || (ipc_res.msg_id = msgget(key, SHM_R | SHM_W)) == -1)
        return (EXIT_FAILURE);
      if ((field = (unsigned char*)shmat(ipc_res.field_id, NULL, SHM_R | SHM_W)) == NULL)
	return (EXIT_FAILURE);
      ressources_info(&ipc_res);
      return (gui_field(&ipc_res,field));
    }
  return (EXIT_FAILURE);
}
