CC			= cc
CFLAGS		= -Wall -Wextra -Werror -I. -Ilibft
RM			= rm -f

NAME		= minishell

GNL_SRC		= get_next_lien/get_next_line_bonus.c \
			  get_next_lien/get_next_line_utils_bonus.c

PARSING_SRC	= parsing/cmd_builder.c \
			  parsing/env_init.c \
			  parsing/exec_mock.c \
			  parsing/expander.c \
			  parsing/free.c \
			  parsing/lexer_utils.c \
			  parsing/lexer.c \
			  parsing/main.c \
			  parsing/parse_core.c \
			  parsing/syntax_check.c \
			  parsing/redirections.c \

SRCS		= $(PARSING_SRC) $(GNL_SRC)

OBJS		= $(SRCS:.c=.o)

LIBFT_A		= libft/libft.a

all: $(NAME)

$(LIBFT_A):
	make -C libft bonus

$(NAME): $(OBJS) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C libft clean
	$(RM) $(OBJS)

fclean: clean
	make -C libft fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
