# ifndef CUB3D_H
#define CUB3D_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define WIDTH 1920
#define HEIGHT 1280
#define ROT_SPEED 0.05
#define MARGIN 0.2
#define MOVE_SPEED 0.05

#define TEX_W 64
#define TEX_H 64


typedef struct s_map
{
    char *north_texture;
    char *south_texture;
    char *west_texture;
    char *east_texture;
    char *floor_color;
    char *ceiling_color;
    char **data;
    int width;
    int height;
    int flag;
} t_map;

typedef struct s_keys {
    int w, s, a, d, left, right;
}   t_keys;

typedef struct s_img
{
    void    *img;
    char    *addr;
    int     bpp;
    int     line_len;
    int     endian;
    int     width;
    int     height;
}   t_img;

typedef struct s_vars
{
	void		*mlx;
	void		*mlx_win;
	char		**map;
    t_img       image;
    t_img       textures[4];

    double      posX, posY;
    double      dirX, dirY;
    double      planeX, planeY;
    t_keys keys;
    int         ceiling_color_hex;
    int         floor_color_hex;
}				t_vars;

int ft_strcmp(const char *str1, const char *str2);
char	**ft_split(char const *s, char c);

#endif