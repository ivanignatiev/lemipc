/*
** lemipc.h for lemipc in /home/ignati_i/projects/lemipc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Tue Mar 26 14:17:37 2013 ivan ignatiev
** Last update Sun Mar 31 21:34:14 2013 ivan ignatiev
*/

#ifndef LEMIPC_H_
# define LEMIPC_H_

# include	<time.h>
# include	"handle_messages.h"
# include	"run_away.h"
# include	"random_move.h"
# include	"lemipc_structures.h"
# include	"attack.h"

int		count_players(t_ipc_res *ipc_res,
			      unsigned char *field);
int	        count_players_in_team(t_ipc_res *ipc_res, t_player *player,
				      unsigned char *field);
int		count_aliens(int my_team, int *around);
int		get_shm_cell(t_ipc_res *ipc_res, int x, int y,
			     unsigned char *field);
int		get_shm_index(int, int);
int		send_msg_to_team(t_ipc_res *ipc_res, t_player *player,
				 int count, const char *msg);
int             send_msg_to_player(t_ipc_res *ipc_res, t_player *player,
				   int player_num, const char *msg);
int		recv_msg_from_team(t_ipc_res *ipc_res, t_player *player,
				   t_msg *msg);
void            parse_message(t_ipc_res *ipc_res, t_player *player,
			      const char *msg, t_fct_msgs *p_fct);
int		send_msg_to_gui(t_ipc_res *ipc_res, t_player *player,
				const char *msg, ...);
void		lock_sem(t_ipc_res *ipc_res, int i);
void		unlock_sem(t_ipc_res *ipc_res, int i);
void		sems_init(t_ipc_res *ipc_res);
void		place_player(t_ipc_res *ipc_res, t_player *player,
			     unsigned char *field);
void		clear_player(t_ipc_res *ipc_res, t_player *player,
			     t_uchar *field);
int		player_kill(t_ipc_res *ipc_res, t_player *player,
			    t_uchar *field);
int		player_preplace(t_ipc_res *ipc_res, t_player *player,
				t_uchar *field);
int		player_die(t_ipc_res *ipc_res, t_player *player,
			   t_uchar *field);
int		battle_begini_ctl(t_ipc_res *ipc_res, t_player *player,
				  t_uchar *field);
void		ressources_info(t_ipc_res *ipc_res);
void		clear_ressources(t_ipc_res *ipc_res);

#endif /* !LEMIPC_H_ */
