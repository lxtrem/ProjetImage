/** @file
 * Filtres de conversion couleur <-> teintes de gris
 **/

#include <iostream>
#include <stdexcept>
using namespace std;
#include "pgm.h"
#include "sobel.h"
#include "seuillage.h"
#include "image-test.h"

void seuillageTest() {
    CHECK( ImageGrisEgal(seuillage( imgGrisTest, 80),
              ImageGris( {
                  {255, 0, 255, 0},
                  {255, 0, 0, 0},
                  {0, 0, 0, 0},
                  {0, 0, 0, 0}
              }),
              0.001) );
    CHECK( ImageGrisEgal(seuillage( intensite(imgGrisTest), 80),
              ImageGris( {
                  {255, 255, 255, 255},
                  {255, 0, 0, 255},
                  {255, 255, 0, 255},
                  {255, 255, 255, 255}
              }),
              0.001) );
    CHECK( ImageGrisEgal(doubleSeuillage(intensite(imgGrisTest), 400, 80),
                          ImageGris( {
                              {255, 255, 255, 255},
                              {255, 0, 0, 255},
                              {255, 255, 0, 255},
                              {255, 255, 255, 255}
                          }),
                          0.001) );


    // cout << "Vérifier que les images obtenues dans 'seuillage/' sont semblables à celles fournies dans 'seuillage/correction/'" << endl;
    ecrirePGM(seuillage(intensite(lirePGM("images/Billes.256.pgm")), 80), "seuillage/Billes.256.pgm");
    ecrirePGM(seuillage(intensite(lirePGM("images/Lena.512.pgm"  )), 80), "seuillage/Lena.512.pgm");
    // cout << "\tProposer des seuils pour Embryos.512.ppm et House.256.ppm" << endl;
}

void doubleSeuillageTest(){
    // cout << "Vérifier que les images obtenues dans 'seuillage_double/' sont semblables à celles fournies dans 'seuillage_double/correction/'" << endl;

    ecrirePGM(doubleSeuillage(lirePGM("images/Lena.512.pgm"),
                              seuillage(intensite(lirePGM("images/Lena.512.pgm" )),80),70),
              "seuillage_double/Lena-unpas.512.pgm");
    ecrirePGM(doubleSeuillage(intensite(lirePGM("images/Billes.256.pgm")), 500, 80), "seuillage_double/Billes.256.pgm");
    ecrirePGM(doubleSeuillage(intensite(lirePGM("images/Lena.512.pgm"  )), 500, 80), "seuillage_double/Lena.500_80.pgm");
    ecrirePGM(doubleSeuillage(intensite(lirePGM("images/Lena.512.pgm"  )), 500, 130), "seuillage_double/Lena.500_130.pgm");
    ecrirePGM(doubleSeuillage(intensite(lirePGM("images/Lena.512.pgm"  )), 500, 50), "seuillage_double/Lena.500_50.pgm");
    ecrirePGM(doubleSeuillage(intensite(lirePGM("images/Lena.512.pgm"  )), 400, 80), "seuillage_double/Lena.400_80.pgm");
    ecrirePGM(doubleSeuillage(intensite(lirePGM("images/Embryos.512.pgm"  )), 500, 170), "seuillage_double/Embryos.512.pgm");
    ecrirePGM(doubleSeuillage(intensite(lirePGM("images/House.256.pgm"  )), 500, 80), "seuillage_double/House.256.pgm");
}

int main(){

    seuillageTest();
    doubleSeuillageTest();

    return 0;
}
