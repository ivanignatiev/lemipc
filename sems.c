/*
** sems.c for lemipc in /home/ignati_i//projects/lemipc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sun Mar 31 19:08:14 2013 ivan ignatiev
** Last update Sun Mar 31 19:32:49 2013 ivan ignatiev
*/

#include	"lemipc.h"

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
