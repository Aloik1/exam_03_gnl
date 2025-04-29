#include "get_next_line.h"

static void	free_and_null(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
	{
		free(matrix[i]);
		matrix[i] = NULL;
		i++;
	}
	free(matrix);
	return ;
}

static int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static char	*ft_substr(char *str, int start, size_t len)
{
	char	*result;
	int	i;

	i = 0;
	if (!str)
		return (NULL);
	result = (char *)malloc(sizeof(char) * ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	while ((size_t)i < len)
	{
		result[i] = str[start];
		i++;
		start++;
	}
	result[i] = '\0';
	return (result);
}

static char	**ft_split(char *str, char c)
{
	char	**matrix;
	int	i;
	int	j;
	int	start;
	int	words;

	i = 0;
	words = 1;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == c)
			words++;
		i++;
	}
	matrix = (char **)malloc(sizeof(char *) * ft_strlen(str));
	i = 0;
	start = 0;
	j = 0;
	while (i < ft_strlen(str) && str[i] && j < words)
	{
		while (str[i] && str[i] == c)
			i++;
		while (str[i] && str[i] != c)
			i++;
		matrix[j] = ft_substr(str, start, i - start);
		j++;
		start = i;
//		}
		i++;
	}
	matrix[j] = NULL;
	return (matrix);
}

static char	*ft_strchr(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == c)
			return (&str[i]);
		i++;
	}
	return (NULL);
}

static char	*ft_strjoin(char *str, char *to_join)
{
	char	*result;
	int 	i;
	int	j;

	if (!str || !to_join)
		return (NULL);
	result = (char *)malloc(sizeof(char) * (ft_strlen(str) + ft_strlen(to_join) + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		result[i] = str[i];
		i++;
	}
	while (to_join[j])
	{
		result[i] = to_join[j];
		i++;
		j++;
	}
	result[i] = '\0';
	return (result);
}

static char 	*ft_strdup(char *str)
{
	char	*cpy;
	int	i;

	i = 0;
	if (!str)
		return (NULL);
	cpy = (char *)malloc(sizeof(char) * ft_strlen(str) + 1);
	while (str[i])
	{
		cpy[i] = str[i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}

char	*get_next_line(int fd)
{
	char		*buffer; //save whatever is read
	ssize_t 	to_read; //number of bytes read
	static char	*line;   //line left from previous read
	char	*out;		 //line to be returned

	to_read = 0;
	if (BUFFER_SIZE <= 0 || fd < 0)
		return (NULL);
	buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	to_read = read(fd, buffer, BUFFER_SIZE);
	out = ft_strdup("");
	if (to_read <= 0)
	{
		free(buffer);
		free(out);
		line = NULL;
		return (NULL);
	}
	char	*aux;
	line = ft_strdup("");
	while (to_read > 0)
	{
		buffer[to_read] = '\0';
		if (!line)
		{
			free(out);
			free(buffer);
			return (NULL);
		}
		aux = ft_strjoin(line, buffer);
		free(line);
		line = ft_strdup(aux);
		free(aux);
		if (ft_strchr(line, '\n') && ft_strlen(line) != 1)
		{
			char	**separated;
			separated = ft_split(line, '\n');
			int	i;
			i = 0;
			if (i > 1)
			{
				char	*temp;
				i = 1;
				free(line);
				line = ft_strdup("");
				while (separated[i])
				{
					temp = ft_strjoin(line, "\n");
					free(line);
					line = ft_strdup(temp);
					free(temp);
					temp = ft_strjoin(line, separated[i]);
					free(line);
					line = ft_strdup(temp);
					free(temp);
					i++;
				}
				temp = ft_strjoin(separated[0], "\n");
				free(out);
				out = ft_strdup(temp);
				free(temp);
				free(buffer);
				free_and_null(separated);
				return (out);
			}
			free_and_null(separated);
			free(out);
			free(buffer);
			return (line);
		}
		to_read = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	if (ft_strlen(line) == 0)
	{
		free(out);
		return (NULL);
	}
	free(out);
	return (line);
}

int main(void)
{
	int fd;
	char *line;

	fd = open("test1", O_RDONLY);
	if (fd < 0)
		return (printf("Fd cant be openned\n"), 1);

	while ((line = get_next_line(fd)))
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}