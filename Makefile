CC = cc

CFLAGS = -Wall -Wextra -Werror

SRCS = ./gnl/get_next_line_utils.c ./gnl/get_next_line.c ./cub3d.c ./utils/ft_split.c\
		
OBJS = $(SRCS:.c=.o)

NAME = cub3d

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all