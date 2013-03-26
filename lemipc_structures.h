/*
** lemipc_structures.h for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Tue Mar 26 16:26:00 2013 vincent couvignou
** Last update Tue Mar 26 17:01:34 2013 vincent couvignou
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

# define WIDTH 20
# define HEIGHT 20
# define MAX_TEAM_NUM 10

typedef struct	s_ipc_res
{
  int		field_id;
  int		sem_id;
  int		msg_id;
}		t_ipc_res;

typedef struct	s_player_list
{
  int		player_number;
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
