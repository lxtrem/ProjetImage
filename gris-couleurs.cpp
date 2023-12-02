#include <stdexcept>
/** @file
 * Filtres de conversion couleur <-> teintes de gris
 **/
#include "gris-couleurs.h"
using namespace std;

ImageGris CouleurAuGris( Image img ) {
    if (img.size()==0) { throw runtime_error("Tableau vide"); }
    if (img[0].size() == 0) { throw runtime_error("Tableau vide"); }
    ImageGris imageGrise(img.size());
    for (int i{0}; i<img.size(); i++ ) {
        for (int j{0}; j<img[0].size(); j++) {
            imageGrise[i].push_back((0.2126*img[i][j].r)+(0.7152*img[i][j].g)+(0.0722*img[i][j].b));
        }
    }
    return imageGrise;
}

Image GrisACouleur( ImageGris img ) {
    if (img.size()==0) { throw runtime_error("Tableau vide"); }
    if (img[0].size() == 0) { throw runtime_error("Tableau vide"); }
    Image imageCouleur(img.size());
    Couleur couleur{0,0,0};
    for (int i{0}; i<img.size(); i++ ) {
        for (int j{0}; j<img[0].size(); j++) {
            couleur = {img[i][j],img[i][j],img[i][j]};
            imageCouleur[i].push_back(couleur);
        }
    }
    return imageCouleur;
}

