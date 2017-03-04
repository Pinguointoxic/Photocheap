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
   Revoit une structure Pixel contenant les valeurs RGB (3 unsigned char -> 255) d'une structure BMP (_BMP* bmp_) aux coordonnées _int i_ et _int j_ passées en paramètres
   
#### Définir un pixel avec certaines valeurs
```C
void setPixel(BMP* bmp, int i, int j, Pixel p)
```
   Pose un pixel _p_ aux coordonnées _i_ et _j_ sur la structure _bmp_
   Le pixel _p_ est composé des composantes R, G et B.
   
### Modification
- [x] Nuance de gris
- [x] Negatif
- [ ] Saturation
- [ ] Contrastes
- [ ] Echanger les cannaux de couleur 2 a 2
- [ ] Echanger les 3 cannaux de couleur cycliquement
- [ ] \( optionnel ) Redimenssionnement proche voisin

### Analyse
- [x] Contour \(Sobel)
- [x] Contour \(Pewitt)
- [x] Histogramme \(Rouge, Vert, Bleu, Gris)
- [ ] Statistiques \(Portait ? Paysage ? ...)

## License

MIT License.
