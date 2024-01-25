/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agallon <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 19:18:30 by agallon           #+#    #+#             */
/*   Updated: 2023/11/23 19:18:33 by agallon          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>	// read close
#include <stdio.h>	// printf malloc free
#include <stdlib.h> // malloc free
#include <fcntl.h>	// open
#include "get_next_line_bonus.h"

///////////////////////////////////////////////////////
// #	RETURN FULL SIZE + CREATE ALL LIST
int	rtrn_size(t_list *first_list, int fd, char (*g_temp)[BUFFER_SIZE])
{
	t_list	*current_list;
	int		size;

	current_list = first_list;
	size = 0;
	while (is_there_n(current_list) <= 0)
	{
		size += -is_there_n(current_list);
		current_list = ini_node(current_list, g_temp, fd);
		if (!current_list)
			return (-1);
		if (read(fd, current_list->buff, BUFFER_SIZE) <= 0)
			break ;
	}
	if (size == 0 && is_there_n(current_list) == 0)
		return (-1);
	if (is_there_n(current_list) > 0)
		size += is_there_n(current_list);
	return (size);
}

///////////////////////////////////////////////////////
// #	RETURN CLEAN NODE, IF FIRST NODE, FILL WITH TEMP
t_list	*ini_node(t_list *first_list, char (*g_temp)[BUFFER_SIZE], int fd)
{	
	t_list	*next_list;
	t_list	*current_list;
	int		i;

	current_list = first_list;
	next_list = (t_list *)malloc(sizeof(t_list));
	if (!next_list)
		return (NULL);
	clean_buff(next_list->buff, 0);
	next_list->next = NULL;
	if (first_list != NULL)
	{
		while (current_list->next != NULL)
			current_list = current_list->next;
		current_list->next = next_list;
	}
	else
	{
		i = 0;
		while (i++ < BUFFER_SIZE && g_temp[fd][i - 1] != '\0')
			next_list->buff[i - 1] = g_temp[fd][i - 1];
	}
	return (next_list);
}

///////////////////////////////////////////////////////
char	*f_rtrn_1(t_list *first_list, int fd, char (*g_temp)[BUFFER_SIZE])
{
	int		size;
	char	*rtrn;

	size = rtrn_size(first_list, fd, g_temp);
	if (size <= 0)
		return (NULL);
	rtrn = (char *)malloc(size + 1);
	if (!rtrn)
		return (NULL);
	rtrn = f_rtrn_2(first_list, rtrn, size);
	return (rtrn);
}

///////////////////////////////////////////////////////
// #	RESET TEMP = COPY THE REMAINDER TO TEMP
void	f_copy_rest(t_list *last_list, char (*g_temp)[BUFFER_SIZE], int fd)
{
	int	i;
	int	size;

	clean_buff(g_temp[fd], 1);
	size = is_there_n(last_list);
	i = 0;
	if (size > 0)
	{
		while (i++ + size < BUFFER_SIZE && \
							last_list->buff[size + i - 1] != '\0')
			g_temp[fd][i - 1] = last_list->buff[size + i - 1];
	}
	else if (size == 0)
		return ;
	else if (size < 0)
	{
		while (i - size < BUFFER_SIZE && last_list->buff[i - size] != '\0')
		{
			g_temp[fd][i] = last_list->buff[i - size];
			i++;
		}
	}
}

///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
///////////////////////////////////////////////////////
char	*get_next_line(int fd)
{
	t_list		*first_list;
	t_list		*current_list;
	char		*rtrn;
	static char	g_temp[1024][BUFFER_SIZE];

	first_list = ini_node(NULL, g_temp, fd);
	if (!first_list)
		return (NULL);
	if (fd > 1023)
		return (NULL);
	rtrn = f_rtrn_1(first_list, fd, g_temp);
	if (rtrn == NULL)
	{
		free_all(&first_list);
		return (NULL);
	}
	current_list = first_list;
	while (current_list->next != NULL)
		current_list = current_list->next;
	f_copy_rest(current_list, g_temp, fd);
	free_all(&first_list);
	return (rtrn);
}
/*
int main()
{
    int fd = open("txt.txt", 0);
    if (fd == -1)
    {
        printf("error opening");
        return 0;
    }

    char *nextline = get_next_line(fd);
    int i = 1;

    while (nextline != NULL)
    {
        printf("Line num:%d = ", i);

        // Print the ASCII values of each character in the line
        for (int j = 0; nextline[j] != '\0'; j++)
        {
            printf("%d ", nextline[j]);
        }

        printf("\n");

        i++;
        free(nextline);
        nextline = get_next_line(fd);
    }

    free(nextline);
    close(fd);
    return 0;
}*/
/*
int	main()
{
	int	fd = open("txt.txt", 0);
    if (fd == -1)
    {
        printf("error opening");
        return 0;
    }
    //printf("TEMP start = %s\n", g_temp);
    char *nextline = get_next_line(fd);
    //free(nextline);
    int	i = 1;
  
    while (nextline != NULL)
    {
  		//printf("TEMP = %s\n", g_temp);
  		printf("line num:%d = %s", i, nextline);
  		i++;
  		free(nextline);
  		nextline = get_next_line(fd);
  	}
  	//printf("TEMP = %s\n", g_temp);
  	printf("line num:%d = %s", i, nextline);
  	free(nextline);
	close(fd);
	return 0;
}*/
