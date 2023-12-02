#include <stdexcept>
/** @file
 * Filtre Super Pixel
 **/
#include <cmath>
#include <iostream>
#include "superpixel.h"

using namespace std;
double distancePoints(Point p, Point c) {
    double retour=0;
    for(int i=0; i<p.size(); i++)
        retour+=(p[i]-c[i])*(p[i]-c[i]);
    return sqrt(retour);
}

double distanceAEnsemble(Point p, EnsemblePoints C) {
    double distanceMin{0};
    for (Point i: C) {
        distanceMin = distancePoints(i, p) > distanceMin ? distanceMin : distancePoints(i,p);
    }
    return distanceMin;
}

int plusProcheVoisin(Point p, EnsemblePoints C) {
    int indice{0};
    for (int i{0}; i< C.size(); i++) {
        indice = distancePoints(C[i], p) < distancePoints(C[indice], p) ? i : indice;
    }
    return indice;
}

EnsemblePoints sousEnsemble(EnsemblePoints P,EnsemblePoints C,int k) {
    EnsemblePoints retour;
    for(Point p: P)
        if(plusProcheVoisin(p,C)==k) // Si C[k] est le point le plus proche de p
            retour.push_back(p); // On ajoute p a la liste.
    return retour;
}

Point barycentre(EnsemblePoints Q) {
    Point bary(Q[0].size());
    for(int i=0; i<Q.size(); i++) { // Pour tous les points
        for(int j=0; j<Q[0].size(); j++) {// Pour chacune de leur coordonnées
            bary[j]+=Q[i][j]; // On somme les coordonnées des points 
        }
    }
    for(int i=0; i<bary.size(); i++) { // Pour chaque coordonnée
        bary[i] = bary[i]/Q.size(); // On divise chaque coordonnée par le nombre de point a l'origine
    }
    return bary;
}

EnsemblePoints KMoyenne(EnsemblePoints P,EnsemblePoints C, int nbAmeliorations) {
    EnsemblePoints CTemp{C};
    for(int n=0; n<nbAmeliorations; n++) {
        for(int k=0; k<C.size(); k++) {
            CTemp[k] = barycentre(sousEnsemble(P,C,k));
        }
        C = CTemp;
    }
    return C;
}

EnsemblePoints FAST_KMoyenne(EnsemblePoints P,EnsemblePoints C, int nbAmeliorations) {
    vector<int> label;
    label.resize(P.size());
    for(int n=0; n<nbAmeliorations; n++) {
        vector<int> clusterSize;
        clusterSize.resize(C.size(),0);
        for (int p=((int)P.size())-1; p>=0; p--) {
            double di = 0;
            int nn=0;
            for(int d=((int)P[0].size())-1; d>=0; d--)
                di+=(P[p][d]-C[0][d])*(P[p][d]-C[0][d]);
            for(int c=((int)C.size())-1; c>=1; c--) {
                double dt=0;
                for(int d=((int)P[0].size())-1; d>=0; d--)
                    dt+=(P[p][d]-C[c][d])*(P[p][d]-C[c][d]);
                if(dt<di) {
                    di=dt;
                    nn=c;
                }
            }
            label[p]=nn;
            clusterSize[nn]++;
        }
        for (int p=((int)P.size())-1; p>=0; p--)
            for(int d=((int)P[0].size())-1; d>=0; d--)
                C[label[p]][d]+=P[p][d];
        for(int c=((int)C.size())-1; c>=0; c--)
            if(clusterSize[c]!=0)
                for(int d=((int)P[0].size())-1; d>=0; d--)
                    C[c][d] = C[c][d]/(clusterSize[c]+1);
    }
    return C;
}

EnsemblePoints pivotSuperPixel(Image img, double lambda, int mu) {
    TESTIMAGE(img);
    EnsemblePoints retour;
    for(int i=0; i<img.size(); i+=mu) {
        for(int j=0; j<img[0].size(); j+=mu) {
            retour.push_back(Point{ (double)i, (double)j, lambda*img[i][j].r, lambda*img[i][j].g, lambda*img[i][j].b }); // On ajoute le pivot avec le coefficient associé
        }
    }
    return retour;
}

EnsemblePoints superPixels(Image img, double lambda, int mu, int nbAmeliorations) {
    TESTIMAGE(img);
    EnsemblePoints points;
    for(int i=0; i<img.size(); i++) {
        for(int j=0; j<img[0].size(); j++) {
            points.push_back({ (double)i, (double)j, lambda*img[i][j].r, lambda*img[i][j].g, lambda*img[i][j].b}); // On transforme l'image en une liste de points dans R^5 (en prenant en compte les coefficients)
        }
    }
    return FAST_KMoyenne(points, pivotSuperPixel(img,lambda,mu), nbAmeliorations); // On renvoie la répartition des points dans les différents super pixels
}

Image superPixel(Image img, double lambda, int mu, int nbAmeliorations) {
    TESTIMAGE(img);
    EnsemblePoints pointsSuperPixels = superPixels(img,lambda,mu,nbAmeliorations);
    Point point;
    int indice;
    Image imgRemplacee{img};
    for(int i=0; i<img.size(); i++) {
        for(int j=0; j<img[0].size(); j++) {
            indice = plusProcheVoisin({ (double)i, (double)j, lambda*img[i][j].r, lambda*img[i][j].g, lambda*img[i][j].b }, pointsSuperPixels);
	        point = pointsSuperPixels[indice]; // On récupère le superpixel auquel appartient le point
            imgRemplacee[i][j].r = point[2]/lambda; // on remplace les couleurs
            imgRemplacee[i][j].g = point[3]/lambda; // on remplace les couleurs
            imgRemplacee[i][j].b = point[4]/lambda; // on remplace les couleurs
        }
    }
    Image imgBords{img};
    for(int i=1; i<img.size()-1; i++) {
        for(int j=1; j<img[0].size()-1; j++) {
            if ( 
                imgRemplacee[i][j] != imgRemplacee[i+1][j] ||
                imgRemplacee[i][j] != imgRemplacee[i][j+1] ||
                imgRemplacee[i][j] != imgRemplacee[i-1][j] ||
                imgRemplacee[i][j] != imgRemplacee[i][j-1]
            )
            {
                imgBords[i][j].r = 0; // On met le bord en bleu
                imgBords[i][j].g = 0;
                imgBords[i][j].b = 255;
            }
        }
    }
    return imgBords;
}
