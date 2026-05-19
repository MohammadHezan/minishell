CC          = cc
CFLAGS      = -Wall -Wextra -Werror -I. -I42_libft 
RM          = rm -f

NAME        = minishell

PARSING_SRC = parsing/lexer_utils.c \
              parsing/lexer.c \
              parsing/syntax_check.c \
              parsing/env_init.c \
              parsing/free.c \
              parsing/cmd_builder.c \
              parsing/redirections.c \
              parsing/heredoc.c \
              parsing/expander_utils.c \
              parsing/expander_core.c \
              parsing/parse_core.c \
              parsing/exec_mock.c \
              parsing/signals.c \
              parsing/main.c

EXECUTION_SRC = execution/builtin.c \
                execution/builtin2.c \
                execution/builtin_checkers.c \
                execution/env.c \
                execution/exec.c \
                execution/path.c

SRCS        = $(PARSING_SRC) $(EXECUTION_SRC)

OBJS        = $(SRCS:.c=.o)

LIBFT_DIR   = 42_libft
LIBFT_A     = $(LIBFT_DIR)/libft.a

all: $(NAME)

$(LIBFT_A):
	make -C $(LIBFT_DIR) bonus

$(NAME): $(OBJS) $(LIBFT_A)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -lreadline -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C $(LIBFT_DIR) clean
	$(RM) $(OBJS)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re