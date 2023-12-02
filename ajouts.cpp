#include "ajouts.h"
#include <stdexcept>
using namespace std;

Image fusionCouleurs(std::vector<ImageGris> images)
{
    if (images.size() != 3) { throw runtime_error("Mauvais nombre d'images passées");}
    TESTIMAGE(images[0]); 
    TESTIMAGE(images[1]); 
    TESTIMAGE(images[2]);
    if (images[0].size() != images[1].size() ||
        images[0].size() != images[2].size() ||
        images[0][0].size() != images[1][0].size() ||
        images[0][0].size() != images[2][0].size()
        ) {
        throw runtime_error("Mauvaises tailles");
    }
    Image retour(images[0].size());
    for (int i{0}; i<images[0].size(); i++) {
        for (int j{0}; j<images[0][0].size(); j++) {
            retour[i].push_back(Couleur{images[0][i][j], images[1][i][j], images[2][i][j]});
        }
    }
    return retour;
}

std::vector<ImageGris> splitCouleurs(Image img) {
    TESTIMAGE(img);
    ImageGris r(img.size());
    ImageGris g(img.size());
    ImageGris b(img.size());
    for (int i{0}; i<img.size(); i++) {
        for (int j{0}; j<img[0].size(); j++) {
            r[i].push_back(img[i][j].r);
            g[i].push_back(img[i][j].g);
            b[i].push_back(img[i][j].b);
        }
    }
    return vector<ImageGris>{r,g,b};
}

inline double convolutionPoint(int k, int l, const ImageGris *img, vector<vector<int>> noyau ) {
    double retour{0};
    int m,n;
    for (int i{0}; i< noyau.size(); i++) {
        for (int j{0}; j< noyau.size(); j++) {
            m=k+i-noyau.size()/2;
            n=l+j-noyau.size()/2;
            retour += noyau[i][j]*img->at(m)[n];
        }
    }
    return retour;
}

ImageGris convolution(ImageGris img, vector<vector<int>> noyau) {
    TESTIMAGE(img);
    TESTIMAGE(noyau);
    ImageGris imgConvol{img};
    for (int i{0}; i<img.size(); i++) { // Bords droit et gauche
         imgConvol[i][0] = 0;
         imgConvol[i][img.size()-1] = 0;
    }
    for (int j{0}; j<img[0].size(); j++) { // Bords superieurs et inferieurs
         imgConvol[0][j] = 0;
         imgConvol[img[0].size()-1][j] = 0;

    }
    if (noyau.size() != noyau[0].size() || noyau.size()%2!=1 || noyau.size() >img.size() ) { throw runtime_error("Le noyau ne peut etre appliqué");}
    for (int i{0+noyau.size()/2}; i<img.size()-noyau.size()/2; i++)
    {
        for (int j{0+noyau.size()/2}; j<img[0].size()-noyau.size()/2;j++)
        {
            imgConvol[i][j] = convolutionPoint(i,j,&img,noyau);
        }
    }
    return imgConvol;
}

Image convolution(Image img, vector<vector<int>> noyau) {
    TESTIMAGE(img);
    vector<ImageGris> imgSplit = splitCouleurs(img);
    for (int i{0}; i<3; i++) {
        imgSplit[i]=convolution(imgSplit[i], noyau);
    }
    return fusionCouleurs(imgSplit);
}