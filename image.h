/** @file
 * Structures de données pour représenter des images
 **/
#pragma once
#ifndef _IMAGE_H
#define _IMAGE_H

#define GRAYMAX 255
#define TESTIMAGE(img)  if (img.size()==0) { throw runtime_error("Tableau vide"); } if (img[0].size() == 0) { throw runtime_error("Tableau vide"); }

#include <vector>
#include <string>

/** Structure de donnees pour representer un pixel en couleur **/
struct Couleur {
    /** Intensite de rouge **/
    double r;
    /** Intensite de vert **/
    double g;
    /** Intensite de bleu **/
    double b;
};

inline bool operator!=(const Couleur& c1, const Couleur& c2) {
    return (c1.r != c2.r || c1.g != c2.g || c1.b != c2.b );
}

/** Structure de données pour représenter une image binaire **/
typedef std::vector<std::vector<int> > ImageNB;

/** Structure de donnees pour representer une image en teintes de gris **/
typedef std::vector<std::vector<double> > ImageGris;

/** Structure de donnees pour representer une image **/
typedef std::vector<std::vector<Couleur>> Image;

#endif
