/** @file
 * Filtres de seuillage
 **/

#pragma once

#include "image.h"
/** Applique une approximation du flou gaussien sur l'image pour réduire le bruit. On utilisera la matrice de convolution (le noyau) suivante:
 * 1 2 1
 * 2 4 2
 * 1 2 1
 * Le tout*1/16 afin d'avoir des valeurs normalisées.
 * @param img l'image à traiter
 * @return l'image traitée
 **/
ImageGris lissage(ImageGris img);

/** Applique une approximation du flou gaussien sur l'image en couleurs pour réduire le bruit. On utilisera la matrice de convolution (le noyau) suivante:
 * 1 2 1
 * 2 4 2
 * 1 2 1
 * Le tout*1/16 afin d'avoir des valeurs normalisées.
 * @param img l'image à traiter
 * @return l'image traitée
 **/
 Image lissageCouleur(Image img);

/** Renormalize une image en teinte de gris, les ramenant dans l'intervalle [0,255]
 * @param img un image en teintes de gris
 * @return une image en teintes de gris
 **/
ImageGris renormalise(ImageGris img);

/** Filtre de seuillage
 * @param img
 * @param seuil un entier dans l'intervalle [0,255]
 * @return image en noir et blanc obtenue en remplacant la teinte de 
 *  chaque pixel par
 *  - du blanc si teinte < seuil
 *  - du noir  si teinte > seuil
 **/
ImageGris seuillage(ImageGris img, int seuil);

/** Filtre de double seuillage
 * @param imgIntensite image d'intensite
 * @param imgContour image codant un ensemble de pixels selectionnes
 * @param seuil un entier de l'intervalle [0,255]
 * @return une copie d'imgIntensite modifiee de sorte que:
 *  -si teinte > seuil et voisin d'un pixel de imgContour, alors pixel noir
 *  -sinon pixel blanc
 **/
ImageGris doubleSeuillage(ImageGris imgIntensite, ImageGris imgContour, int seuil);

/** Filtre de double seuillage iteratif
 * @param imgIntensite image d'intensité
 * @param seuilFort un entier de l'intervalle [0,255]
 * @param seuilFaible un entier de l'intervalle [0,255]
 * @return le double seuillage de img
 **/
ImageGris doubleSeuillage(ImageGris imgIntensite, int seuilFort, 
				int seuilFaible);

