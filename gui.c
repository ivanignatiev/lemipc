/*
** gui.c for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sat Mar 30 15:19:36 2013 ivan ignatiev
** Last update Sun Mar 31 21:31:16 2013 ivan ignatiev
*/

#include	"gui.h"

void		display_field(WINDOW *win, t_ipc_res *ipc_res,
			      unsigned char *field)
{
  int		i;

  wclear(win);
  wprintw(win, "\n");
  i = 0;
  while (i < WIDTH * HEIGHT)
    {
      lock_sem(ipc_res, i);
      if (field[i])
	wattron(win, A_BOLD);
      wattron(win, COLOR_PAIR(field[i]));
      if (field[i] != 0)
	wprintw(win, "%3u", field[i]);
      wattroff(win, COLOR_PAIR(field[i]));
      if (field[i])
	wattroff(win, A_BOLD);
      unlock_sem(ipc_res, i);
      if ((i + 1) % WIDTH == 0)
	wprintw(win, "\n");
      i = i + 1;
    }
  wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
  wrefresh(win);
}

int		gui_getmessages(WINDOW *win, WINDOW *win_field,
				t_ipc_res *ipc_res, unsigned char *field)
{
  t_msg		ipc_msg;
  int		msg_size;
  int		i;

  i = 0;
  while ((msg_size = msgrcv(ipc_res->msg_id, (void*)&ipc_msg,
			    sizeof(t_msg), GUI_MSG_TYPE, IPC_NOWAIT)) > 0)
    {
      if (strcmp(ipc_msg.msg, "UPDATE") != 0)
	wprintw(win, " Player %d : %s\n", ipc_msg.sender, ipc_msg.msg);
      i = i + 1;
    }
  if (msg_size == -1 && errno != ENOMSG)
    return (-1);
  if (i)
    {
      display_field(win_field, ipc_res, field);
      wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
      wrefresh(win);
    }
  return (i);
}

int		gui_field(t_ipc_res *ipc_res,
			  unsigned char *field)
{
  char		key;
  int		capture;
  WINDOW	*win_field;
  WINDOW	*win_msg;

  win_field = init_filed_window();
  win_msg = init_msg_window();
  capture = 0;
  while (1)
  {
    key = getch();
    if (key == 'q'
	|| gui_getmessages(win_msg, win_field, ipc_res, field) == -1)
      {
        clear_ressources(ipc_res);
	destroy_window(win_field);
	destroy_window(win_msg);
	return (EXIT_SUCCESS);
      }
    capture = capture + 1;
  }
  return (EXIT_FAILURE);
}

int		main()
{
  key_t		key;
  t_ipc_res	ipc_res;
  unsigned char	*field;
  char		*pwd;

  if ((pwd = getenv("PWD")) == NULL)
    return (EXIT_FAILURE);
  if ((key = ftok(pwd, 1)) < 0)
    return (EXIT_FAILURE);
  if ((ipc_res.field_id = shmget(key, WIDTH * HEIGHT, SHM_R | SHM_W)) != -1)
    {
      if ((ipc_res.sem_id = semget(key, WIDTH * HEIGHT + 1, SHM_R | SHM_W)) == -1
	  || (ipc_res.msg_id = msgget(key, SHM_R | SHM_W)) == -1)
        return (EXIT_FAILURE);
      if ((field = (unsigned char*)shmat(ipc_res.field_id, NULL, SHM_R | SHM_W)) == NULL)
	return (EXIT_FAILURE);
      ressources_info(&ipc_res);
      return (gui_field(&ipc_res,field));
    }
  return (EXIT_FAILURE);
}
