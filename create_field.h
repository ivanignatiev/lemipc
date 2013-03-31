/*
** create_field.h for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Sun Mar 31 19:38:25 2013 vincent couvignou
** Last update Sun Mar 31 21:41:48 2013 ivan ignatiev
*/

#ifndef CREATE_FIELD_H_
# define CREATE_FIELD_H_

# include "lemipc.h"
# include "lemipc_structures.h"

void		create_dfield(unsigned char *field,
			      unsigned char d_field[HEIGHT][WIDTH], t_ipc_res *res);

#endif /* !CREATE_FIELD_H_ */
