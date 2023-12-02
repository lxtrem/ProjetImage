/** @file
 * Filtres de Sobel
 **/
#include "sobel.h"
using namespace std;
typedef vector<vector<double> > ImageGris;

ImageGris intensiteH(ImageGris img) {
    TESTIMAGE(img);
    ImageGris imgIntensiteH{img};
    for(int i{1}; i <img.size()-1; i++) { // Ici on traite l'image sans les bords
        for(int j{1}; j < img[0].size()-1; j++) {
            imgIntensiteH[i][j] = img[i-1][j-1] + 2*img[i][j-1] + img[i+1][j-1]- img[i-1][j+1] - 2*img[i][j+1] - img[i+1][j+1];
        }
    }

    for (int i{0}; i<img.size(); i++) { // Bords droit et gauche
         imgIntensiteH[i][0] = 0;
         imgIntensiteH[i][img.size()-1] = 0;
    }
    for (int j{0}; j<img[0].size(); j++) { // Bords superieurs et inferieurs
         imgIntensiteH[0][j] = 0;
         imgIntensiteH[img[0].size()-1][j] = 0;

    }
    return imgIntensiteH;
}

ImageGris intensiteV(ImageGris img) {
    TESTIMAGE(img);
    ImageGris imgIntensiteV{img};
    for(int i{1}; i <img.size()-1; i++) { // Ici on traite l'image sans les bords
        for(int j{1}; j < img[0].size()-1; j++) {
            imgIntensiteV[i][j] = img[i-1][j-1] + 2*img[i-1][j] + img[i-1][j+1]- img[i+1][j-1] - 2*img[i+1][j] - img[i+1][j+1];
        }
    }
    for (int i{0}; i<img.size(); i++) { // Bords droit et gauche
         imgIntensiteV[i][0] = 0;
         imgIntensiteV[i][img.size()-1] = 0;
    }
    for (int j{0}; j<img[0].size(); j++) { // Bords superieurs et inferieurs
         imgIntensiteV[0][j] = 0;
         imgIntensiteV[img[0].size()-1][j] = 0;

    }
    return imgIntensiteV;
}

ImageGris intensite(ImageGris img) {
    TESTIMAGE(img);
    ImageGris imgIntensiteV, imgIntensiteH, imgIntensite;
    imgIntensiteH = intensiteH(img);
    imgIntensiteV = intensiteV(img);
    imgIntensite = img;
    for (int i{0}; i<img.size(); i++){
        for (int j{0}; j<img[0].size(); j++){
            imgIntensite[i][j] = sqrt((imgIntensiteH[i][j]*imgIntensiteH[i][j])+(imgIntensiteV[i][j]*imgIntensiteV[i][j]));
        }
    }
    return imgIntensite;
}

