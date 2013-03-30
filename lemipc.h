/*
** lemipc.h for lemipc in /home/ignati_i/projects/lemipc
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Tue Mar 26 14:17:37 2013 ivan ignatiev
** Last update Sat Mar 30 14:59:47 2013 ivan ignatiev
*/

#ifndef LEMIPC_H_
# define LEMIPC_H_

# include <time.h>
# include "handle_messages.h"
# include "run_away.h"
# include "random_move.h"
# include "lemipc_structures.h"

void		display_field(unsigned char *field);
int	        count_players_in_team(t_player *player, unsigned char *field);
int		send_msg_to_team(t_ipc_res *ipc_res, t_player *player, int count, const char *msg);
int             send_message_to_player(t_ipc_res *ipc_res, t_player *player, int player_num, const char *msg);
int		recv_msg_from_team(t_ipc_res *ipc_res, t_player *player, t_msg *msg);
void		lock_sem(t_ipc_res *ipc_res, int i);
void		unlock_sem(t_ipc_res *ipc_res, int i);
void		place_player(t_ipc_res *ipc_res, t_player *player, unsigned char *field);
void            parse_message(t_ipc_res *ipc_res, t_player *player, const char *msg);
int		slave_process(t_ipc_res *ipc_res, t_player *player);
int		run_game(key_t key, t_player *player);
int		main(int argc, char **argv);

#endif /* !LEMIPC_H_ */
