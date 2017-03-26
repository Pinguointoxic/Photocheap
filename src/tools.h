	/**************
	 CONCATENATION
	**************/
char* concat(int count, ...)
{
    va_list ap;
    int i;

    int len = 1; // room for NULL
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
        len += strlen(va_arg(ap, char*));
    va_end(ap);

    char *merged = calloc(sizeof(char),len);
    int null_pos = 0;

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

	/**************
	 AFFICHER LOGO
	**************/
void printLogo()
{

}

