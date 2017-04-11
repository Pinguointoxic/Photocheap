/***************************
*		MODIFICATION	   *
***************************/

	/***************
	 NIVEAU DE GRIS
	***************/
BMP* greyScale(BMP* bmp)
{
	BMP* bmpTemp = copyBMP(bmp);
	unsigned char grey;
	Pixel p;
	int i, j;

	for(i=0; i<bmpTemp->width; i++)
	{
		for(j=0; j<bmpTemp->height; j++)
		{
			p		= getPixel(bmpTemp, i, j);
			grey	= greyIt(p, 0);
			p.Blue	= p.Red = p.Green = grey;
			setPixel(bmpTemp, i, j, p);
		}
	}
	return bmpTemp;
}

	/********
	 NEGATIF
	********/
BMP* invert(BMP* bmp)
{
	BMP* bmpTemp = copyBMP(bmp);
	int i, j;
	Pixel p;

	for(i=0; i<bmp->width; i++)
	{
		for(j=0; j<bmp->height; j++)
		{
			p 		= getPixel(bmpTemp, i, j);
			p.Red 	= 255-p.Red;
			p.Green = 255-p.Green;
			p.Blue 	= 255-p.Blue;
			setPixel(bmpTemp, i, j, p);
		}
	}
	return bmpTemp;
}

 	/*********
 	 CONTRAST
 	*********/
BMP* contrast(BMP* bmp, int cont)
{
	BMP* bmpTemp = copyBMP(bmp);
	int i, j;
	Pixel p, cp;
	float f;
	f  = (259*(cont + 255)) / (255*(259 - cont));

	for(i=0; i<bmp->width; i++)
	{
		for(j=0; j<bmp->height; j++)
		{
			p 			= getPixel(bmpTemp, i, j);
			cp.Red 		= trunc( f * (p.Red - 128) + 128 );
			cp.Green 	= trunc( f * (p.Green - 128) + 128 );
			cp.Blue 	= trunc( f * (p.Blue - 128) + 128 );
			setPixel(bmpTemp, i, j, cp);
		}
	}

	return bmpTemp;
}

 	/***********
 	 SATURATION
 	***********/
BMP* saturation(BMP* bmp, int sat)
{
	BMP*	bmpTemp = copyBMP(bmp);
	int		i, j;
	float	satu = sat/100.0;
	Pixel	p, sp;
	HSL		hsl;

	for(i=0; i<bmp->width; i++)
	{
		for(j=0; j<bmp->height; j++)
		{
			p	= getPixel(bmpTemp, i, j);
			hsl	= RGB2HSL(p);
			hsl.Sat += hsl.Sat * satu;
			if(hsl.Sat > 1.0)
				hsl.Sat = 1.0;
			if(hsl.Sat < 0.0)
				hsl.Sat = 0.0;

			p	= HSL2RGB(hsl);
			setPixel(bmpTemp, i, j, p);
		}
	}

	return bmpTemp;
}

	/*****************
	 CONTOURS - SOBEL
	*****************/
BMP* sobel(BMP* bmp)
{
	BMP*	bmpTemp = greyscale(copyBMP(bmp));
	BMP*	border = newBMP(bmp->width, bmp->height);
	Pixel	p;
	int		sobel_x[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
	int		sobel_y[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};
	int		i, j, px, py;

	for(i=1; i<bmp->width-1; i++)
	{
		for(j=1; j<bmp->height-1; j++)
		{
            px = 	(sobel_x[0][0] * getPixel(bmpTemp,i-1,j-1).Red)	+
            		(sobel_x[0][1] * getPixel(bmpTemp,i,j-1).Red)	+
            		(sobel_x[0][2] * getPixel(bmpTemp,i+1,j-1).Red)	+
					(sobel_x[1][0] * getPixel(bmpTemp,i-1,j).Red)	+
					(sobel_x[1][1] * getPixel(bmpTemp,i,j).Red)		+
					(sobel_x[1][2] * getPixel(bmpTemp,i+1,j).Red)	+
              		(sobel_x[2][0] * getPixel(bmpTemp,i-1,j+1).Red)	+
              		(sobel_x[2][1] * getPixel(bmpTemp,i,j+1).Red)	+
              		(sobel_x[2][2] * getPixel(bmpTemp,i+1,j+1).Red);

			py = 	(sobel_y[0][0] * getPixel(bmpTemp,i-1,j-1).Red)	+
					(sobel_y[0][1] * getPixel(bmpTemp,i,j-1).Red)	+
					(sobel_y[0][2] * getPixel(bmpTemp,i+1,j-1).Red)	+
              		(sobel_y[1][0] * getPixel(bmpTemp,i-1,j).Red)	+
              		(sobel_y[1][1] * getPixel(bmpTemp,i,j).Red)		+
              		(sobel_y[1][2] * getPixel(bmpTemp,i+1,j).Red)	+
              		(sobel_y[2][0] * getPixel(bmpTemp,i-1,j+1).Red)	+
              		(sobel_y[2][1] * getPixel(bmpTemp,i,j+1).Red)	+
              		(sobel_y[2][2] * getPixel(bmpTemp,i+1,j+1).Red);

			p.Red	= p.Green = p.Blue = (unsigned char)sqrt((px * px) + (py * py));
			setPixel(border, i, j, p);
		}
	}
	return border;
}

	/******************
	 CONTOURS - PEWITT
	*******************/
BMP* pewitt(BMP* bmp)
{
	BMP*	bmpTemp = greyscale(copyBMP(bmp));
	BMP*	border = newBMP(bmp->width, bmp->height);
	Pixel	p;
	int		pewitt_x[3][3] = {{-1,0,1},{-1,0,1},{-1,0,1}};
	int		pewitt_y[3][3] = {{-1,-1,-1},{0,0,0},{1,1,1}};
	int		i, j, px, py;

	for(i=1; i<bmp->width-1; i++)
	{
		for(j=1; j<bmp->height-1; j++)
		{
			px = 	(pewitt_x[0][0] * getPixel(bmpTemp,i-1,j-1).Red)+
					(pewitt_x[0][1] * getPixel(bmpTemp,i,j-1).Red)	+
					(pewitt_x[0][2] * getPixel(bmpTemp,i+1,j-1).Red)+
					(pewitt_x[1][0] * getPixel(bmpTemp,i-1,j).Red)	+
					(pewitt_x[1][1] * getPixel(bmpTemp,i,j).Red)	+
					(pewitt_x[1][2] * getPixel(bmpTemp,i+1,j).Red)	+
              		(pewitt_x[2][0] * getPixel(bmpTemp,i-1,j+1).Red)+
              		(pewitt_x[2][1] * getPixel(bmpTemp,i,j+1).Red)	+
              		(pewitt_x[2][2] * getPixel(bmpTemp,i+1,j+1).Red);

			py = 	(pewitt_y[0][0] * getPixel(bmpTemp,i-1,j-1).Red)+
					(pewitt_y[0][1] * getPixel(bmpTemp,i,j-1).Red)	+
					(pewitt_y[0][2] * getPixel(bmpTemp,i+1,j-1).Red)+
              		(pewitt_y[1][0] * getPixel(bmpTemp,i-1,j).Red)	+
              		(pewitt_y[1][1] * getPixel(bmpTemp,i,j).Red)	+
              		(pewitt_y[1][2] * getPixel(bmpTemp,i+1,j).Red)	+
              		(pewitt_y[2][0] * getPixel(bmpTemp,i-1,j+1).Red)+
              		(pewitt_y[2][1] * getPixel(bmpTemp,i,j+1).Red)	+
              		(pewitt_y[2][2] * getPixel(bmpTemp,i+1,j+1).Red);

			p.Red	= p.Green = p.Blue = (unsigned char)sqrt((px * px) + (py * py));
			setPixel(border, i, j, p);
		}
	}
	return border;
}

	/*****************
	 COLORISATION RGB (remplacé par ColorizeMIX)
	*****************/
BMP* colorizeRGB(BMP* bmp)
{
	BMP*	bmpTemp = copyBMP(bmp);
	FILE*	fp = fopen("color.txt", "r");
	Pixel	p;
	int		grey, i, j;
	int		tab[3][256] = {{0}};
	char	line[128];
	float	trash;

	if( fp == NULL)
		exit(EXIT_FAILURE);

	i=0;
	while(fgets(line, 1000, fp) != NULL)
	{
		sscanf(line, "%d %d %d %f %f %f", &tab[0][i],&tab[1][i],&tab[2][i], &trash, &trash, &trash);
		i++;
	}

	for(i=0; i<bmp->width; i++)
	{
		for(j=0; j<bmp->height; j++)
		{
			p		= getPixel(bmp, i, j);
			grey	= greyIt(p, 1);

			p.Red	= tab[0][grey];
			p.Green	= tab[1][grey];
			p.Blue	= tab[2][grey];
			setPixel(bmpTemp, i, j, p);
		}
	}
	fclose(fp);

	return bmpTemp;
}

	/*****************
	 COLORISATION HSL (remplacé par ColorizeMIX)
	*****************/
BMP* colorizeHSL(BMP* bmp)
{
	BMP*	bmpTemp = copyBMP(bmp);
	FILE*	fp = fopen("color.txt", "r");
	Pixel	p;
	HSL		hsl;
	int		grey, i, j;
	int		trash;
	float	small = 256.0;
	int		smallCoord[2] = {0};
	float	tab[3][256] = {{0.0}};
	char	line[128];

	if( fp == NULL)
		exit(EXIT_FAILURE);

		TEST;
	i=0;
	while(fgets(line, 1000, fp) != NULL)
	{
		sscanf(line, "%d %d %d %f %f %f", &trash, &trash, &trash, &tab[0][i],&tab[1][i],&tab[2][i]);
		i++;
	}
	for(i=0; i<bmp->width; i++)
	{
		for(j=0; j<bmp->height; j++)
		{
			p		= getPixel(bmp, i, j);
			hsl		= RGB2HSL(p);
			grey	= greyIt(p, 1);

			hsl.Hue		= tab[0][grey];
			hsl.Sat		= tab[1][grey];
			hsl.Light	= tab[2][grey];
			p = HSL2RGB(hsl);
			
			setPixel(bmpTemp, i, j, p);
		}
	}
	fclose(fp);

	return bmpTemp;
}

BMP* colorizeMIX(BMP* bmp)
{
	BMP*	bmpTemp = copyBMP(bmp);
	BMP*	bmpDelta = copyBMP(bmp);
	FILE*	fp = fopen("color.txt", "r");
	Pixel	p, cp1, cp2;
	Pixel	pdelta;
	HSL		hsl;
	int		grey, i, j;
	int		borne = 0;
	int		smallCoord;
	float	small = 300.0;
	char	line[128];
	
	int		tabRGB[3][256] = {{0}};
	float	tabHSL[3][256] = {{0.0}};

	float	tab[256][256] = {{0.0}};
	float	tab2[256] = {0.0};


	if( fp == NULL)
		exit(EXIT_FAILURE);

	i=0;
	while(fgets(line, 1000, fp) != NULL) // recupération de chaque lignes du fichier pour la mettre dans tabRG et tabHSL
	{
		sscanf(line, "%d %d %d %f %f %f", &tabRGB[0][i],&tabRGB[1][i],&tabRGB[2][i], &tabHSL[0][i],&tabHSL[1][i],&tabHSL[2][i]);
		i++;
	}

	for(borne = 0; borne<256; borne++) // On créer toutes les images possible avec toutes les bornes/pivots possibles
	{
		for(i=0; i<bmp->width; i++)
		{
			for(j=0; j<bmp->height; j++)
			{
				p		= getPixel(bmp, i, j);
				hsl		= RGB2HSL(p);
				grey	= greyIt(p, 0);

				if(grey >= borne)
				{
					hsl.Hue		= tabHSL[0][grey];
					hsl.Sat		= tabHSL[1][grey];
					hsl.Light	= tabHSL[2][grey];
					cp1 = HSL2RGB(hsl);
				}
				else
				{
					cp1.Red		= tabRGB[0][grey];
					cp1.Green	= tabRGB[1][grey];
					cp1.Blue	= tabRGB[2][grey];
				}

				pdelta.Red		= abs(p.Red - cp1.Red);
				pdelta.Green	= abs(p.Green - cp1.Green);
				pdelta.Blue		= abs(p.Blue - cp1.Blue);
				setPixel(bmpTemp, i, j, pdelta);
				setPixel(bmpDelta, i, j, pdelta);
			}
	
			//saveBMP(bmpDelta, "./Gallery/delta.bmp"); // enregistre l'image 'delta' pour chaque borne/pivot.
		}
		tab2[borne] = meanPixel(bmpTemp); // pour chaque bornes, on stock dans tab2 la couleur moyenne de l'image en cours (R+V+B)/3
	}

	for(i=0; i<256; i++) // on cherche la plus petite valeur et sa coordonnée
	{
		if(tab2[i] <= small)
		{
			small		= tab2[i];
			smallCoord	= i;
		}
	}

	for(i=0; i<bmp->width; i++) // Créer l'image la plus proche de la source avec la meilleure borne
	{
		for(j=0; j<bmp->height; j++)
		{
			p		= getPixel(bmp, i, j);
			hsl		= RGB2HSL(p);
			grey	= greyIt(p, 1);

			if(grey >= smallCoord)
			{
				hsl.Hue		= tabHSL[0][grey];
				hsl.Sat		= tabHSL[1][grey];
				hsl.Light	= tabHSL[2][grey];
				p = HSL2RGB(hsl);

				setPixel(bmpTemp, i, j, p);
			}
			else
			{
				p.Red	= tabRGB[0][grey];
				p.Green	= tabRGB[1][grey];
				p.Blue	= tabRGB[2][grey];

				setPixel(bmpTemp, i, j, p);
			}
		}
	}
	saveBMP(bmpTemp, concat(3, "./Gallery/", toString(smallCoord), ".bmp"));
	fclose(fp);

	return bmpTemp;
}
