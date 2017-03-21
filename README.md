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

A FAIRE

## Fonctions
### Manipulation
- [x] Créer une image BMP
- [x] Charger une image BMP
- [x] Sauvegarder une image BMP
- [x] Récupérer les valeurs d'un pixel
- [x] Définir un pixel avec certaines valeurs
- [ ] RGB2HSL
- [ ] HSL2RGB

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

   Transforme les informations RGB d'un pixel en HSL (Hue, Saturation, Lightness)
   
#### HSL2RGB

   Transforme les informations HSL d'un pixel en RGB pour les enregristrer dans l'image
   
### Modification
- [x] Nuance de gris
- [x] Negatif
- [ ] Saturation
- [x] Contrastes
- [ ] Colorisé une image noir et blanc

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
Il va etre nécessaire de transformer les valeur RGB en HLS (Hue, Lightness, Saturation) pour effectuer les modifications de saturation sur l'image.

#### Contrastes
```C
BMP* constrast(BMP* bmp, int contrast)
```
   Change le constraste d'une image avec des valeurs entre -255 et +255



### Analyse
- [x] Contour \(Sobel + Pewitt)
- [x] Histogramme \(Rouge, Vert, Bleu, Gris)
- [ ] Statistiques \(Portait ? Paysage ? ...)

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
   
#### Statistiques

## License

MIT License.
