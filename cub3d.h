#ifndef CUB3D_H
#define CUB3D_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
} t_map;

char	**ft_split(char const *s, char c);

#endif