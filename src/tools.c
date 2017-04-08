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
	printf(
		CBLU"pcpppcccpppcccpcpcpcpppcccpppcccpcpcpcpcp \n"
		"ppcccppppcccpcpcpcppcpcpcccccppppccpcpcpc \n"
		"ppcp                                 ppcc \n"
		"ppcp                                 pcpc \n"
		"pccp       ppcpcp                    pcpp \n"
		"pcpc    pccpcpcpcpp                  ppcc \n"
		"ppcc     pcp   ppcc   cppccpcpc      pccc \n"
		"ccpp     pcc   pcpp  pcpcpppccc      pccp \n"
		"cppp     pcpcpcccp  pccc             pccc \n"
		"ppcc    ppcpc       pcpp             ccpp \n"
		"ccpc     pppp       ppccp    ccp     ccpp \n"
		"cpcp     ppc        ppccppcpcpc      pccc \n"
		"cccc     cpc         pcpcppcc        pppp \n"
		"pppp     pcp                         cpcp \n"
		"cpcp                                 ccpp \n"
		"ppcc"CRESET"      Bienvenue sur Photocheap" CBLU"   cpcp \n"
		"pcpcpcpcpppcppcpcppcpcppcpcpcpppcppcpcppc \n"
		"cppcpcccppcpcppcpcpcpcpcppcccppcppppcppcp \n" CRESET
		"\n"
		"\n"
	);

}

float greyIt(Pixel p, int mode)
{
	if(mode == 0) // CIE
		return p.Red*0.2125 + p.Green*0.7154 + p.Blue*0.0721;
	else
		return (p.Red+p.Blue+p.Green)/3;
}

char* toString(int a)
{
	char* str = malloc(sizeof(int));
	sprintf(str, "%d", a);
	return str;
	free(str);
}
