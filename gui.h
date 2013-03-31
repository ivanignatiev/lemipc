/*
** gui.h for leipc in /home/ignati_i//projects/lemipc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sun Mar 31 21:28:11 2013 ivan ignatiev
** Last update Sun Mar 31 21:31:31 2013 ivan ignatiev
*/

#ifndef GUI_H_
# define GUI_H_

# include	<stdlib.h>
# include	<ncurses.h>
# include	"lemipc.h"
# include	"lemipc_structures.h"

void		destroy_window(WINDOW *win);
WINDOW		*init_msg_window(void);
WINDOW		*init_filed_window(void);
void		init_colors();

#endif /* !GUI_H_ */
