/** @file
 * Filtres de Sobel
 **/

#pragma once
#include <cmath>
#include <vector>
#include <stdexcept>
#include "image.h"

/** filtre de Sobel horizontal
 * @param img une image en teintes de gris
 * @return une image en teintes de gris de l'intensite horizontale de img
 **/
ImageGris intensiteH(ImageGris img);

/** filtre de Sobel vertical
 * @param img une image en teintes de gris
 * @return une image en teintes de gris de l'intensite verticale de img
 **/
ImageGris intensiteV(ImageGris img);

/** filtre de Sobel
 * @param img une image en teintes de gris
 * @return une image en teintes de gris de l'intensite de img
 **/
ImageGris intensite(ImageGris img);

/** maximum d'un tableau
 * @param tableau le tableau a parcourir
 * @return la plus grande valeur trouvée
 **/
double maxVector2D(std::vector<std::vector<double>> tableau);