/***************************
*	CREER CHARGER SAUVER   *
***************************/

	/************
	 CREER IMAGE
	************/
BMP* newBMP(int width, int height)
{
	BMP* bmp 	= malloc(sizeof(BMP));
	bmp->width 	= width;
	bmp->height = height;
	bmp->data 	= calloc(1, width*height*sizeof(Pixel*));
	return bmp;
}

	/**************
	 CHARGER IMAGE
	**************/
BMP* loadBMP(const char* filename)
{

	Header 			header;
	Pixel 			p;
	int 			i, j, padding;
	unsigned char 	bgrpix[3];
	char 			corr[4] = {0,3,2,1};
	BMP* 			bmp;

	FILE* bmp_input = fopen(filename, "rb");
	if(!bmp_input)
	{
		printf("\nOuverture du fichier \"%s\" impossible.", filename);
		exit(1);
	}
	fread(&header, sizeof(Header), 1, bmp_input);

	bmp 	= newBMP(header.InfoHeader.Width, header.InfoHeader.Height);
	padding = corr[(3*header.InfoHeader.Width)%4];		// Une ligne de pixel doit TOUJOURS a voir un nombre d'octet multiple de 4.

	fseek(bmp_input, header.Offset, SEEK_SET);

	for(j=0; j<bmp->height; j++)
	{
		for(i=0; i<bmp->width; i++)
		{
			fread(&bgrpix, 1, 3, bmp_input);
			p.Red	= bgrpix[2];
			p.Green = bgrpix[1];
			p.Blue	= bgrpix[0];
			setPixel(bmp, i, bmp->height-j-1, p);
		}
		fread(&bgrpix, 1, padding, bmp_input);
	}
	fclose(bmp_input);

	return bmp;
}

	/*************
	 SAUVER IMAGE
	*************/
int saveBMP(BMP* bmp, const char* filename)
{
	Header			header;
	Pixel			p;
	int				i, j, padding, dataSize;
	unsigned char	bgrpix[3];
	char			corr[4] = {0,3,2,1};

	FILE* bmp_output = fopen(filename, "wb");
	if(!bmp_output)
	{
		printf("\nCreation du fichier \"%s\" impossible.", filename);
		exit(1);
	}
	memset(&header, 0, sizeof(Header));

	header.Type[0]				= 'B';
	header.Type[1]				= 'M';
	header.Offset				= sizeof(Header);
	header.InfoHeader.Size		= sizeof(InfoHeader);
	header.InfoHeader.Width		= bmp->width;
	header.InfoHeader.Height	= bmp->height;
	header.InfoHeader.Planes	= 1;
	header.InfoHeader.Bits		= 24;
	padding						= corr[(3*header.InfoHeader.Width)%4];
	dataSize					= 3*header.InfoHeader.Height*header.InfoHeader.Width + header.InfoHeader.Height*padding;
	header.InfoHeader.ImageSize	= dataSize;
	header.Size					= header.Offset + header.InfoHeader.ImageSize;

	fwrite(&header, sizeof(Header), 1, bmp_output);

	for(j=0; j<bmp->height; j++)
	{
		for(i=0; i<bmp->width; i++)
		{
			p 			= getPixel(bmp, i, bmp->height-j-1);
			bgrpix[0]	= p.Blue;
			bgrpix[1]	= p.Green;
			bgrpix[2]	= p.Red;
			fwrite(&bgrpix, 1, 3, bmp_output);
		}
		bgrpix[0] = bgrpix[1] = bgrpix[2] = 255;
		fwrite(&bgrpix, 1, padding, bmp_output);
	}
	fclose(bmp_output);
	return 0;
}

	/*************
	 COPIER IMAGE
	*************/
BMP* copyBMP(BMP* bmp)
{
	int i, j;
	BMP* new = newBMP(bmp->width, bmp->height);
	for(i=0; i<bmp->width; i++)
	{
		for(j=0; j<bmp->height; j++)
		{
			setPixel(new, i, j, getPixel(bmp, i, j));
		}
	}
	return new;
}
