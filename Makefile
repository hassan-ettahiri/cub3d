CC = cc

CFLAGS = -Wall -Wextra -Werror -O3 #-fsanitize=address -g

SRCS = ./gnl/get_next_line_utils.c ./gnl/get_next_line.c ./cub3d.c ./utils/ft_split.c ./utils/ft_strcmp.c\
		
OBJS = $(SRCS:.c=.o)

MLX_PATH = ./minilibx
MLX = $(MLX_PATH)/libmlx.a

NAME = cub3d

all: $(NAME)

$(NAME): $(OBJS) $(MLX)
	$(CC) $(CFLAGS) $(OBJS) -lmlx -framework OpenGL -framework AppKit $(MLX) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(MLX):
	$(MAKE) -C $(MLX_PATH)


clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME) $(MLX)

re: fclean all