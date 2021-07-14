/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rkierbel <rkierbel@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/21 10:28:53 by rkierbel          #+#    #+#             */
/*   Updated: 2021/07/14 10:02:31 by rkierbel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(const char *str)
{
	char	*dup;
	int		i;

	dup = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (str[i])
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_strjoin(const char *str1, const char *str2)
{
	char			*joined;
	unsigned int	i;

	if (!str1)
		return ((char *)str2);
	if (!str2)
		return ((char *)str1);
	joined = malloc(sizeof(char) * (ft_strlen(str1) + ft_strlen(str2) + 1));
	if (joined == NULL)
		return (NULL);
	i = 0;
	while (*str1)
		joined[i++] = *str1++;
	while (*str2)
		joined[i++] = *str2++;
	joined[i] = '\0';
	return (joined);
}

char	*ft_substr(const char *str, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;

	if (!str)
		return (NULL);
	if (start > ft_strlen(str))
		return (ft_strdup(""));
	if (ft_strlen(&str[start]) < len)
		len = ft_strlen(&str[start]);
	i = 0;
	substr = malloc(sizeof(char) * len + 1);
	if (substr == NULL)
		return (NULL);
	while (i < len && str[start + i])
	{
		substr[i] = (char)str[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

char	*ft_strcpy(char *dst, char *src)
{
	int	i;

	if (!dst || !src)
		return (NULL);
	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}
