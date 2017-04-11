#define CRED   "\x1B[31m"
#define CGRN   "\x1B[32m"
#define CYEL   "\x1B[33m"
#define CBLU   "\x1B[34m"
#define CMAG   "\x1B[35m"
#define CCYN   "\x1B[36m"
#define CWHT   "\x1B[37m"
#define CRESET "\x1B[0m"

#define TEST printf(" ! TEST !")
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
#define trunc(a) ((a) > 255 ? 255 : (a) < 0 ? 0 : (a))
#define CLEAR    system("clear")

/***************************
		 STRUCTURES
***************************/

typedef struct BMPInfoHeader	InfoHeader;
typedef struct BMPHeader		Header;
typedef struct Pixel			Pixel;
typedef struct HSL				HSL;
typedef struct BMP				BMP;

struct BMPInfoHeader
{
   int 		Size;				// Taille du header (40 octets en général)
   int 		Width;				// Largeur de l'image en pixel
   int		Height;				// Hauteur de l'image en pixel
   short	Planes;				// Nombre de plans de couleurs (toujours 1);
   short 	Bits;				// Octers par pixel (24 ici : B - R - G )
   int	 	Compression;		// Methode de compression, souvent 0
   int	 	ImageSize;			// Taille de l'image (nombre total de pixel)
   int		xResolution;		// Nombre de pixels par metre horizontallement
   int	 	yResolution;		// Nombre de pixels par metre verticalement
   int	 	Colors;				// Nombre de couleurs dans la palette (non gerer en 24bits)
   int	 	ImportantColors;	// Nombre de couleurs importante, souvent 0
};

struct BMPHeader
{
   char			Type[2];		// "Nombre magique" : 'BM' pour les images bitmap
   int			Size;			// Taille de l'image en octets
   int			Reserved;		// octet reserve a l'application qui a creer l'image, inutil
   int			Offset;			// Coordonnee du début des pixels
   InfoHeader	InfoHeader;
} ;

struct Pixel
{
   unsigned char Blue;
   unsigned char Green;
   unsigned char Red;
};

struct BMP
{
	int		width, height;		// Largeur + Hauteur de l'image
	Pixel*	data;				// Tableau contenant les pixels
};

struct HSL
{
   float Hue;
   float Sat;
   float Light;
};


/***************************
		DECLARATIONS
***************************/

// CONSATNTES
Pixel RED   =  {0,0,255};
Pixel GREEN =  {0,255,0};
Pixel BLUE  =  {255,0,0};
Pixel BLACK =  {0,0,0};
Pixel WHITE =  {255,255,255};
Pixel GREY  =  {126,126,126};

// MANIP PIXEL
void	setPixel	(BMP*, int, int, Pixel);	// Place un pixel a des coordonnées données
Pixel	getPixel	(BMP*, int, int);			// Recupere le pixel a des coordonnées données
HSL		RGB2HSL		(Pixel);					// Transforme un Pixel RGB en HSL
Pixel	HSL2RGB		(HSL);						// Transforme un Pixel HSL en RGB
float	Hue2RGB		(float, float, float);		// Transforme une teinte (Hue) en couleur RGB

// MANIP IMAGE
BMP* 	newBMP		(int, int);					// Créer un objet(struct) BMP
BMP* 	loadBMP		(const char*);				// Charge un fichier .bmp en un objet(struct) BMP
int 	saveBMP		(BMP*, const char*);		// Sauve un objet(struct) BMP en un fichier .bmp

// OPERATION SUR IMAGE
BMP*	greyScale	(BMP*);						// Passe l'image en niveau de gris
BMP*	invert		(BMP*);						// Passe l'image en negatif
BMP*	contrast	(BMP*, int);				// Change le contraste de l'image
BMP*	saturation	(BMP*, int);				// Modifie la saturation des couleurs
BMP*	sobel		(BMP*);						// Detection des contours par l'operateur de Sobel
BMP*	pewitt		(BMP*);						// Detection des contours par l'operateur de Pewitt
BMP*	colorizeRGB	(BMP*);						// Colorise une image N&B via un tableau de valeur RGB
BMP*	colorizeHSL	(BMP*);						// Colorise une image N&B via un tableau de valeur HSL
BMP*	colorizeMIX	(BMP*);						// Colorise une image N&B via un tableau de valeur RGB ET HSL
//int		meanColorBMPeme(BMP*);
int		meanColorBMProb(BMP*);					// Obtention des triplets RG et HSL moyens pour chaque valeurs de gris
float	meanPixel	(BMP*);						// Renvoi un flotant de la couleur moyenne d'une image (R+V+B)/3

// OUTILS
char*	concat(int count, ...);
void	printLogo();							// Afficher le logo de Photocheap
float	greyIt(Pixel, int);						// Retourne la valeur de gris associer a un triplet RGB
char*	toString(int);							// Transforme un entier en chaine de caractere

