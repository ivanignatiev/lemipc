/*
** move_functions.h for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Sun Mar 31 19:30:04 2013 vincent couvignou
** Last update Sun Mar 31 19:32:01 2013 vincent couvignou
*/

#ifndef MOVE_FUNCTIONS_H_
# define MOVE_FUNCTIONS_H_

# include <string.h>
# include "lemipc_structures.h"
# include "lemipc.h"
# include "go_functions.h"

int		move_right(t_player *player, unsigned char *field,
    			t_ipc_res *ipc_res);
int		move_left(t_player *player, unsigned char *field,
    			t_ipc_res *ipc_res);
int		move_up(t_player *player, unsigned char *field,
    			t_ipc_res *ipc_res);
int		move_down(t_player *player, unsigned char *field,
    			t_ipc_res *ipc_res);

#endif /* !MOVE_FUNCTIONS_H_ */
