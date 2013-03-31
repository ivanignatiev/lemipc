/*
** lemipc.h for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Tue Mar 26 14:17:37 2013 ivan ignatiev
** Last update Sat Mar 30 18:35:55 2013 vincent couvignou
*/

#ifndef LEMIPC_H_
# define LEMIPC_H_

# include <time.h>
# include "handle_messages.h"
# include "run_away.h"
# include "random_move.h"
# include "lemipc_structures.h"
# include "attack.h"

int		count_players(t_ipc_res *ipc_res,
			      unsigned char *field);
int	        count_players_in_team(t_ipc_res *ipc_res, t_player *player, unsigned char *field);
int		send_msg_to_team(t_ipc_res *ipc_res, t_player *player, int count, const char *msg);
int             send_message_to_player(t_ipc_res *ipc_res, t_player *player, int player_num, const char *msg);
int		recv_msg_from_team(t_ipc_res *ipc_res, t_player *player, t_msg *msg);
int		get_shm_cell(t_ipc_res *ipc_res, int x, int y,
			     unsigned char *field);
int		get_shm_index(int, int);
void		lock_sem(t_ipc_res *ipc_res, int i);
void		unlock_sem(t_ipc_res *ipc_res, int i);
void		place_player(t_ipc_res *ipc_res, t_player *player, unsigned char *field);
void            parse_message(t_ipc_res *ipc_res, t_player *player, const char *msg, t_fct_messages *p_fct);
int		slave_process(t_ipc_res *ipc_res, t_player *player, unsigned char *field);
int		run_game(key_t key, t_player *player);

#endif /* !LEMIPC_H_ */
