#include <unistd.h>
#include <fcntl.h>
#include "gnl/get_next_line.h"
#include <string.h>
#include <stdio.h>
#include "cub3d.h"

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


int main()
{
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
		}
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

}