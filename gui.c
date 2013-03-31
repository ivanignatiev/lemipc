/*
** gui.c for lemipc in /home/ignati_i/projects/lemipc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sat Mar 30 15:19:36 2013 ivan ignatiev
** Last update Sun Mar 31 15:51:59 2013 ivan ignatiev
*/

#include	<ncurses.h>
#include	"lemipc.h"
#include	"lemipc_structures.h"

void		lock_sem(t_ipc_res *ipc_res, int i)
{
  struct sembuf	sem;

  sem.sem_num = i;
  sem.sem_flg = 0;
  sem.sem_op = -1;
  semop(ipc_res->sem_id, &sem, 1);
}

void		unlock_sem(t_ipc_res *ipc_res, int i)
{
  struct sembuf	sem;

  sem.sem_num = i;
  sem.sem_flg = 0;
  sem.sem_op = 1;
  semop(ipc_res->sem_id, &sem, 1);
}

void		init_colors()
{
  start_color();
  init_pair(0, COLOR_WHITE, COLOR_BLACK);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_YELLOW, COLOR_BLACK);
  init_pair(4, COLOR_CYAN, COLOR_BLACK);
  init_pair(5, COLOR_BLUE, COLOR_BLACK);
  init_pair(6, COLOR_BLACK, COLOR_WHITE);
  init_pair(7, COLOR_YELLOW, COLOR_RED);
  init_pair(8, COLOR_WHITE, COLOR_GREEN);
  init_pair(9, COLOR_BLUE, COLOR_YELLOW);
}

void		display_field(WINDOW *win, t_ipc_res *ipc_res,
			      unsigned char *field)
{
  int		i;

  i = 0;
  while (i < WIDTH * HEIGHT)
    {
      lock_sem(ipc_res, i);
      if (field[i])
	wattron(win, A_BOLD);
      wattron(win, COLOR_PAIR(field[i]));
      wprintw(win, "%3u", field[i]);
      wattroff(win, COLOR_PAIR(field[i]));
      if (field[i])
	wattroff(win, A_BOLD);
      unlock_sem(ipc_res, i);
      if ((i + 1) % WIDTH == 0)
	wprintw(win, "\n");
      i = i + 1;
    }
}

void		clear_ressources(t_ipc_res *ipc_res)
{
  shmctl(ipc_res->field_id, IPC_RMID, NULL);
  semctl(ipc_res->sem_id, 0, IPC_RMID, NULL);
  msgctl(ipc_res->msg_id, IPC_RMID, NULL);
}

WINDOW		*init_filed_window(void)
{
  WINDOW	*win;

  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  init_colors();
  timeout(1);
  win = newwin(12, 32, 10, 10);
  return (win);
}

void		destroy_field_window(WINDOW *win)
{
  delwin(win);
  endwin();
}

int		gui_field(t_ipc_res *ipc_res,
			  unsigned char *field)
{
  char		key;
  int		capture;
  WINDOW	*win;

  win = init_filed_window();
  capture = 0;
  while (1)
  {
    wclear(win);
    wborder(win, '|', '|', '-', '-', '+', '+', '+', '+');
    display_field(win, ipc_res, field);
    wprintw(win, "%d\n", capture);
    key = getch();
    if (key == 'q')
      {
        clear_ressources(ipc_res);
	destroy_field_window(win);
	return (EXIT_SUCCESS);
      }
    capture = capture + 1;
    wrefresh(win);
    sleep(1);
  }
}

void		ressources_info(t_ipc_res *ipc_res)
{
  printf("FIELD ID : %d\n", ipc_res->field_id);
  printf("SEM ID : %d\n", ipc_res->sem_id);
  printf("MSG ID : %d\n", ipc_res->msg_id);
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
