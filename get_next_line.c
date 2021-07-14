/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkierbel <rkierbel@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/19 16:16:05 by rkierbel          #+#    #+#             */
/*   Updated: 2021/07/14 10:02:20 by rkierbel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	*ft_calloc(size_t count, size_t size)
{
	unsigned char	*object;
	size_t			i;

	i = 0;
	object = malloc(count * size);
	if (object == NULL)
		return (NULL);
	while (i < count * size)
	{
		object[i] = 0;
		i++;
	}
	return ((void *)object);
}

int	ft_find_newline_char(char *buf, char nl)
{
	int	i;

	i = 0;
	if (buf[i])
	{
		while (buf[i] && buf[i] != nl) //while we havent reach null byte AND while character analyzed is different from newline char
		// -> ie if we reach null byte OR if we find a newline char -> exit loop 
			i++;
		if (buf[i] == nl) //return the index of eventual newline char
			return (i);
	}
	return (-1); //no newline char found -> return -1
}

ssize_t	perform_reads(char **save, ssize_t ret, char *buf, int fd)
{
	//if gnl is called for the first time OR our static has been freed because a new line has been outputted using data previously stored in the static
	// -> our static is empty -> points to NULL -> go to "else" ctrl structure
	if (save[fd] != NULL) //save[fd] contains value from previous reading
	{
		ft_strcpy(buf, save[fd]); //copy that in buf, our temp char array we can easily manipulate
		free(save[fd]); //free that static and set it to NULL, thus we will be able to perform a new reading
		save[fd] = NULL;
	}
	else //save[fd] is empty -> we read
	{
		ret = read(fd, buf, BUFFER_SIZE); //ret will have either a value of -1 in case of syscall read error
		//if we are at EOF, ret will be assigned a value of 0
		//or ret will have a value of n-bytes read, ie corresponding to the index of last character read + 1 
		// => buf[ret] will be set to null byte to indicate this is a null-terminated string (see wikipedia page for more expl on why it's important)
		if (ret < 0) //if ret has a neg value ie syscall read fails, error -> ret = -1
			return (-1);
		buf[ret] = '\0';
	}
	return (ret);
}

int	ft_create_line(char *buf, char **line, char **save, int fd)
{
	int		i;
	char	*tmp;
	char	*buf_with_nl;

	i = ft_find_newline_char(buf, '\n'); //that's pretty obvious what it does
	if (i != -1) //we found a newline char -> we can get a line
	{
		buf_with_nl = ft_substr(buf, 0, i); //extract this substring starting at index 0 in buf ending at index newline -> i
		tmp = *line; //assign the value of line to tmp in case a previous reading was performed during the same gnl call considering BUFFSIZE
		*line = ft_strjoin(tmp, buf_with_nl); //join that data
		free(tmp); //free that local temp
		free(buf_with_nl); //free that local buf 
		if (buf + i + 1 != NULL) //check whether there is data we might need after the newline char
			save[fd] = ft_strdup(buf + i + 1); //save it in our beloved static to hold for further GNL call in main prog
		return (1); //we got a line, return one
	}
	else //we didnt find a newline char
	{
		tmp = *line; //tmp is assigned the value of *line -> nothing
		*line = ft_strjoin(tmp, buf); //*line gets the value of buf, this procedure allows to mallocate for line, keeps track of what was read
		//during previous gnl iteration and not used to get a line
		free(tmp); //and free tmp right away
	}
	return (0);
}

int	get_next_line(int fd, char **line)
{
	ssize_t		ret; //conventionally, size_t is used to represent the size of memory objects (equiv unsigned int). 
					//s(igned)size_t allows us to handle error value of -1
	char		buf[BUFFER_SIZE + 1]; //buf is a temporary array of char that will allow us to perform readings and manipulate the data as a string
	static char	*save[OPEN_MAX]; //in the main prog, gnl will be called several times, in a loop, to read an input source of data like a file
	//until the end. Since we are reading n-bytes at a time depending on above BUFFER_SIZE, we might have some interesting data after the first read is perform, that is not used and that we will need througout the successive calls of gnl. 
//A local static variable "remembers" the data held from previous calls that were not manually wiped (freed). 
//By default in MAC OS X, 256 files may be opened by the system at the same time. One of the main interest of gnl is to be able to read from 
//several files at the same time while keeping track of what is read. Implementing our static as an array of 255 contiguous pointers to char*
//allows us to directly handle reading from multiple files while saving "staticly" the corresponding useful data, during each iteration, for each file read.

	if (fd < 0 || !line || BUFFER_SIZE <= 0 || fd > OPEN_MAX) //error check -> no fd can be negative nor above 255 (see above), 
	//if we have nothing to read ie BUFFSIZE <= 0 there's nothing to do, 
	//if our pointer to pointer to char line is invalid, we cannot perform readings because we cannot hold any data (and we might segfault)
		return (-1);
	*line = NULL; //always initialize pointers to null to prevent undefined behavior due to garbage value laying in mem
	ret = 1; //our ret var is set to one, since if we reach this point and everything is valid, 	
	while (ret > 0)
	{
		ret = perform_reads(save, ret, buf, fd); //we call our function perform_reads to... syscall read.
		if (ret < 0) //error check
			return (-1);
		if (!*line) //first loop iteration -> *line is init to NULL =>
			*line = ft_calloc(1, sizeof(char)); //we callocate a tiny space in mem for it, that will then dynamically grow
		//when we "create" lines that will be "gotten" by gnl. 
		//in case of an error during string "joinings" (see in create_line function), this callocation handles *line being assigned a NULL value
		// -> so we can keep reading if relevent data is still to be found considering the BUFFSIZE
		if (ft_create_line(buf, &*line, save, fd) == 1) //we call our function create_line to... create lines. Go check above
			return (1); //if our create_line function returns 1, that means a line has been found and formed and can be used
	}
	return (0); //ret reads EOF
}
