#include <unistd.h>
#include <fcntl.h>
#include "gnl/get_next_line.h"
#include <string.h>
#include <stdio.h>

int main()
{
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
	fd = open("map.cub", O_RDONLY);
	get_data = malloc((i + 1) * sizeof(char *));
	i = 0;
	while((str = get_next_line(fd)) != NULL)
	{
		get_data[i] = malloc (strlen(str));
		strcpy(get_data[i], str);
		get_data[i][strlen(str) - 1] = '\0';
		free(str);
		i++;
	}
	get_data[i] = NULL;
	int j = 0;
	while(j < i)
	{
		if(get_data[j][0] == '\0')
			printf("line khawi\n");
		else
			printf("%s\n", get_data[j]);
		j++;
	}
}