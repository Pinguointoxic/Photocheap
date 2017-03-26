/*********************************
Robin Dell et Emeline Ehles
MIT licence
*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <dirent.h>
#include <stdarg.h>

#pragma pack(1)			// "Compresse" la structure en mémoire, utile pour avoir la VRAIE taille du "header"
#include "./src/header.h"		// Header, contient les definitions des fonctions et structure
#include "./src/tools.h"

/***************************
*	 PIXEL MANIPULATION	   *
***************************/

	/****************
	 RECUPERER PIXEL
	****************/
void setPixel(BMP* bmp, int i, int j, Pixel p)
{
	assert(bmp && i>=0 && i<bmp->width && j>=0 && j<bmp->height);
	bmp->data[bmp->width*j+i] = p;
}

	/***************
	 ASSIGNER PIXEL
	***************/
Pixel getPixel(BMP* bmp, int i, int j)
{
	assert(bmp && i>=0 && i<bmp->width && j>=0 && j<bmp->height);
	return bmp->data[bmp->width*j+i];
}

	/***************
	 	RGB2HSL
	***************/
HSL RGB2HSL(Pixel p)
{
	HSL 	hsl;
	float	R=((float)p.Red / 255.0), G=((float)p.Green / 255.0), B=((float)p.Blue / 255.0);
	float	r, g, b;
	float	max		= max(R, max(G, B));
	float	min		= min(R, min(G, B));
	float	delta 	= max-min;

	hsl.Light = (max+min)/2.0;

	if(delta == 0.0)
		hsl.Hue = hsl.Sat = 0.0;
	else
	{
		if(hsl.Light < 0.5)
			hsl.Sat = delta / (max+min);
		else
			hsl.Sat = delta / (2-delta);

		r = ((max - R) / 6.0 ) + (delta / 2.0) / delta;
		g = ((max - G) / 6.0 ) + (delta / 2.0) / delta;
		b = ((max - B) / 6.0 ) + (delta / 2.0) / delta;

		if(R == max)
			hsl.Hue = b-g;
		else if(G == max)
			hsl.Hue = (1/3.0) + r-b;
		else if(B == max)
			hsl.Hue = (2/3.0) + g-r;

		if(hsl.Hue < 0.0) hsl.Hue +=1;
		if(hsl.Hue > 1.0) hsl.Hue -=1;
	}
	return hsl;
}

	/***************
	 	HSL2RGB
	***************/
Pixel HSL2RGB(HSL hsl)
{
	Pixel p;
	float a, b;

	if(hsl.Sat == 0.0)
	{
		p.Red	= hsl.Light * 255;
		p.Green = hsl.Light * 255;
		p.Blue	= hsl.Light * 255;
	}
	else
	{
		if(hsl.Light < 0.5)
			b = hsl.Light * (1 + hsl.Sat);
		else
			b = (hsl.Light+hsl.Sat) - (hsl.Light*hsl.Sat);

		a = 2 * hsl.Light - b;

		p.Red	= 255 * Hue2RGB(a, b, hsl.Hue+(1/3.0));
		p.Green = 255 * Hue2RGB(a, b, hsl.Hue);
		p.Blue	= 255 * Hue2RGB(a, b, hsl.Hue-(1/3.0));
	}
	return p;
}

float Hue2RGB(float a, float b, float h)
{
	if(h < 0.0) h +=1;
	if(h > 1.0) h -=1;
	if(( 6 * h ) < 1.0 ) return a + (b - a) * 6 * h;
	if(( 2 * h ) < 1.0 ) return b;
	if(( 3 * h ) < 2.0 ) return a + (b - a) * ((2/3.0) - h) * 6;
	return a;
}


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
			grey	= p.Red*0.2125 + p.Green*0.7154 + p.Blue*0.0721;
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
	BMP* bmpTemp = copyBMP(bmp);
	int i, j;
	float satu = sat/100.0;
	Pixel p, sp;
	HSL hsl;

	for(i=0; i<bmp->width; i++)
	{
		for(j=0; j<bmp->height; j++)
		{
			p = getPixel(bmpTemp, i, j);
			hsl = RGB2HSL(p);
			hsl.Sat += hsl.Sat * satu;
			if(hsl.Sat > 1.0)
				hsl.Sat = 1.0;
			if(hsl.Sat < 0.0)
				hsl.Sat = 0.0;

			p = HSL2RGB(hsl);
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
	BMP* bmpTemp = copyBMP(bmp);
	bmpTemp = greyScale(bmpTemp);

	BMP* border = newBMP(bmp->width, bmp->height);
	Pixel 		p;
	int 		sobel_x[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
	int 		sobel_y[3][3] = {{-1,-2,-1},{0,0,0},{1,2,1}};
	int 		i, j, px, py;

	for(i=1; i<bmp->width-1; i++)
	{
		for(j=1; j<bmp->height-1; j++)
		{
            px = 	(sobel_x[0][0] * getPixel(bmpTemp,i-1,j-1).Red) + (sobel_x[0][1] * getPixel(bmpTemp,i,j-1).Red) + (sobel_x[0][2] * getPixel(bmpTemp,i+1,j-1).Red) 	+
					(sobel_x[1][0] * getPixel(bmpTemp,i-1,j).Red)   + (sobel_x[1][1] * getPixel(bmpTemp,i,j).Red)   + (sobel_x[1][2] * getPixel(bmpTemp,i+1,j).Red) 	+
              		(sobel_x[2][0] * getPixel(bmpTemp,i-1,j+1).Red) + (sobel_x[2][1] * getPixel(bmpTemp,i,j+1).Red) + (sobel_x[2][2] * getPixel(bmpTemp,i+1,j+1).Red);

			py = 	(sobel_y[0][0] * getPixel(bmpTemp,i-1,j-1).Red) + (sobel_y[0][1] * getPixel(bmpTemp,i,j-1).Red) + (sobel_y[0][2] * getPixel(bmpTemp,i+1,j-1).Red) 	+
              		(sobel_y[1][0] * getPixel(bmpTemp,i-1,j).Red)   + (sobel_y[1][1] * getPixel(bmpTemp,i,j).Red)   + (sobel_y[1][2] * getPixel(bmpTemp,i+1,j).Red) 	+
              		(sobel_y[2][0] * getPixel(bmpTemp,i-1,j+1).Red) + (sobel_y[2][1] * getPixel(bmpTemp,i,j+1).Red) + (sobel_y[2][2] * getPixel(bmpTemp,i+1,j+1).Red);

			p.Red = p.Green = p.Blue = (unsigned char)sqrt((px * px) + (py * py));
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
	BMP* bmpTemp = copyBMP(bmp);
	bmpTemp = greyScale(bmpTemp);

	BMP* 	border = newBMP(bmp->width, bmp->height);
	Pixel 	p;
	int 	pewitt_x[3][3] = {{-1,0,1},{-1,0,1},{-1,0,1}};
	int 	pewitt_y[3][3] = {{-1,-1,-1},{0,0,0},{1,1,1}};
	int 	i, j, px, py;

	for(i=1; i<bmp->width-1; i++)
	{
		for(j=1; j<bmp->height-1; j++)
		{
			px = 	(pewitt_x[0][0] * getPixel(bmpTemp,i-1,j-1).Red) + (pewitt_x[0][1] * getPixel(bmpTemp,i,j-1).Red) + (pewitt_x[0][2] * getPixel(bmpTemp,i+1,j-1).Red) 	+
					(pewitt_x[1][0] * getPixel(bmpTemp,i-1,j).Red)   + (pewitt_x[1][1] * getPixel(bmpTemp,i,j).Red)   + (pewitt_x[1][2] * getPixel(bmpTemp,i+1,j).Red) 	+
              		(pewitt_x[2][0] * getPixel(bmpTemp,i-1,j+1).Red) + (pewitt_x[2][1] * getPixel(bmpTemp,i,j+1).Red) + (pewitt_x[2][2] * getPixel(bmpTemp,i+1,j+1).Red);

			py = 	(pewitt_y[0][0] * getPixel(bmpTemp,i-1,j-1).Red) + (pewitt_y[0][1] * getPixel(bmpTemp,i,j-1).Red) + (pewitt_y[0][2] * getPixel(bmpTemp,i+1,j-1).Red) 	+
              		(pewitt_y[1][0] * getPixel(bmpTemp,i-1,j).Red)   + (pewitt_y[1][1] * getPixel(bmpTemp,i,j).Red)   + (pewitt_y[1][2] * getPixel(bmpTemp,i+1,j).Red) 	+
              		(pewitt_y[2][0] * getPixel(bmpTemp,i-1,j+1).Red) + (pewitt_y[2][1] * getPixel(bmpTemp,i,j+1).Red) + (pewitt_y[2][2] * getPixel(bmpTemp,i+1,j+1).Red);
			p.Red = p.Green = p.Blue = (unsigned char)sqrt((px * px) + (py * py));
			setPixel(border, i, j, p);
		}
	}
	return border;
}

	/*****************
	 COLORISATION RGB
	*****************/
BMP* colorizeRGB(BMP* bmp)
{
	BMP* bmpTemp = copyBMP(bmp);
	FILE* fp = fopen("./src/spring/colorRGB.txt", "r");
	Pixel p;
	int grey, i, j;
	int tab[3][256] = {{0}};
	char line[128];


	if( fp == NULL)
		exit(EXIT_FAILURE);

	i=0;
	while(fgets(line, 1000, fp) != NULL)
	{
		sscanf(line, "%d %d %d", &tab[0][i],&tab[1][i],&tab[2][i]);
		i++;
	}

	for(i=0; i<bmp->width; i++)
	{
		for(j=0; j<bmp->height; j++)
		{
			p = getPixel(bmp, i, j);
			grey = p.Red*0.2125 + p.Green*0.7154 + p.Blue*0.0721;

			p.Red = tab[0][grey];
			p.Green = tab[1][grey];
			p.Blue = tab[2][grey];
			setPixel(bmpTemp, i, j, p);
		}
	}
	fclose(fp);

	return bmpTemp;
}

	/*****************
	 COLORISATION HSL
	*****************/
BMP* colorizeHSL(BMP* bmp)
{
	BMP* bmpTemp = copyBMP(bmp);
	FILE* fp = fopen("./src/spring/colorHSL.txt", "r");
	Pixel p;
	HSL hsl;
	int grey, i, j;
	float tab[3][256] = {{0.0}};
	char line[128];

	if( fp == NULL)
		exit(EXIT_FAILURE);

		TEST;
	i=0;
	while(fgets(line, 1000, fp) != NULL)
	{
		sscanf(line, "%f %f %f", &tab[0][i],&tab[1][i],&tab[2][i]);
		i++;
	}
	for(i=0; i<bmp->width; i++)
	{
		for(j=0; j<bmp->height; j++)
		{
			p = getPixel(bmp, i, j);
			hsl = RGB2HSL(p);
			grey = p.Red*0.2125 + p.Green*0.7154 + p.Blue*0.0721;

			hsl.Hue = tab[0][grey];
			hsl.Sat = tab[1][grey];
			hsl.Light = tab[2][grey];
			p = HSL2RGB(hsl);
			setPixel(bmpTemp, i, j, p);
		}
	}
	fclose(fp);

	return bmpTemp;
}


/***************************
*		STATISTIQUES	   *
***************************/

	/************
	 HISTOGRAMME
	************/
void histogram(BMP* bmp)
{
	int 	i, j, k;
	int		MAXR=0, MAXG=0, MAXB=0, MAX=0;
	int		histo[4][256] = {{0}}; // ligne 0->red -- ligne 1->green -- ligne 3->blue --- ligne 4->gris
	Pixel 	p;
	int		greyp;
	BMP* 	histoR;
	BMP*	histoG;
	BMP*	histoB;
	BMP*	histoGrey;

	// Remplissage de l'histogramme et recherche du plus grand nombre de pixel de la meme couleur
	for(i=0; i<bmp->width; i++)
	{
		for(j=0; j<bmp->height; j++)
		{
			p = getPixel(bmp, i, j);
			greyp = p.Red*0.2125 + p.Green*0.7154 + p.Blue*0.0721;
			histo[0][p.Red]++;
			histo[1][p.Green]++;
			histo[2][p.Blue]++;
			histo[3][greyp]++;

			if( histo[0][p.Red]		> MAXR ) 	MAXR = histo[0][p.Red];
			if( histo[1][p.Green] 	> MAXG ) 	MAXG = histo[1][p.Green];
			if( histo[2][p.Blue] 	> MAXB ) 	MAXB = histo[2][p.Blue];
			if( histo[3][greyp] 	> MAX  ) 	MAX  = histo[3][greyp];
		}
	}

	// Création de l'image histogramme
	histoR 		= newBMP(256,200);
	histoG 		= newBMP(256,200);
	histoB 		= newBMP(256,200);
	histoGrey 	= newBMP(256,200);

	FILE* f1 = fopen("histograms/histogram.txt", "w");
	fprintf(f1, "VALUE	RED	GREEN	BLUE GREY\n");
	for(i=0; i<256; i++)
	{
		fprintf(f1, "%d	%d	%d	%d	%d\n",i, (histo[0][i]*200/MAXR), (histo[1][i]*200/MAXG), (histo[2][i]*200/MAXB), (histo[3][i]*200/MAX) ) ;
		for(j=199; j>=0; j--)
		{
			if ( histo[0][i]*200/MAXR > j)
				setPixel(histoR, i, 199-j, RED);
			else
				setPixel(histoR, i, 199-j, WHITE);

			if ( histo[1][i]*200/MAXG > j)
				setPixel(histoG, i, 199-j, GREEN);
			else
				setPixel(histoG, i, 199-j, WHITE);

			if ( histo[2][i]*200/MAXB > j)
				setPixel(histoB, i, 199-j, BLUE);
			else
				setPixel(histoB, i, 199-j, WHITE);

			if ( histo[3][i]*200/MAX > j)
				setPixel(histoGrey, i, 199-j, GREY);
			else
				setPixel(histoGrey, i, 199-j, WHITE);
		}
	}

	fclose(f1);
	saveBMP(histoR, "histograms/histogram_Red.bmp");
	saveBMP(histoG, "histograms/histogram_Green.bmp");
	saveBMP(histoB, "histograms/histogram_Blue.bmp");
	saveBMP(histoGrey, "histograms/histogram_Grey.bmp");

}

	/***************************************
	 MOYENNE DES COULEUR SUR GROUPE D'IMAGE
	***************************************/
int meanColorOfFolder(int season, char* folderName)
{
	DIR *dir;
	FILE *fpRGB, *fpHSL;
	BMP *bmpTemp;
	Pixel p;
	HSL hsl;
	int tab[6][256] = {{0}};		// R G B
	float tab2[6][256] = {{0}};		// H S L
	float tabPic[6][256] = {{0}};	// R G B nbR nbG nB
	float tabPic2[6][256] = {{0}};	//  H S L nbH nbS nbL
	int grey, i, j, nbIm=0;
	struct dirent *ent;

	if(season == 1)
	{
		fpRGB = fopen("./src/spring/colorRGB.txt", "w+");
		fpHSL = fopen("./src/spring/colorHSL.txt", "w+");
	}

	else if(season == 2)
	{
		fpRGB = fopen("./src/summer/colorRGB.txt", "w+");
		fpHSL = fopen("./src/summer/colorHSL.txt", "w+");
	}
	else if(season == 3)
	{
		fpRGB = fopen("./src/fall/colorRGB.txt", "w+");
		fpHSL = fopen("./src/fall/colorHSL.txt", "w+");
	}
	else
	{
		fpRGB = fopen("./src/winter/colorRGB.txt", "w+");
		fpHSL = fopen("./src/winter/colorHSL.txt", "w+");
	}

	if ((dir = opendir (folderName)) != NULL)
	{
		while ((ent = readdir (dir)) != NULL)
		{
			if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0  && strcmp(ent->d_name, "colorRGB.txt") != 0 && strcmp(ent->d_name, "colorHSL.txt")!= 0)
			{
				bmpTemp = loadBMP(concat(2, folderName, ent->d_name));
				for(i=0; i<bmpTemp->width; i++)
				{
					for(j=0; j<bmpTemp->height; j++)
					{

						p = getPixel(bmpTemp, i, j);
						hsl = RGB2HSL(p);
						grey = p.Red*0.2125 + p.Green*0.7154 + p.Blue*0.0721;
						// RGB
						tabPic[0][grey] += p.Red;	tabPic[3][grey]++;
						tabPic[1][grey] += p.Green;	tabPic[4][grey]++;
						tabPic[2][grey] += p.Blue;	tabPic[5][grey]++;

						// HSL
						tabPic2[0][grey] += hsl.Hue;	tabPic2[3][grey]++;
						tabPic2[1][grey] += hsl.Sat;	tabPic2[4][grey]++;
						tabPic2[2][grey] += hsl.Light;	tabPic2[5][grey]++;
					}
				}
				for(i=0; i<256; i++) // Remplissage du tableau avec la couleur "moyenne" de chaque valeur de gris de l'image en cours
				{
					if(tabPic[3][i] != 0)
						tab[0][i] += tabPic[0][i] / tabPic[3][i];
					else
						tab[0][i] = 0;

					if(tabPic2[3][i] != 0)
						tab2[0][i] += tabPic2[0][i] / tabPic2[3][i];
					else
						tab2[0][i] = 0;

					if(tabPic[4][i] != 0)
						tab[1][i] += tabPic[1][i] / tabPic[4][i];
					else
						tab[1][i] = 0;

					if(tabPic2[4][i] != 0)
						tab2[1][i] += tabPic2[1][i] / tabPic2[4][i];
					else
						tab2[1][i] = 0;

					if(tabPic[5][i] != 0)
						tab[2][i] += tabPic[2][i] / tabPic[5][i];
					else
						tab[2][i] = 0;

					if(tabPic2[5][i] != 0)
						tab2[2][i] += tabPic2[2][i] / tabPic2[5][i];
					else
						tab2[2][i] = 0;

				}
				for(i=0; i<256; i++) // Remise a 0 du tableau propre a l'image en cours
				{
					for(j=0; j<6; j++)
					{
						tabPic[j][i] = 0;
						tabPic2[j][i] = 0.0;
					}
				}
				printf("image n: %d.\n", ++nbIm);
			}
		}
		closedir (dir);
	}
	else
	{
		perror ("");
		return EXIT_FAILURE;
	}

	for(i=0; i<256; i++) // Moyenne finale des couleurs par le nombre d'image totale anlyser
	{
		tab[0][i] /= (1.0*nbIm);
		tab[1][i] /= (1.0*nbIm);
		tab[2][i] /= (1.0*nbIm);

		tab2[0][i] /= (1.0*nbIm);
		tab2[1][i] /= (1.0*nbIm);
		tab2[2][i] /= (1.0*nbIm);
		fprintf(fpRGB, "%d %d %d\n", tab[0][i], tab[1][i], tab[2][i]);
		fprintf(fpHSL, "%f %f %f\n", tab2[0][i], tab2[1][i], tab2[2][i]);
	}
	fclose(fpRGB);
	fclose(fpHSL);

	return 1;
}


int main()
{
	char nameimage[200], buf[200];
	int choice = 0, param=0, i=1;
	BMP* image;
	BMP* image2;

	CLEAR;
	printf("####################################\n");
	printf("#                                  #\n");
	printf("#     Bonjour,                     #\n");
	printf("#     Bienvenue sur Photocheap     #\n");
	printf("#                                  #\n");
	printf("####################################\n\n");
	printf("   Nous pouvons vous proposer differentes modifications pour de plus belles images\n\n");

	while(i){

		printf("  Veuillez donnez le nom de votre image:\n");
		scanf("%s",nameimage);
		image = loadBMP(nameimage);
		printf("\n");
		printf("  Veuillez choisir vos modification :\n \n");
		printf("  " CBLU"1."CRESET" Niveau de gris\n  "
					CBLU"2."CRESET" Negatif\n  "
					CBLU"3."CRESET" Contrast\n  "
					CBLU"4."CRESET" Saturation\n  "
					CBLU"5."CRESET" Contours\n  "
					CBLU"6."CRESET" Histogramme\n  "
					CBLU"7."CRESET" Moyennage des couleurs\n  "
					CBLU"8."CRESET" Colorisation\n");

		scanf("%d", &choice);
		switch(choice){
			case 1:
				image = greyScale(image);
				saveBMP(image, concat(2,"./Gallery/greyscale_", nameimage));
			break;
			case 2:
				image = invert(image);
				saveBMP(image, concat(2, "./Gallery/invert_", nameimage));
			break;
			case 3:
				printf("  Donnez la valeur des contraste que vous desirez: \n");
				scanf("%d", &param);
				image = contrast(image,param);
				saveBMP(image, concat(4, "./Gallery/contraste(",sprintf(buf, "%d", param),")_", nameimage));
			break;
			case 4:
				printf("  Donnez la valeur de saturation que vous desirez: \n");
				scanf("%d", &param);
				image = saturation(image, param);
				saveBMP(image, concat(4, "./Gallery/saturation(", sprintf(buf, "%d", param), ")_", nameimage));
			break;
			case 5:
				image = pewitt(image);
				saveBMP(image, "./Gallery/contour.bmp");
			break;
			case 6:
				histogram(image);
			break;
			case 7:
				printf("\n   Choisizzez la saison :\n1. Printemps\n2. Ete\n3. Automne\n4. Hiver\n");
				scanf("%d", &choice);
				switch(choice)
				{
					case 1:
					meanColorOfFolder(1, "./src/spring/");
					break;
					case 2:
					meanColorOfFolder(2, "./src/summer/");
					break;
					case 3:
					meanColorOfFolder(3, "./src/fall/");
					break;
					case 4:
					meanColorOfFolder(4, "./src/winter/");
					break;
				}
			break;
			case 8:
				image2 = colorizeHSL(image);
				saveBMP(image2, concat(2, "./Gallery/colorized_HSL_", nameimage));
				image2 = colorizeRGB(image);
				saveBMP(image2, concat(2, "./Gallery/colorized_RGB_", nameimage));
			break;
		}
		printf(CGRN "  Vos modification on bien ete effectue \n" CRESET);
		printf("  Voulez vous continuez a modifier vos images?\n");
		printf(CGRN"  	1.oui"CRED"	0.non\n"CRESET);
		scanf("%d",&i);
	}

	printf("  Merci de votre visite et a bientot ! :)\n");

	return 0;
}
