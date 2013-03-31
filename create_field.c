/*
** create_field.c for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by vincent couvignou
** Login   <couvig_v@epitech.net>
** 
** Started on  Sun Mar 31 19:37:44 2013 vincent couvignou
** Last update Sun Mar 31 19:40:47 2013 vincent couvignou
*/

#include "create_field.h"

void		create_dfield(unsigned char *field,
    unsigned char d_field[HEIGHT][WIDTH], t_ipc_res *res)
{
  int		i;
  int		j;
  int		z;

  i = 0;
  j = 0;
  z = 0;
  while (z < WIDTH * HEIGHT)
  {
    lock_sem(res, z);
    d_field[i][j] = field[z];
    unlock_sem(res, z);
    if ((j + 1) % HEIGHT  == 0)
    {
      i++;
      j = 0;
    }
    else
      j++;
    z++;
  }
}

