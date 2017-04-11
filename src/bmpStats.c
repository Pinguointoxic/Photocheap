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

	FILE* f1 = fopen("./Histograms/histogram.txt", "w");
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
	saveBMP(histoR, "./Histograms/histogram_Red.bmp");
	saveBMP(histoG, "./Histograms/histogram_Green.bmp");
	saveBMP(histoB, "./Histograms/histogram_Blue.bmp");
	saveBMP(histoGrey, "./Histograms/histogram_Grey.bmp");

}
	/******************************************************
	 MOYENNE DES COULEURS PAR NIVEAU DE GRIS SUR UNE IMAGE
	******************************************************/
int meanColorBMProb(BMP* bmp)
{
	BMP* bmpTemp = copyBMP(bmp);
	FILE* fp = fopen("color.txt", "w+");
	Pixel p;
	HSL hsl;
	int i, j, k, l, m, n;
	int grey, nbTotal=0;

	int		tabRGB[6][256] = {{0}};			// R V B nbR nbG nbB
	float	tabHSL[6][256] = {{0.0}};		// H S L nbH nbS nbL
	int		_tabRGB[3][256] = {{0}};		// valeurs moyennes de RGB
	float	_tabHSL[3][256] = {{0.0}};		// valeurs moyennes de HSL


	for(i=0; i<bmpTemp->width; i++)
	{
		for(j=0; j<bmpTemp->height; j++)
		{
			p = getPixel(bmpTemp, i, j);
			hsl = RGB2HSL(p);
			grey = greyIt(p, 1);

			tabRGB[0][grey] += p.Red;		tabRGB[3][grey]++;
			tabRGB[1][grey] += p.Green;		tabRGB[4][grey]++;
			tabRGB[2][grey] += p.Blue;		tabRGB[5][grey]++;

			tabHSL[0][grey] += hsl.Hue;		tabHSL[3][grey]++;
			tabHSL[1][grey] += hsl.Sat;		tabHSL[4][grey]++;
			tabHSL[2][grey] += hsl.Light;	tabHSL[5][grey]++;
		}
	}

	for(i=0; i<256; i++) // Remplissage du tableau avec la couleur "moyenne" de chaque valeur de gris de l'image en cours
	{
		(tabRGB[3][i] != 0) ? (_tabRGB[0][i] = tabRGB[0][i] / tabRGB[3][i]) : (_tabRGB[0][i] = 0);
		(tabRGB[4][i] != 0) ? (_tabRGB[1][i] = tabRGB[1][i] / tabRGB[4][i]) : (_tabRGB[1][i] = 0);
		(tabRGB[5][i] != 0) ? (_tabRGB[2][i] = tabRGB[2][i] / tabRGB[5][i]) : (_tabRGB[2][i] = 0);

		(tabHSL[3][i] != 0.0) ? (_tabHSL[0][i] = tabHSL[0][i] / tabHSL[3][i]) : (_tabHSL[0][i] = 0.0);
		(tabHSL[4][i] != 0.0) ? (_tabHSL[1][i] = tabHSL[1][i] / tabHSL[4][i]) : (_tabHSL[1][i] = 0.0);
		(tabHSL[5][i] != 0.0) ? (_tabHSL[2][i] = tabHSL[2][i] / tabHSL[5][i]) : (_tabHSL[2][i] = 0.0);

		fprintf(fp, "%d %d %d %f %f %f\n", _tabRGB[0][i], _tabRGB[1][i], _tabRGB[2][i], _tabHSL[0][i], _tabHSL[1][i], _tabHSL[2][i]);
	}
	fclose(fp);

	return 1;
}

	/******************************************************
	 MOYENNE DES COULEURS PAR NIVEAU DE GRIS SUR UNE IMAGE
	******************************************************/
/*
ESSAI DE MOYENNE PONDEREE -> Il nous fallait un tableau de 256⁴ cases, trop gros pour l'ordinateur...
int meanColorBMPeme(BMP* bmp)
{
	FILE *fp;
	BMP *bmpTemp;
	Pixel p;
	HSL hsl;
	int tab[6][256] = {{0}};		// R G B
	float tab2[6][256] = {{0}};		// H S L
	float tabPic[6][256] = {{0}};	// R G B nbR nbG nB
	float tabPic2[6][256] = {{0}};	//  H S L nbH nbS nbL
	int G[256] = {0};
	int grey, i, j, gris=0;

	fp = fopen("color.txt", "w+");
	bmpTemp = copyBMP(bmp);
	for(i=0; i<bmpTemp->width; i++)
	{
		for(j=0; j<bmpTemp->height; j++)
		{

			p = getPixel(bmpTemp, i, j);
			hsl = RGB2HSL(p);
			grey = greyIt(p, 1);
			// RGB
			tabPic[0][grey] += p.Red;	tabPic[3][grey]++;
			tabPic[1][grey] += p.Green;	tabPic[4][grey]++;
			tabPic[2][grey] += p.Blue;	tabPic[5][grey]++;

			// HSL
			tabPic2[0][grey] += hsl.Hue;	tabPic2[3][grey]++;
			tabPic2[1][grey] += hsl.Sat;	tabPic2[4][grey]++;
			tabPic2[2][grey] += hsl.Light;	tabPic2[5][grey]++;
			G[grey]++;
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

	for(i=0; i<256; i++) // Moyenne finale des couleurs par le nombre d'image totale anlyser
	{
		tab[0][i] /= (1.0*nbIm);
		tab[1][i] /= (1.0*nbIm);
		tab[2][i] /= (1.0*nbIm);

		tab2[0][i] /= (1.0*nbIm);
		tab2[1][i] /= (1.0*nbIm);
		tab2[2][i] /= (1.0*nbIm);
		fprintf(fp, "%d %d %d %f %f %f\n", tab[0][i], tab[1][i], tab[2][i], tab2[0][i], tab2[1][i], tab2[2][i]);
	}
	fclose(fp);
	return 1;
}

*/


float meanPixel(BMP* bmp)
{
	Pixel	p;
	int		i, j, n=0;
	float	tab[3] = {0.0};

	for(i=0; i<bmp->width; i++)
	{
		for(j=0; j<bmp->height; j++)
		{
			p = getPixel(bmp, i, j);
			tab[0] += p.Red;
			tab[1] += p.Green;
			tab[2] += p.Blue;
			n++;
		}
	}

	tab[0] = (tab[0] /= n);
	tab[1] = (tab[1] /= n);
	tab[3] = (tab[2] /= n);

	return (tab[0] + tab[1] + tab[3])/3.0;
}
