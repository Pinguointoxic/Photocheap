#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdarg.h>       // va_*

char* concat(int count, ...)
{
    va_list ap;
    int i;

    // Find required length to store merged string
    int len = 1; // room for NULL
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
        len += strlen(va_arg(ap, char*));
    va_end(ap);

    // Allocate memory to concat strings
    char *merged = calloc(sizeof(char),len);
    int null_pos = 0;

    // Actually concatenate strings
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
    {
        char *s = va_arg(ap, char*);
        strcpy(merged+null_pos, s);
        null_pos += strlen(s);
    }
    va_end(ap);

    return merged;
}

int main()
{
	DIR *dir;
	FILE* fp = fopen("fall.txt", "w");
	int i = 1;
	struct dirent *ent;
	if ((dir = opendir ("./img/")) != NULL)
	{
		while ((ent = readdir (dir)) != NULL)
		{
			if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
				fprintf (fp, "%d %s\n", i, ent->d_name);
				i++;
		}
		closedir (dir);
	}
	else
	{
		perror ("");
	return EXIT_FAILURE;
	}
	
	printf("%s", concat(2, "Bonjour ", "Etienne"));
	
	return 0;
}

