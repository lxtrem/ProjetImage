/** @file
 * Filtre Super Pixel
 **/

#include <cmath>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
using namespace std;

#include "image.h"
#include "seuillage.h"
#include "superpixel.h"
#include "pgm.h"
#include "ppm.h"
#include "gris-couleurs.h"
#include "image-test.h"

void distancePointsTest() {

    CHECK( distancePoints( Point( {2,0,1}), Point( {2,0,1})) == 0);
    CHECK( distancePoints( Point( {1,0,1}), Point( {1,0,0})) == 1);
    CHECK( distancePoints( Point( {2,2,2,2}), Point( {1,1,1,1})) == 2);
}

void plusProcheVoisinTest() {
    CHECK( plusProcheVoisin( Point( {1,0,1}),
                              EnsemblePoints( { Point({2,3,6}), Point({1,0,0}), Point({-1,2,0})}))
        == 1);
}

/** Test du filtre SuperPixel **/
void superPixelTest() {
    // TODO: beaucoup d'étudiants ont eu 128.91
    /*CHECK( ImageGrisEgal(CouleurAuGris(superPixel( imgTest, 2, 60, 15 )),
              ImageGris( {
                  {71.971, 71.971, 71.971, 71.971},
                  {71.971, 71.971, 71.971, 71.971},
                  {71.971, 71.971, 71.971, 71.971},
                  {71.971, 71.971, 71.971, 71.971}
                  }),
              0.001) );*/
    ecrirePPM(superPixel(lirePPM("images/Billes.256.ppm"), 1, 35, 50),  "superpixel/Billes.256.ppm");
    ecrirePPM(superPixel(lissageCouleur(lirePPM("images/Baboon.512.ppm")), 0.5, 80, 50),  "superpixel/Baboon.512.ppm");
    ecrirePPM(superPixel(lissageCouleur(lirePPM("images/Lena.512.ppm"  )), 0.4, 35, 100),  "superpixel/Lena.0,4_35.ppm");
    ecrirePPM(superPixel(lissageCouleur(lirePPM("images/Lena.512.ppm"  )), 0.05, 35, 100),  "superpixel/Lena.0,05,35.ppm");
    ecrirePPM(superPixel(lissageCouleur(lirePPM("images/Lena.512.ppm"  )), 6, 50, 100),  "superpixel/Lena.6,50.ppm");
    ecrirePPM(superPixel(lissageCouleur(lirePPM("images/Embryos.512.ppm"  )), 0.25, 40, 100),  "superpixel/Embryos.512.ppm");
    ecrirePPM(superPixel(lirePPM("images/House.256.ppm"  ),2, 50, 40),  "superpixel/House.256.ppm");
}


void KMoyenneTest(){
    EnsemblePoints P;
    for(int i=0;i<100;i++){
        double xbiais=0.;
        double ybiais=0.;
        if(i%3==1){
            xbiais = 100.;
            ybiais = 0.;
        }
        if(i%3==2){
            xbiais = 0.;
            ybiais = 100.;
        }
        Point p;
        p.push_back(xbiais + 1.*(rand()%10)-20.);
        p.push_back(ybiais + 1.*(rand()%10)-20.);
        P.push_back(p);
    }

    EnsemblePoints C = {
        {75.,50.}, {50., 75.}, {25., 25.}};

    EnsemblePoints Cout = KMoyenne(P,C,100);
    EnsemblePoints CRef = FAST_KMoyenne(P,C,100);

    for(int i=0; i<2; i++)
    for(int j=0; j<2; j++)
        CHECK( abs( Cout[i][j] - CRef[i][j] ) <= 0.5 );
}


int main(){
    cout << "Tests de la fonction distancePoints" << endl;
    distancePointsTest();
    cout << "Tests de la fonction plusProcheVoisin" << endl;
    plusProcheVoisinTest();
    cout << "Tests de la fonction KMoyenne" << endl;
    KMoyenneTest();
    cout << "Tests de la fonction superPixel" << endl;
    superPixelTest();
}

