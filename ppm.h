/** @file
 * Lecture et ecriture de fichiers au format PPM
 **/

#pragma once
#include <string>
#include "image.h"


/** Lit une image au format PPM, retourne un tableau de Couleur
 * @param source vers une image .ppm
 * @return une image
 **/
Image lirePPM(std::string source);

/**  Écrit une image dans un fichier PPM
 * @param img une image
 * @param cible le nom d'un fichier PPM
 **/
void  ecrirePPM(Image img, std::string cible);


