CC 				= gcc
RM				= rm -rf
CFLAGS 			= -Wall -Wextra -Werror
NAME 			= minishell
READLINE_LIB 	= -lreadline -L/Users/jjeong/.brew/opt/readline/lib
READLINE_INC	= -I/Users/jjeong/.brew/opt/readline/include
LIBFT		= libft.a
LIBFT_DIR	= libft

SRC_DIR 	= srcs
SRC 		= srcs/main.c srcs/begin_util.c srcs/begin_util2.c \
	  	  	  srcs/parse.c srcs/exec.c \
			  srcs/parse_cmd.c \
			  srcs/parse_env.c srcs/parse_utils.c \
			  srcs/parse_cpy_token.c srcs/parse_key_value.c \
			  srcs/parse_cpy_token2.c\
			  srcs/redirect.c srcs/redirect_util1.c \
			  srcs/redirect_util2.c srcs/ctl_signal.c \
			  srcs/m_cd.c srcs/m_echo.c srcs/m_env.c \
			  srcs/m_exit.c srcs/m_export_util.c srcs/m_export.c \
			  srcs/m_extern_func_util.c srcs/m_extern_func.c \
			  srcs/m_pwd.c srcs/m_unset.c


OBJ_DIR 	= objs
OBJ 		= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all : 		$(NAME)

$(NAME) : 	$(LIBFT) $(OBJ)
			$(CC) $(CFLAGS) -o $@ $(OBJ) $(READLINE_LIB) $(READLINE_INC) \
			$(LIBFT)

$(LIBFT) :
			cd $(LIBFT_DIR); make
			cp $(LIBFT_DIR)/$(LIBFT) ./

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
			mkdir -p $(OBJ_DIR)
			$(CC) $(CFLAGS) -c $< -o $(<:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o) \
			$(READLINE_INC)

clean :
			cd $(LIBFT_DIR); make clean
			$(RM) $(OBJ) $(OBJ_DIR)

fclean : 	clean
			cd $(LIBFT_DIR); make fclean
			$(RM) $(NAME) $(LIBFT)

re : 		fclean all

.PHONY :	all clean fclean
