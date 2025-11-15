#include <unistd.h>
#include <fcntl.h>
#include "gnl/get_next_line.h"
#include <string.h>
#include <stdio.h>
#include "cub3d.h"
#include "minilibx/mlx.h"

int load_texture(t_vars *data, t_img *tex, const char *path)
{
    tex->img = mlx_xpm_file_to_image(data->mlx, (char *)path,
            &tex->width, &tex->height);
    if (!tex->img)
    {
        fprintf(stderr, "Error: failed to load texture: %s\n", path);
        return (0);
    }
    tex->addr = mlx_get_data_addr(tex->img,
        &tex->bpp, &tex->line_len, &tex->endian);
    return (1);
}

void init_images(t_vars *data)
{
    data->image.img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
    if (!data->image.img)
    {
        fprintf(stderr, "Error: mlx_new_image failed\n");
        exit(1);
    }
    data->image.addr = mlx_get_data_addr(data->image.img,
        &data->image.bpp, &data->image.line_len, &data->image.endian);

    if (!load_texture(data, &data->textures[0], "textures/wall.xpm") ||
        !load_texture(data, &data->textures[1], "textures/wall.xpm") ||
        !load_texture(data, &data->textures[2], "textures/wall.xpm")  ||
        !load_texture(data, &data->textures[3], "textures/wall.xpm"))
    {
        fprintf(stderr, "Error: one or more textures failed to load.\n");
        exit(1);
    }
}

int find_player(char **map, t_vars *v)
{
    int y = 0;
    while (map[y])
    {
        int x = 0;
        while (map[y][x])
        {
            char c = map[y][x];
            if (c == 'N' || c == 'S' || c == 'W' || c == 'E')
            {
                v->posX = x + 0.5;
                v->posY = y + 0.5;

                if (c == 'N') { v->dirX = 0;  v->dirY = -1; v->planeX = 0.66; v->planeY = 0; }
                if (c == 'S') { v->dirX = 0;  v->dirY =  1; v->planeX = -0.66; v->planeY = 0; }
                if (c == 'E') { v->dirX = 1;  v->dirY =  0; v->planeX = 0; v->planeY = 0.66; }
                if (c == 'W') { v->dirX = -1; v->dirY =  0; v->planeX = 0; v->planeY = -0.66; }

                map[y][x] = '0';
                return 1;
            }
            x++;
        }
        y++;
    }
    fprintf(stderr, "Error: player start position not found in map\n");
    return 0;
}

int key_press(int key, t_vars *data)
{
    printf("key_press: %d\n", key);
    if (key == 53)
        exit(0);
    
    if (key == 13) data->keys.w = 1;
    if (key == 1) data->keys.s = 1;
    if (key == 0) data->keys.a = 1;
    if (key == 2) data->keys.d = 1;

    if (key == 123) data->keys.left = 1;
    if (key == 124) data->keys.right = 1;

    return (0);
}

int key_release(int key, t_vars *data)
{
    if (key == 13)
        data->keys.w = 0;
    if (key == 1)
        data->keys.s = 0;
    if (key == 0)
        data->keys.a = 0;
    if (key == 2)
        data->keys.d = 0;
    if (key == 123)
        data->keys.left = 0;
    if (key == 124)
        data->keys.right = 0;
    return (0);
}

void move_player(t_vars *data)
{
    double move_step_x = 0;
    double move_step_y = 0;
    double backward_step_x = 0;
    double backward_step_y = 0;

    double strafe_x = 0;
    double strafe_y = 0;

    double perp_dirX = -data->dirY;
    double perp_dirY = data->dirX;

    if (data->keys.w)
    {
        move_step_x = data->dirX * MOVE_SPEED;
        move_step_y = data->dirY * MOVE_SPEED;
    }

    if (data->keys.s)
    {
        backward_step_x = -data->dirX * MOVE_SPEED;
        backward_step_y = -data->dirY * MOVE_SPEED;
    }
    
    if (data->keys.d)
    {
        strafe_x = perp_dirX * MOVE_SPEED;
        strafe_y = perp_dirY * MOVE_SPEED;
    }

    if (data->keys.a)
    {
        strafe_x = -perp_dirX * MOVE_SPEED;
        strafe_y = -perp_dirY * MOVE_SPEED;
    }
    
    if (data->keys.w)
    {
        double next_x_check = data->posX + move_step_x;
        if (data->dirX > 0)
            next_x_check += MARGIN;
        else if (data->dirX < 0)
            next_x_check -= MARGIN;
        if (data->map[(int)data->posY][(int)next_x_check] != '1')
            data->posX += move_step_x;

        double next_y_check = data->posY + move_step_y;
        if (data->dirY > 0)
            next_y_check += MARGIN;
        else if (data->dirY < 0)
            next_y_check -= MARGIN;
        if (data->map[(int)next_y_check][(int)data->posX] != '1')
            data->posY += move_step_y;
    }

    if (data->keys.s)
    {
        double next_x_check = data->posX + backward_step_x;
        if (data->dirX > 0)
            next_x_check -= MARGIN;
        else if (data->dirX < 0)
            next_x_check += MARGIN;
        if (data->map[(int)data->posY][(int)next_x_check] != '1')
            data->posX += backward_step_x;

        double next_y_check = data->posY + backward_step_y;
        if (data->dirY > 0)
            next_y_check -= MARGIN;
        else if (data->dirY < 0)
            next_y_check += MARGIN;
        if (data->map[(int)next_y_check][(int)data->posX] != '1')
            data->posY += backward_step_y;
    }

    if (data->keys.a || data->keys.d)
    {
        double next_x_check = data->posX + strafe_x;
        if (strafe_x > 0)
            next_x_check += MARGIN;
        else if (strafe_x < 0)
            next_x_check -= MARGIN;
        if (data->map[(int)data->posY][(int)next_x_check] != '1')
            data->posX += strafe_x;

        double next_y_check = data->posY + strafe_y;
        if (strafe_y > 0)
            next_y_check += MARGIN;
        else if (strafe_y < 0)
            next_y_check -= MARGIN;
        if (data->map[(int)next_y_check][(int)data->posX] != '1')
            data->posY += strafe_y;
    }
    
    double rotate_angle = 0;
    
    if (data->keys.left)
        rotate_angle = -ROT_SPEED;
    if (data->keys.right)
        rotate_angle = ROT_SPEED;

    if (rotate_angle != 0)
    {
        double oldDirX = data->dirX;
        data->dirX = data->dirX * cos(rotate_angle) - data->dirY * sin(rotate_angle);
        data->dirY = oldDirX * sin(rotate_angle) + data->dirY * cos(rotate_angle);

        double oldPlaneX = data->planeX;
        data->planeX = data->planeX * cos(rotate_angle) - data->planeY * sin(rotate_angle);
        data->planeY = oldPlaneX * sin(rotate_angle) + data->planeY * cos(rotate_angle);
    }
}

int render(void *param);

int check_valid_map_params(char **map_data)
{
	int i = 0;
	int j;
	int count = 0;
	while(map_data[i])
	{
		j = 0;
		while(map_data[i][j])
		{
			if(map_data[i][j] != '0' && map_data[i][j] != '1' && map_data[i][j] != ' '
				&& map_data[i][j] != 'N' && map_data[i][j] != 'E' && map_data[i][j] != 'S'
				&& map_data[i][j] != 'W')
			{
				return 0;
			}
			if(map_data[i][j] == 'N' || map_data[i][j] == 'E' || map_data[i][j] == 'S'
				|| map_data[i][j] == 'W')
			{
				count++;
			}
			if(count > 1)
			{
				return 0;
			}
			j++;
		}
		i++;
	}
	return 1;
}

int check_valid_map_wall(char **map_data)
{
	int i = 0;
	int j = 0;
	while(map_data[i][j])
	{
		if(map_data[i][j] != '1' && map_data[i][j] != ' ')
		{
			return 0;
		}
		j++;
	}
	i++;
	if(map_data[i] == NULL)
	{
		return 0;
	}
	while (map_data[i + 1])
	{
		j = 1;
		while (map_data[i][j])
		{
			if (map_data[i][j] == '1' || map_data[i][j] == ' ')
			{
				j++;
				continue;
			}

			if ((map_data[i][j] == '0' || map_data[i][j] == 'W' || map_data[i][j] == 'N'
				|| map_data[i][j] == 'E' || map_data[i][j] == 'S'))
			{
				if (map_data[i][j + 1] == ' ' ||
					map_data[i][j - 1] == ' ' ||
					map_data[i + 1][j] == ' ' ||
					map_data[i - 1][j] == ' ')
				{
					return 0;
				}
			}
			j++;
		}
		i++;
	}
	if(map_data[i] == NULL)
	{
		return 0;
	}
	j = 0;
	while(map_data[i][j])
	{
		if(map_data[i][j] != '1' && map_data[i][j] != ' ')
		{
			return 0;
		}
		j++;
	}
	return 1;
}

int check_param(char* str)
{
    int i = 0;

    while(str[i] && str[i] != '.')
        i++;
    if(!str[i] || ft_strcmp(&str[i], ".cub") != 0)
        return 0;
    return 1;
}

void put_pixel(t_img *img, int x, int y, int color)
{
    char    *dst;

    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
    	return;

    dst = img->addr + (y * img->line_len + x * (img->bpp / 8));
    *(unsigned int *)dst = color;
}

int main(int ac, char **av)
{
	if(ac != 2)
		return (write(2, "error of params ...\n", 20), 1);
	if(check_param(av[1]) == 0)
		return (write(2, "error of params ...\n", 20), 1);
	t_map map;
	char **get_data;
	char *str;
	int fd = open("map.cub", O_RDONLY);
	int i = 0;
	while((str = get_next_line(fd)) != NULL)
	{
		free(str);
		i++;
	}
	close(fd);
	size_t max_lines = 0;
	fd = open("map.cub", O_RDONLY);
	get_data = malloc((i + 1) * sizeof(char *));
	i = 0;
	while((str = get_next_line(fd)) != NULL)
	{
		get_data[i] = malloc (strlen(str) + 1);
		strcpy(get_data[i], str);
		if(get_data[i][strlen(str) - 1] == '\n')
			get_data[i][strlen(str) - 1] = '\0';
		free(str);
		i++;
	}
	get_data[i] = NULL;
	close(fd);
	int j = 0;
	int no = 0;
	int so = 0;
	int we = 0;
	int ea = 0;
	int f = 0;
	int c = 0;
	while(j < i)
	{
		if(get_data[j][0] == '\0')
			j++;
		else
		{
			char **split_data = ft_split(get_data[j], ' ');
			if (split_data[0] && strcmp(split_data[0], "NO") == 0){
				map.north_texture = strdup(split_data[1]);
				no++;
			}
			else if (split_data[0] && strcmp(split_data[0], "SO") == 0){
				map.south_texture = strdup(split_data[1]);
				so++;
			}
			else if (split_data[0] && strcmp(split_data[0], "WE") == 0){
				map.west_texture = strdup(split_data[1]);
				we++;
			}
			else if (split_data[0] && strcmp(split_data[0], "EA") == 0){
				map.east_texture = strdup(split_data[1]);
				ea++;
			}
			else if (split_data[0] && strcmp(split_data[0], "F") == 0){
				map.floor_color = strdup(split_data[1]);
				f++;
			}
			else if (split_data[0] && strcmp(split_data[0], "C") == 0){
				map.ceiling_color = strdup(split_data[1]);
				c++;
			}
			j++;
		}
	}
	if(no != 1 || so != 1 || we != 1 || ea != 1 || f != 1 || c != 1)
	{
		printf("Error: Invalid params configuration.\n");
		return (1);
	}
	j = 0;
	while (get_data[j])
	{
		if(get_data[j][0] != '\0')
		{
			if (get_data[j] && strncmp(get_data[j], "NO", 2) != 0 &&
				strncmp(get_data[j], "SO", 2) != 0 &&
				strncmp(get_data[j], "WE", 2) != 0 &&
				strncmp(get_data[j], "EA", 2) != 0 &&
				strncmp(get_data[j], "F", 1) != 0 &&
				strncmp(get_data[j], "C", 1) != 0)
			{
				break;
			}
		}
		j++;
	}
	map.data = malloc((i - j + 1) * sizeof(char *));
	int temp = j;
	while (get_data[temp])
	{
		if(get_data[temp][0] != '\0')
		{
			if(strlen(get_data[temp]) > max_lines)
				max_lines = strlen(get_data[temp]);
		}
		temp++;
	}
	int k = 0;
	map.flag = 0;
	while (get_data[j])
	{
		if(get_data[j][0] != '\0')
		{
			map.data[k] = malloc(max_lines + 1);
			strcpy(map.data[k], get_data[j]);
			size_t len = strlen(get_data[j]);
			while(len < max_lines)
			{
				map.data[k][len] = ' ';
				len++;
			}
			map.data[k][max_lines] = '\0';
			k++;
		}else
			map.flag = 1;
		if(map.flag == 1 && get_data[j][0] != '\0')
			return (write(2, "map error ...\n", 14), 1); 
		j++;
	}
	map.data[k] = NULL;

	if(check_valid_map_params(map.data) == 0)
	{
		printf("Error: Invalid map parameters.1\n");
		return (1);
	}
	
	if(check_valid_map_wall(map.data) == 0)
	{
		printf("Error: Invalid map parameters.2\n");
		return (1);
	}
	printf("North Texture: %s\n", map.north_texture);
	printf("South Texture: %s\n", map.south_texture);
	printf("West Texture: %s\n", map.west_texture);
	printf("East Texture: %s\n", map.east_texture);
	printf("Floor Color: %s\n", map.floor_color);
	printf("Ceiling Color: %s\n", map.ceiling_color);
	printf("Map Data:\n");

	for (int m = 0; map.data[m]; m++)
	{
		printf("%s\n", map.data[m]);
	}
	t_vars va;
	memset(&va, 0, sizeof(va));

	va.floor_color_hex = 0x52502c;
	va.ceiling_color_hex = 0x46cae8;

    va.mlx = mlx_init();
    if (!va.mlx)
    {
        fprintf(stderr, "Error: mlx_init failed\n");
        return 1;
    }

    va.mlx_win = mlx_new_window(va.mlx, WIDTH, HEIGHT, "cub3d");
    if (!va.mlx_win)
    {
        fprintf(stderr, "Error: mlx_new_window failed\n");
        return 1;
    }

	init_images(&va); 

	mlx_hook(va.mlx_win, 2, 1L<<0, key_press, &va);
	mlx_hook(va.mlx_win, 3, 1L<<1, key_release, &va);
	mlx_hook(va.mlx_win, 17, 0, (int(*)(void *))exit, NULL);

    va.map = map.data;

    if (!find_player(va.map, &va))
        return (1);

    mlx_loop_hook(va.mlx, render, &va);
    mlx_loop(va.mlx);
    return 0;
}

unsigned int get_texture_color(t_img *tex, int x, int y)
{
    char *dst;
    
    if (x < 0 || y < 0 || x >= tex->width || y >= tex->height)
        return (0);
        
    dst = tex->addr + (y * tex->line_len + x * (tex->bpp / 8));
    return (*(unsigned int*)dst);
}

int render(void *param)
{
    t_vars *data = (t_vars *)param;
    t_img  *texture;
    
    move_player(data);

    for (int y = 0; y < HEIGHT; y++)
        for (int x = 0; x < WIDTH; x++)
            put_pixel(&data->image, x, y, 0x000000); 

    for (int x = 0; x < WIDTH; x++)
    {
        double cameraX = 2 * x / (double)WIDTH - 1;
        double rayDirX = data->dirX + data->planeX * cameraX;
        double rayDirY = data->dirY + data->planeY * cameraX;
        int mapX = (int)data->posX;
        int mapY = (int)data->posY;
        double deltaDistX = (rayDirX == 0) ? 1e30 : fabs(1 / rayDirX);
        double deltaDistY = (rayDirY == 0) ? 1e30 : fabs(1 / rayDirY);
        int stepX, stepY;
        double sideDistX, sideDistY;

        if (rayDirX < 0) { stepX = -1; sideDistX = (data->posX - mapX) * deltaDistX; }
        else { stepX = 1; sideDistX = (mapX + 1.0 - data->posX) * deltaDistX; }

        if (rayDirY < 0) { stepY = -1; sideDistY = (data->posY - mapY) * deltaDistY; }
        else { stepY = 1; sideDistY = (mapY + 1.0 - data->posY) * deltaDistY; }

        int hit = 0;
        int side;

        while (!hit)
        {
            if (sideDistX < sideDistY) { sideDistX += deltaDistX; mapX += stepX; side = 0; }
            else { sideDistY += deltaDistY; mapY += stepY; side = 1; }
            if (data->map[mapY][mapX] == '1')
                hit = 1;
        }

        double perpWallDist;
        if (side == 0)
            perpWallDist = (mapX - data->posX + (1 - stepX) / 2) / rayDirX;
        else
            perpWallDist = (mapY - data->posY + (1 - stepY) / 2) / rayDirY;

        int lineHeight = (int)(HEIGHT / perpWallDist);
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT) drawEnd = HEIGHT - 1;
        
        for (int y = 0; y < drawStart; y++)
            put_pixel(&data->image, x, y, data->ceiling_color_hex);

        if (side == 0)
            texture = (rayDirX > 0) ? &data->textures[0] : &data->textures[1]; // E vs W
        else
            texture = (rayDirY > 0) ? &data->textures[2] : &data->textures[3]; // S vs N

        double wallX; 
        if (side == 0) 
            wallX = data->posY + perpWallDist * rayDirY;
        else 
            wallX = data->posX + perpWallDist * rayDirX;
        
        wallX -= floor(wallX);
        int texX = (int)(wallX * (double)texture->width);

        if (side == 0 && rayDirX > 0)
            texX = texture->width - texX - 1;
        if (side == 1 && rayDirY < 0)
            texX = texture->width - texX - 1;

        double step = 1.0 * texture->height / lineHeight;
        double texPos = (drawStart - HEIGHT / 2 + lineHeight / 2) * step;

        for (int y = drawStart; y < drawEnd; y++)
        {
            int texY = (int)texPos;
            texPos += step;

            unsigned int color = get_texture_color(texture, texX, texY);
            
            if (side == 1)
                color = (color >> 1) & 8355711;

            put_pixel(&data->image, x, y, color);
        }
        
        for (int y = drawEnd; y < HEIGHT; y++)
            put_pixel(&data->image, x, y, data->floor_color_hex);
    }

    mlx_put_image_to_window(data->mlx, data->mlx_win, data->image.img, 0, 0);
    return (0);
}