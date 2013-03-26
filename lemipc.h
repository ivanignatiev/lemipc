/*
** lemipc.h for lemipc in /home/ignati_i/projects/lemipc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Tue Mar 26 14:17:37 2013 ivan ignatiev
** Last update Tue Mar 26 15:37:38 2013 ivan ignatiev
*/

#ifndef LEMIPC_H_
# define LEMIPC_H_

# include <sys/ipc.h>
# include <sys/shm.h>
# include <sys/sem.h>
# include <sys/msg.h>
# include <sys/types.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>

# define WIDTH 20
# define HEIGHT 20
# define MAX_TEAM_NUM 10

typedef struct  s_ipc_res
{
  int	field_id;
  int	sem_id;
  int	msg_id;
} t_ipc_res;

typedef struct s_player
{
  int   num;
  int	x;
  int	y;
  int   sh_i;
  long	team_id;
} t_player;

typedef struct s_msg
{
  long	mtype;
  char	msg[255];
} t_msg;

#endif /* !LEMIPC_H_ */
