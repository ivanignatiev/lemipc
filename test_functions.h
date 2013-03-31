/*
** test_functions.h for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Sun Mar 31 19:33:48 2013 vincent couvignou
** Last update Sun Mar 31 19:35:37 2013 vincent couvignou
*/

#ifndef TEST_FUNCTIONS_H_
# define TEST_FUNCTIONS_H_

# include <string.h>
# include "lemipc_structures.h"
# include "lemipc.h"
# include "move_functions.h"

int test_down(t_player *player, unsigned char *field, t_ipc_res *ipc_res,
    unsigned char d_field[HEIGHT][WIDTH]);
int test_up(t_player *player, unsigned char *field, t_ipc_res *ipc_res,
    unsigned char d_field[HEIGHT][WIDTH]);
int test_left(t_player *player, unsigned char *field, t_ipc_res *ipc_res,
    unsigned char d_field[HEIGHT][WIDTH]);
int test_right(t_player *player, unsigned char *field, t_ipc_res *ipc_res,
    unsigned char d_field[HEIGHT][WIDTH]);

#endif /* !TEST_FUNCTIONS_H_ */
