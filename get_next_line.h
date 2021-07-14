/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkierbel <rkierbel@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 10:23:58 by rkierbel          #+#    #+#             */
/*   Updated: 2021/07/14 10:09:25 by rkierbel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include <stdlib.h>
# include <unistd.h>

int		get_next_line(int fd, char **line);
size_t	ft_strlen(const char *str);
char	*ft_strdup(const char *str);
char	*ft_strjoin(const char *str1, const char *str2);
char	*ft_substr(const char *str, unsigned int start, size_t len);
char	*ft_strcpy(char *dst, char *src);
#endif
