#include "get_next_line.h"

static int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static char	*ft_strjoin(char *str1, char *str2)
{
	char	*out;
	int	i;
	int	j;

	out = (char *)malloc(ft_strlen(str1) + ft_strlen(str2) + 1);
	i = 0;
	j = 0;
	while (str1[i])
	{
		out[i] = str1[i];
		i++;
	}
	while (str2[j])
	{
		out[i] = str2[j];
		i++;
		j++;
	}
	out[i] = '\0';
	return (out);
}

static char	*ft_substr(char *str, int start, size_t len)
{
	char	*out;
	int	i;

	out = (char *)malloc(len + 1);
	i = 0;
	while(str[start] && (size_t)i < len)
	{
		out[i] = str[start];
		i++;
		start++;
	}
	out[i] = '\0';
	return (out);
}

static char *ft_strchr(char *str, int c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (&str[i]);
		i++;
	}
	return (NULL);
}

static char *ft_strdup(char *str)
{
	int	i;
	char	*out;

	i = 0;
	out = (char *)malloc(ft_strlen(str) + 1);
	while (str[i])
	{
		out[i] = str[i];
		i++;
	}
	out[i] = '\0';
	return (out);
}

char	*get_next_line(int fd)
{
	char		*buffer;
	ssize_t		to_read;
	static char	*left;
	char		*to_return;
	char		*aux;

	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	buffer = (char *)malloc(BUFFER_SIZE + 1);
	to_read = read(fd, buffer, BUFFER_SIZE);
	if (to_read < 0)
	{
		free(buffer);
		return (NULL);
	}
	while (to_read > 0)
	{
		buffer[to_read] = '\0';
		if (!left)
			left = ft_strdup("");
		aux = ft_strjoin(left, buffer);
		if (left)
			free(left);
		left = ft_strdup(aux);
		if (ft_strchr(buffer, '\n'))
		{
			free(left);
			left = ft_substr(aux, ft_strlen(aux) - ft_strlen(ft_strchr(buffer, '\n') + 1),
				ft_strlen(ft_strchr(buffer, '\n')) - 1);
			to_return = ft_substr(aux, 0, ft_strlen(aux) - ft_strlen(ft_strchr(buffer, '\n') + 1));
			free(aux);
			free(buffer);
			return (to_return);
		}
		free(aux);
		to_read = read(fd, buffer, BUFFER_SIZE);
	}
	if (!left || ft_strlen(left) == 0)
	{
		free(buffer);
		return (NULL);
	}
	to_return = ft_strdup(left);
	free(left);
	left = NULL;
	free(buffer);
	return (to_return);
}

int main()
{
	int	fd;
	char	*line;

	fd = open("test1", O_RDONLY);
	while (line = get_next_line(fd))
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}
