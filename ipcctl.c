/*
** ipcctl.c for lemipc in /home/ignati_i//projects/lemipc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sun Mar 31 19:31:35 2013 ivan ignatiev
** Last update Sun Mar 31 19:32:56 2013 ivan ignatiev
*/

#include	"lemipc.h"

void		ressources_info(t_ipc_res *ipc_res)
{
  printf("FIELD ID : %d\n", ipc_res->field_id);
  printf("SEM ID : %d\n", ipc_res->sem_id);
  printf("MSG ID : %d\n", ipc_res->msg_id);
}

int		battle_begini_ctl(t_ipc_res *ipc_res, t_uchar *field)
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

void		clear_ressources(t_ipc_res *ipc_res)
{
  shmctl(ipc_res->field_id, IPC_RMID, NULL);
  semctl(ipc_res->sem_id, 0, IPC_RMID, NULL);
  msgctl(ipc_res->msg_id, IPC_RMID, NULL);
}
