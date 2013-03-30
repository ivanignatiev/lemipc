/*
** run_away.h for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Fri Mar 29 13:35:14 2013 vincent couvignou
** Last update Sat Mar 30 18:20:57 2013 vincent couvignou
*/

#ifndef RUN_AWAY_H_
# define RUN_AWAY_H_

# include <string.h>
# include "lemipc_structures.h"

# define DEF_RANGE	4

int		run_away(t_player *player, unsigned char *field, t_ipc_res *);
void		create_dfield(unsigned char *field,
    unsigned char d_field[HEIGHT][WIDTH], t_ipc_res *res);

#endif /* !RUN_AWAY_H_ */
