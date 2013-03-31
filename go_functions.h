/*
** go_functions.h for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Sun Mar 31 19:27:04 2013 vincent couvignou
** Last update Sun Mar 31 19:28:28 2013 vincent couvignou
*/

#ifndef GO_FUNCTIONS_H_
# define GO_FUNCTIONS_H_

# include <string.h>
# include "lemipc_structures.h"

# define DEF_RANGE	6

int	go_down(t_player *player, unsigned char d_field[HEIGHT][WIDTH]);
int	go_up(t_player *player, unsigned char d_field[HEIGHT][WIDTH]);
int	go_left(t_player *player, unsigned char d_field[HEIGHT][WIDTH]);
int	go_right(t_player *player, unsigned char d_field[HEIGHT][WIDTH]);

#endif /* !GO_FUNCTIONS_H_ */
