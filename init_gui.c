/*
** init_gui.c for lemipc in /home/ignati_i//projects/lemipc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Sun Mar 31 21:27:00 2013 ivan ignatiev
** Last update Sun Mar 31 21:27:42 2013 ivan ignatiev
*/

#include	"gui.h"

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

WINDOW		*init_filed_window(void)
{
  WINDOW	*win;

  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  init_colors();
  timeout(1);
  win = newwin(HEIGHT + 2, WIDTH * 3 + 2, 0, 0);
  return (win);
}

WINDOW		*init_msg_window(void)
{
  WINDOW	*win;

  win = newwin(HEIGHT + 2, (WIDTH * 3 + 2) * 2, 0, WIDTH * 3 + 3);
  scrollok(win, TRUE);
  return (win);
}

void		destroy_window(WINDOW *win)
{
  delwin(win);
  endwin();
}
