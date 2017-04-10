<img src="https://raw.githubusercontent.com/Rodd8/Photocheap/master/src/logo.png" width="250">

# EOST Photocheap CC
Par __Emeline Ehles__ et __Robin Dell__.

Photocheap est une logiciel en C ayant pour but d'effectuer des traitements et des analyses sur des images BMP (24 bits uniquement)
- Negatif
- Saturation
- Nuances de gris
- Histogramme
- ...

## Utilisation

- Lancer l'executable ./pc
- Entrez le nom de l'image (si elle n'existe pas, le logiciel se ferme)
- Choisez la transformation a effectuer (chiffre de 1 a 8)
- Attendre, certaines opérations peuvent être longues
- Choisir si on veut continuer : 1 ou 0.

## Fonctions
### Manipulation
- [x] Créer une image BMP
- [x] Charger une image BMP
- [x] Sauvegarder une image BMP
- [X] Copier une image BMP

- [x] Récupérer les valeurs d'un pixel
- [x] Définir un pixel avec certaines valeurs
- [x] RGB2HSL
- [x] HSL2RGB

#### Créer une image BMP :
```C
BMP* newBMP(int largeur, int hauteur)
```
   Renvoit une structure BMP de la taille passée en paramètre.
   La structure contient une largeur (int), une hauteur (int) et un tableau de pixels (Pixel*)

#### Charger une image BMP
```C
BMP* loadBMP(const char* nomDeLimage)
```
   Charger une image bitmap de l'ordinateur dans le logiciel.
   Renvoit une structure BMP de la taille de l'image avec le tableau de pixels contenant les pixels de l'image passée en paramètre
  
#### Sauvegarder une image BMP
```C
int saveBMP(BMP* image, const char* nomDeLimage)
```
   Enregistre une structure BMP en un fichier bitmap.
   Remplit le _header_ et l'_infoheader_ avec toutes les valeurs necessaires (taille, nombre magique, palette, poids et octets ...)
   
#### Copier une image BMP
```C
BMP* copyBMP(BMP* bmp)
```
   Créer une copie de l'image passé en paramètre. Utile pour ne pas effectuer les modifications directement sur l'image d'origine.
   
#### Récupérer les valeurs d'un pixel
```C
Pixel getPixel(BMP* bmp, int i, int j)
```
   Revoit une structure Pixel contenant les valeurs RGB (3 unsigned char 0 -> 255) d'une structure BMP (_BMP* bmp_) aux coordonnées _int i_ et _int j_ passées en paramètres
   
#### Définir un pixel avec certaines valeurs
```C
void setPixel(BMP* bmp, int i, int j, Pixel p)
```
   Pose un pixel _p_ aux coordonnées _i_ et _j_ sur la structure _bmp_
   Le pixel _p_ est composé des composantes R, G et B.
   
#### RGB2HSL
```C
HSL RGB2HSL(Pixel p)
```
   Transforme les informations RGB d'un pixel en HSL (Hue, Saturation, Lightness)
   
#### HSL2RGB
```C
Pixel HSL2RGB(HSL hsl)
```
   Transforme les informations HSL d'un pixel en RGB pour les enregristrer dans l'image
   
### Modification
- [x] Nuance de gris
- [x] Negatif
- [x] Saturation
- [x] Contrastes (Sobel ou Pewitt)
- [x] Colorisé une image noir et blanc (mode RGB, mode HSL, mode Mix);

#### Nuance de gris
```C
BMP* greyScale(BMP* bmp)
```
   Transforme l'image de départ _bmp_ en son équivalent en nuance de gris suivant la formule proposée par la CIE :  
   `Gris = 0.2125 Rouge + 0.7154 Vert + 0.0721 Bleu`
   
#### Negatif
```C
BMP* invert(BMP* bmp)
```
   Transforme l'image de départ _bmp_ en son équivalent en negatif :  
   `Negatif = 255 - Couleur`
   
#### Saturation
```C
BMP* saturation(BMP* bmp, int saturation)
```
   Modifie la saturation des couleurs de l'image de -100% a +100%

#### Contrastes
```C
BMP* constrast(BMP* bmp, int contrast)
```
   Change le constraste d'une image avec des valeurs entre -255 et +255

### Colorisation
```C
BMP* colorizeRGB(BMP* bmp)
BMP* colorizeHSL(BMP* bmp)
BMP* colorizeMIX(BMP* bmp)
```
   Colorise une image en nuance de gris grace a un tableau regroupant le triplet moyen (RGB ou HSL) pour chaque valeur de gris. Le tableau est basé sur une analyse via la fonction ```C meanColorBMP()``` sur un dossier contenant plusieur image d'un même theme.

### Analyse
- [x] Contour \(Sobel + Pewitt)
- [x] Histogramme \(Rouge, Vert, Bleu, Gris)
- [x] Couleur moyenne par teinte de gris
- [x] Couleur moyenne d'une image

#### Contour (Sobel + Pewitt)
```C
BMP* sobel(BMP* bmp)
BMP* pewitt(BMP* bmp)
```
   Renvoi une image représentant les contours d'une images obtenues via les opérateurs de Sobel ou de Pewitt

#### Histogramme
```C
void histogram(BMP* bmp)
```
   Créer 4 images BMP et un fichier texte représentant les histogrammes d'une image. Un histogramme par canal de couleur (Rouge, Vert et Bleu) et un histogramme de la luminosité de l'image (Gris). Plus la densité de pixels est forte vers la valeur 255, plus l'image est sombre; réciproquement, si la densité est forte vers la valeur 0, l'image est claire.
   
#### Couleur moyenne par teinte de gris
```C
int meanColorBMP(BMP* bmp)
```
  Créer un fichier texte de 6 colones et 256 lignes.
  
| GRIS |  RED | GREEN | BLUE | HUE | SATURATION | LIGHT |
| --- | --- | --- | --- | --- | --- | --- |
| 0 | moy rouge | moy vert | moy bleu | moy teinte | moy saturation | moy luminosité |
| ... | ... | ... | ... | ... | ... | ... |
| 255 | moy rouge | moy vert | moy bleu | moy teinte | moy saturation | moy luminosité |

#### Couleur moyenne d'une image
```C
float meanPixel(BMP* bmp)
```
   Renvoi la couleur moyenne d'une image, la moyenne des valeurs de tous les pixels.

### Outils
#### Concatenation de chaine
```C
char* concat(int count, ...)
```
   Fusionne 'count' chaine de caractères.
exemple : concat(2, "Bonjour ", "toto")
          concat(3, mot1, "_", mot2)
          ...
			
#### Nuance de gris
```C
float greyIt(Pixel p, int mode)
```
   Renvoi la nuance de gris associé a un triplet RGB.
mode = 0 -> formule du C.I.E
mode = 1 -> moyenne du triplet RGB

#### Entier vers Chaine
```C
char* toString(int a)
```
   Transforme un entier en chaine de caractères.
   
## License

MIT License.
