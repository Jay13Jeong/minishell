#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <errno.h>
# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/stat.h>
# include "../libft/libft.h"

# define SUCCESS 	    1
# define FAIL           0
# define ERROR          -1
# define STDIN_BACKUP 	420
# define STDOUT_BACKUP  421
# define SKY            "\001\033[1;36m\002"
# define WHITE          "\001\033[0m\002"
# define STDIN 			0
# define STDOUT 		1
# define STDERR 		2

extern int	g_exit;

typedef struct s_token
{
	char		*cmd;
	char		redir_flag;
}				t_token;

typedef struct s_linked_order
{
	t_token					*cmdline;
	int						pipe_flag;
	int						exit_flag;
	int						right_flag;
	char					quote;
	char					*(redirect_filename[4]);
	struct s_linked_order	*next;
}				t_linked_order;

typedef struct s_minishell
{
	char					**env;
	int						len;
	int						error;
	char					**key;
	char					**content;
	struct s_linked_order	*lo;
}				t_minishell;

typedef struct s_gss
{
	int	cnt;
	int	begin;
	int	close;
	int	redir;
	int	len;
}	t_gss;

typedef struct s_parse
{
	int	m;
	int	c;
	int	end;
	int	prev;
}	t_parse;

char	*get_content(char *key, t_minishell *mini);
void	parse(t_minishell *mini, char *line);
void	reset_add(t_gss *val);
void	gss_init(t_gss *val);
void	parse_init(t_parse *val, t_minishell *mini);
void	free_key_value(t_minishell *mini);
void	destroy_token(t_token *token);
int		rebuild_cmd(char *src, char *dest, t_minishell *mini);
int		ft_strcmp(const char *a, const char *b);
int		get_split_size(const char *s, const char c);
int		unclosed(char *str, char quote);
int		get_key(char *src, char **key);
int		env_cnt(char *str, int *size, t_minishell *mini);
int		env_key_size(char *str);
int		quotes(t_minishell *mini, char *src, char **dest, int *src_move);
int		single_cpy(char *src, char **dest);
int		double_cpy(char *src, char **dest, t_minishell *mini);
t_token	*cmd_split(const char *line, const char pivot, int move, int token_i);
t_token	*rebuild_token(t_minishell *mini, t_token *token, int i);

void	free_char_array(char **array);
int		chk_line(char *line);
void	free_all_list(t_linked_order *order);
void	free_linked_order(t_minishell *mini);

void	init_signal(void);
void	exit_clean(t_minishell *mini, int error_code);
void	show_error(t_linked_order *order);
int		find_env(t_minishell *mini, char *key);
char	*readline_with_path(void);
void	show_exit_upline(void);

int		check_unique_key(t_minishell *mini, char *new_key, char *new_value);
void	alert_export_error(char *cmd, char *key);
void	show_declare_x(t_minishell *mini, int fd_out);
int		key_check_char(char c);
int		key_check_str(char *str, char eof);
void	free_trash(char **arr);

int		exec(t_minishell *mini);
int		redirect_handler(t_minishell *mini, int **pipe_fd);
int		do_redirect(t_linked_order *order, int **pipe_fd);
int		check_flag(t_minishell *mini);
void	alert_redirect_error(char *str, int exit_code);

int		mini_echo(t_minishell *mini, int fd_out);
int		mini_cd(t_minishell *mini);
int		mini_pwd(int fd_out);
int		mini_export(t_minishell *mini, int fd_out);
int		mini_unset(t_minishell *mini);
int		mini_env(t_minishell *mini, int fd_out);
int		mini_exit(t_minishell *mini);
int		extern_func_exec(t_minishell *mini, int *pipe_fd);
void	system_call_exec(t_minishell *mini, char *path, int *fds);

void	ascii_art(void);

#endif
