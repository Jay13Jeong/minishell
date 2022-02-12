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
# define STDIN_BACKUP 	420 //표준입력fd의 백업fd번호
# define STDOUT_BACKUP  421	//표준출력fd의 백업fd번호
# define SKY            "\001\033[1;36m\002" //터미널의 글자색상을 하늘색으로 해주는 접두사
# define WHITE          "\001\033[0m\002"	//터미널의 배경색을 흰색으로 해주는 접미사
# define STDIN 			0	//표준입력
# define STDOUT 		1	//표준출력
# define STDERR 		2	//표준에러

extern int	g_exit; //프로세스의 종료상태를 담는 전역변수

typedef struct s_token //띄어쓰기 혹은 유의미한 식별자를 기준으로 나눈 토큰
{
	char		*cmd; //토큰의 문자열
	char		redir_flag; //리다이렉션 플레그인지 여부 (맞으면 1, 아니면 0, 에러 -1)
}				t_token;

typedef struct s_linked_order //파이프 기준으로나눈 명령들 (단방향 링크드리스트)
{
	t_token					*cmdline; //현재 파이프의 토큰들
	int						pipe_flag; //다음 파이프 구간이 있으면 1 아니면 0
	int						exit_flag; //전체명령중에 파이프가 없으면 1 아니면 0
	int						right_flag; //오른쪽리다이렉션이면 1, 아니면 0, 에러 -1
	char					*(redirect_filename[4]); /*최종 리다이렉션 기호와 대상을 담을 배열, 각 자리는
	0:좌 리다이렉션 기호, 1:좌 리다이렉션 뒤의 토큰(대상), 2:우 리다이렉션 기호, 3:우 리다이렉션 뒤의 토큰(대상)*/
	struct s_linked_order	*next; //다음 링크주소
}				t_linked_order;

typedef struct s_minishell
{
	char					**env; //메인에서 받아온 환경변수 리스트
	int						len; //환경변수 리스트 길이
	int						error; //따옴표 인식 오류 1 정상 0
	char					**key; //동적할당된 환경변수 키 리스트
	char					**content; //동적할당된 환경변수 값 리스트
	struct s_linked_order	*lo; //명령 링크드리스트
}				t_minishell;

typedef struct s_gss //파싱용 구조체1
{
	int	cnt;
	int	begin;
	int	close;
	int	redir;
	int	len;
}	t_gss;

typedef struct s_parse //파싱용 구조체2
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
