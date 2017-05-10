/*********************************
Robin Dell et Emeline Ehles

https://github.com/Rodd8/Photocheap
*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdarg.h>

#pragma pack(1)			// "Compresse" la structure en mémoire, utile pour avoir la VRAIE taille du "header"
#include "./src/header.h"		// Header, contient les definitions des fonctions et structure
#include "./src/tools.c"
#include "./src/pixelManip.c"
#include "./src/bmpManip.c"
#include "./src/bmpModif.c"
#include "./src/bmpStats.c"


int main()
{
	char	nameimage[200], buf[200];
	int		choice = 0, param = 0, i=1;
	BMP*	image;
	BMP*	image2;
	BMP*	image3;

	CLEAR;
	printLogo();

	while(i){

		printf("Veuillez donnez le nom de votre image:\n");
		scanf("%s",nameimage);
		image = loadBMP(concat(2, "./img/", nameimage));
		printf("\n");
		printf("Veuillez choisir vos modification :\n \n");
		printf(	"	" CBLU"1."CRESET" Niveau de gris\n"
				"	" CBLU"2."CRESET" Negatif\n"
				"	" CBLU"3."CRESET" Contraste\n"
				"	" CBLU"4."CRESET" Saturation\n"
				"	" CBLU"5."CRESET" Contours\n"
				"	" CBLU"6."CRESET" Histogramme\n"
				"	" CBLU"7."CRESET" Re-Colorisation (eme)\n"
				"	" CBLU"8."CRESET" Re-Colorisation (rob)\n");

		scanf("%d", &choice);
		switch(choice){
			// NUANCES DE GRIS
			case 1:
				image = greyScale(image);
				saveBMP(image, concat(2,"./Gallery/greyscale_", nameimage));
			break;
			// NEGATIF
			case 2:
				image = invert(image);
				saveBMP(image, concat(2, "./Gallery/invert_", nameimage));
			break;
			// CONTRASTE
			case 3:
				printf("Donnez la valeur des contraste que vous desirez: (-256 < valeur < 256)\n");
				scanf("%d", &param);
				image = contrast(image, param);
				saveBMP(image, concat(2, "./Gallery/contraste_", nameimage));
			break;
			// SATURATION
			case 4:
				printf("Donnez la valeur de saturation que vous desirez: (-100 <= valeur <= 100\n");
				scanf("%d", &param);
				image = saturation(image, param);
				saveBMP(image, concat(2, "./Gallery/saturation_", nameimage));
			break;
			// CONTOUR
			case 5:
				image = pewitt(image);
				saveBMP(image, "./Gallery/contour.bmp");
			break;
			// HISTOGRAMME
			case 6:
				histogram(image);
			break;
			// RECOLORISATION
			case 7:
				meanColorBMProb(image);		// a mettre en commentaire si on souhaite (lors d'un deuxieme passage) colorisé une image a partir des infos d'une autre
				image3 = colorizeMIX(image);
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
