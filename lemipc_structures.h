/*
** lemipc_structures.h for lemipc in /home/ignati_i/projects/lemipc
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Tue Mar 26 16:26:00 2013 vincent couvignou
** Last update Thu Mar 28 03:15:50 2013 ivan ignatiev
*/

#ifndef LEMIPC_STRUCTURES_H_
# define LEMIPC_STRUCTURES_H_

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
# include "LinkedList/my_list.h"

# define WIDTH                  10
# define HEIGHT                 10
# define MAX_TEAM_NUM           10
# define FIGTH_TIMEOUT          10
# define AVAILBL_PLACES_SEM     (WIDTH * HEIGHT)

typedef struct	s_ipc_res
{
  int		field_id;
  int		sem_id;
  int		msg_id;
}		t_ipc_res;

typedef struct	s_player_list
{
  int		player_number;
  bool		activated;
  int		player_x;
  int		player_y;
}		t_player_list;

typedef struct	s_player
{
  int   	num;
  int		x;
  int		y;
  int   	sh_i;
  long		team_id;
  t_my_list	player_list;
} 		t_player;

typedef struct	s_msg
{
  long		mtype;
  char		msg[255];
}		t_msg;

#endif /* !LEMIPC_STRUCTURES_H_ */
