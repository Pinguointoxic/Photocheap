#define TEST printf(" ! TEST ! ")
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

/***************************
		 STRUCTURES
***************************/

typedef struct BMPInfoHeader 	InfoHeader;
typedef struct BMPHeader		Header;
typedef struct Pixel			   Pixel;
typedef struct HSL            HSL;
typedef struct BMP				BMP;

struct BMPInfoHeader
{
   int 		Size;				   // Taille du header (40 octets en g�n�ral)
   int 		Width;			  	// Largeur de l'image en pixel
   int		Height;				// Hauteur de l'image en pixel
   short	   Planes;           // Nombre de plans de couleurs (toujours 1);
   short 	Bits;				   // Octers par pixel (24 ici : B - R - G )
   int	 	Compression;		// Methode de compression, souvent 0
   int	 	ImageSize;			// Taille de l'image (nombre total de pixel)
   int		xResolution;		// Nombre de pixels par metre horizontallement
   int	 	yResolution;		// Nombre de pixels par metre verticalement
   int	 	Colors;				// Nombre de couleurs dans la palette (non gerer en 24bits)
   int	 	ImportantColors;	// Nombre de couleurs importante, souvent 0
};

struct BMPHeader
{
   char 		   Type[2];       // "Nombre magique" : 'BM' pour les images bitmap
   int 			Size;          // Taille de l'image en octets
   int 			Reserved;		// octet reserve a l'application qui a creer l'image, inutil
   int 			Offset;			// Coordonnee du d�but des pixels
   InfoHeader 	InfoHeader;
} ;

struct Pixel
{
   unsigned char Blue;
   unsigned char Green;
   unsigned char Red;
};

struct BMP
{
	int width, height;			// Largeur + Hauteur de l'image
	Pixel* data;				   // Tableau contenant les pixels
};

struct HSL
{
   float HuePrct;
   float HueDeg;
   float Sat;
   float Light;
};


/***************************
		DECLARATIONS
***************************/

Pixel RED   =  {0,0,255};
Pixel GREEN =  {0,255,0};
Pixel BLUE  =  {255,0,0};
Pixel BLACK =  {0,0,0};
Pixel WHITE =  {255,255,255};
Pixel GREY  =  {126,126,126};

void 	setPixel	(BMP*, int, int, Pixel);   // Place un pixel a des coordonn�es donn�es
Pixel getPixel	(BMP*, int, int);          // Recupere le pixel a des coordonn�es donn�es

BMP* 	newBMP		(int, int);             // Cr�er un objet(struct) BMP
BMP* 	loadBMP		(const char*);          // Charge un fichier .bmp en un objet(struct) BMP
int 	saveBMP		(BMP*, const char*);    // Sauve un objet(struct) BMP en un fichier .bmp

BMP*	greyScale	(BMP*);                 // Passe l'image en niveau de gris
BMP*  invert      (BMP*);                 // Passe l'image en negatif
BMP*  sobel       (BMP*);                 // Detection des contours par l'operateur de Sobel
BMP*  pewitt      (BMP*);                 // Detection des contours par l'operateur de Pewitt
