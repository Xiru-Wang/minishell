# include <stdlib.h>
# include <stdio.h>
# include <limits.h>
# include <errno.h>
# include <unistd.h>
# include <stdbool.h>
# include <strings.h>
# include <fcntl.h>
# include <string.h>

char	*ft_strjoin(char *s1, char *s2);
char	*ft_substr(char const *s, unsigned int start, size_t len);

int main (int argc, char **argv)
{
	char 	*promt;
	int		fd_in;
	int		file_in;
	char 	*line;
	char 	buffer[2];
	int		num_byte;
	char	*tmp;
	char	*clean;
  	FILE 	*file_out;

	if (argc != 2)
		return (EXIT_FAILURE);

	file_out =	fopen(".tmp/exec_outp_clean.txt", "w" );
	file_in	=	open(".tmp/start.txt", O_RDONLY );
	fd_in	=	open (argv[1], O_RDONLY);

	line = strdup("");
	num_byte = read(file_in, &buffer, 1);
	line = ft_strjoin(line, strdup(buffer));
	while (!(buffer[0] == '\n' || num_byte == 0))
	{
		num_byte = read(file_in, &buffer, 1);
		if (num_byte == 0)
			break;
		buffer[1] = '\0';
		tmp = line;
		line = ft_strjoin(line, strdup(buffer));
		free(tmp);
	}
	promt = ft_substr(line, 0, strlen(line) - 5);

	line = strdup("");
	num_byte = read(fd_in, &buffer, 1);
	line = ft_strjoin(line, strdup(buffer));
	while (num_byte > 0)
	{
		while (!(buffer[0] == '\n' || num_byte == 0))
		{
			num_byte = read(fd_in, &buffer, 1);
			if (num_byte == 0)
				break;
			buffer[1] = '\0';
			tmp = line;
			line = ft_strjoin(line, strdup(buffer));
			free(tmp);
		}
		tmp = strstr(line, promt);
		if (tmp == NULL)
			fprintf(file_out, "%s", line);
		if (strstr(line, promt))
		{
			if (tmp != line)
			{
				clean = ft_substr(line, 0, tmp - line);
				fputs(clean, file_out);
			}
		}
		free (line);
		line = strdup("");
		num_byte = read(fd_in, &buffer, 1);
		line = ft_strjoin(line, strdup(buffer));	
	}

	close(fd_in);
   	fclose(file_out);
}


char *ft_strjoin(char *s1, char *s2) {
    char *str;
    size_t size;

    if (s1 == NULL || s2 == NULL) return NULL;

    size = strlen(s1) + strlen(s2) + 1;  // Total size needed, including null terminator
    str = (char *)calloc(size, sizeof(char));  // Allocate memory
    if (str == NULL) {
        return NULL;  // Return NULL if memory allocation fails
    }

    strcpy(str, s1);  // Copy the first string
    strcat(str, s2);  // Concatenate the second string

    return str;
}

char *ft_substr(char const *s, unsigned int start, size_t len) {
    char *str;
    size_t size;

    if (!s)
        return NULL;

    size = strlen(s);
    if (size < start)
        return calloc(1, sizeof(char)); // Allocate space for the empty string

    if ((len + start) > size)
        len = size - start; // Adjust length if it exceeds the string length after the start index

    str = malloc(sizeof(char) * (len + 1)); // Allocate memory for the substring + null terminator
    if (!str)
        return NULL;

    strncpy(str, s + start, len); // Copy the substring
    str[len] = '\0'; // Manually add null terminator

    return str;
}

