/*
** lemipc.c for LemIPC in /home/couvig_v/ProjetsEnCours/LemIPC/LemIPC
** 
** Made by ivan ignatiev
** Login   <ignati_i@epitech.net>
** 
** Started on  Mon Mar 25 15:30:47 2013 ivan ignatiev
** Last update Sun Mar 31 20:38:41 2013 ivan ignatiev
*/

#include	"lemipc.h"

int		die_process(t_ipc_res * ipc_res, t_player *player)
{
  t_msg		ipc_msg;

  while (1)
    {
      if (recv_msg_from_team(ipc_res, player, &ipc_msg) < 0
	  && errno != ENOMSG)
	return (EXIT_SUCCESS);
    }
  return (EXIT_SUCCESS);
}

int		slave_process(t_ipc_res *ipc_res, t_player *player,
			      t_uchar *field)
{
  t_msg		ipc_msg;
  int		msg_size;
  t_fct_msgs	p_fct[NB_KIND];

  init_fct(p_fct);
  send_msg_to_gui(ipc_res, player, "Waiting for battle!");
  lock_sem(ipc_res, AVAILBL_PLACES_SEM);
  send_msg_to_gui(ipc_res, player, "Battle begun!");
  while (1)
    {
      if (!attack(player, field, ipc_res) || !run_away(player, field, ipc_res))
	random_move(player, field, ipc_res);
      if ((msg_size = recv_msg_from_team(ipc_res, player, &ipc_msg)) > 0)
	parse_message(ipc_res, player, ipc_msg.msg, p_fct);
      else if (msg_size == -1 && errno != ENOMSG)
	return (EXIT_SUCCESS);
      if (player_die(ipc_res, player, field))
	return (die_process(ipc_res, player));
      send_msg_to_gui(ipc_res, player, "UPDATE");
      sleep(1);
    }
  return (EXIT_SUCCESS);
}

int		master_process(t_ipc_res *ipc_res, t_player *player)
{
  t_uchar	*field;

  if ((field = (t_uchar*)shmat(ipc_res->field_id, NULL, SHM_R | SHM_W))
      == NULL)
    return (EXIT_FAILURE);
  memset(field, 0, WIDTH * HEIGHT);
  sems_init(ipc_res);
  ressources_info(ipc_res);
  if (!player_preplace(ipc_res, player, field)
      || !battle_begini_ctl(ipc_res, player, field))
    {
      clear_ressources(ipc_res);
      return (EXIT_FAILURE);
    }
  return (slave_process(ipc_res, player, field));
}

int		run_game(key_t key, t_player *player)
{
  t_ipc_res	ipc_res;
  t_uchar	*field;

  if ((ipc_res.field_id = shmget(key, WIDTH * HEIGHT, SHM_SLAVE)) != -1)
    {
      if ((ipc_res.sem_id = semget(key, WIDTH * HEIGHT + 1, SHM_SLAVE))
	  == -1
	  || (ipc_res.msg_id = msgget(key, SHM_SLAVE)) == -1
	  || (field = (t_uchar*)shmat(ipc_res.field_id, NULL, SHM_SLAVE))
	  == NULL)
	return (EXIT_FAILURE);
      ressources_info(&ipc_res);
      if (!player_preplace(&ipc_res, player, field))
	return (EXIT_FAILURE);
      return (slave_process(&ipc_res, player, field));
    }
  if ((ipc_res.field_id = shmget(key, WIDTH * HEIGHT, SHM_MASTER)) == -1
      || (ipc_res.sem_id = semget(key, WIDTH * HEIGHT + 1, SHM_MASTER)) == -1
      || (ipc_res.msg_id = msgget(key, SHM_MASTER)) == -1)
    return (EXIT_FAILURE);
  return (master_process(&ipc_res, player));
}

int		main(int argc, char **argv)
{
  char		*pwd;
  key_t		key;
  t_player	player;

  if (argc < 2)
    {
      fprintf(stderr, "Too few arguments. Use : ./lemipc [team_number]\n");
      return (EXIT_FAILURE);
    }
  player.player_list = new_list_default();
  player.team_id = atoi(argv[1]);
  player.sh_i = -1;
  if ((pwd = getenv("PWD")) == NULL)
    return (EXIT_FAILURE);
  if ((key = ftok(pwd, 1)) >= 0)
    {
      if (run_game(key, &player) == EXIT_SUCCESS)
	return (EXIT_SUCCESS);
      perror("");
    }
  return (EXIT_FAILURE);
}
