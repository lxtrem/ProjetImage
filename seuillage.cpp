#include <stdexcept>
/** @file
 * Filtres de seuillage
 **/

#include "seuillage.h"
#include <cmath>
#include <iostream>
#include "sobel.h"
using namespace std;


ImageGris lissage(ImageGris img) {
    TESTIMAGE(img);
    ImageGris imgLissee{img};
    for(int i{1}; i <img.size()-1; i++) { // Ici on traite l'image sans les bords
        for(int j{1}; j < img[0].size()-1; j++) {
            imgLissee[i][j] = (1./16.)*(img[i-1][j-1]+2*img[i-1][j]+img[i-1][j+1]+2*img[i][j-1]+4*img[i][j]+2*img[i][j+1]+img[i+1][j-1]+2*img[i+1][j]+img[i+1][j+1]);
        }
    }
    return img;
}

Image lissageCouleur(Image img) {
    TESTIMAGE(img);
    Image imgLissee{img};
    for(int i{1}; i <img.size()-1; i++) { // Ici on traite l'image sans les bords
        for(int j{1}; j < img[0].size()-1; j++) {
            imgLissee[i][j].r = (1./16.)*(img[i-1][j-1].r+2*img[i-1][j].r+img[i-1][j+1].r+2*img[i][j-1].r+4*img[i][j].r+2*img[i][j+1].r+img[i+1][j-1].r+2*img[i+1][j].r+img[i+1][j+1].r);
            imgLissee[i][j].b = (1./16.)*(img[i-1][j-1].b+2*img[i-1][j].b+img[i-1][j+1].b+2*img[i][j-1].b+4*img[i][j].b+2*img[i][j+1].b+img[i+1][j-1].b+2*img[i+1][j].b+img[i+1][j+1].b);
            imgLissee[i][j].g = (1./16.)*(img[i-1][j-1].g+2*img[i-1][j].g+img[i-1][j+1].g+2*img[i][j-1].g+4*img[i][j].g+2*img[i][j+1].g+img[i+1][j-1].g+2*img[i+1][j].g+img[i+1][j+1].g);
        }
    }
    return imgLissee;
}

ImageGris renormalise(ImageGris img)
{
    TESTIMAGE(img);
    double maximum{maxVector2D(img)};
    for(int i{0}; i <img.size(); i++) {
        for(int j{0}; j < img[0].size(); j++) {
            img[i][j]=abs(img[i][j])*(GRAYMAX/maximum);
        }
    }
    return img;
}

ImageGris seuillage(ImageGris img, int seuil) {
    TESTIMAGE(img);
    if (seuil < 0) { throw runtime_error("Mauvaise valeur de seuil : " + seuil); }
    for (int i{0}; i<img.size(); i++){
        for (int j{0}; j<img[0].size(); j++){
            img[i][j] = img[i][j] >= seuil ? 0. : 255.;
        }
    }
    return img;
}

ImageGris doubleSeuillage(ImageGris imgIntensite, ImageGris imgContour, int seuil) {
    TESTIMAGE(imgIntensite);
    TESTIMAGE(imgContour);
    for(int i=0; i<imgIntensite.size();i++) { // Traitement des bords
        imgIntensite[i][0] = 255;
        imgIntensite[i][imgIntensite[i].size()-1] = 255;
    }
    for(int j=0; j<imgIntensite.size();j++) { // Traitement des bords
        imgIntensite[0][j] = 255;
        imgIntensite[imgIntensite.size()-1][j] = 255;
    }
    ImageGris img2{imgContour}; // On sauvegarde les contours d'origine
    for (int i{1}; i<imgIntensite.size()-1; i++) {
        for (int j{1}; j<imgIntensite[0].size()-1; j++) {
            if(imgIntensite[i][j]>=seuil) { // Si l'intensité est suffisante, on va regarder si elle est connectée a un pixel noir
                imgContour[i][j] = img2[i][j] == 0 || img2[i-1][j] == 0 || img2[i+1][j] == 0 || img2[i][j+1] == 0 || img2[i][j-1] == 0 ? 0 : 255;
            }
            else {
                imgContour[i][j] = 255;
            }
        }
    }
    return imgContour;
}

ImageGris doubleSeuillage(ImageGris imgIntensite, int seuilFort, int seuilFaible) {
    TESTIMAGE(imgIntensite);
    if (seuilFort < 0) { throw runtime_error("Mauvaise valeur de seuilFort : " + seuilFort); }
    if (seuilFaible < 0) { throw runtime_error("Mauvaise valeur de seuilFaible : " + seuilFaible); }
    ImageGris imgDouble{seuillage(imgIntensite, seuilFort)};
    ImageGris imgCompare;
    do {
        imgCompare = imgDouble;
        imgDouble = doubleSeuillage(imgIntensite, imgDouble, seuilFaible); 
    } while (imgDouble != imgCompare); // On continue uniquement si des modifications sont apportées
    return imgDouble;
}