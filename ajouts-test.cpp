#include <iostream>
#include <assert.h>
#include "ajouts.h"
using namespace std;
#include "pgm.h"
#include "ppm.h"
#include "sobel.h"
#include "seuillage.h"
#include "image-test.h"

int main() {
    ImageGris r,g,b;
    r =  {{255, 0, 0}, {0,255, 0}, {0,0,255}};
    b =  {{0,255, 0}, {0,0,255}, {255, 0, 0}};
    g =  {{0,0,255}, {255, 0, 0}, {0,255, 0}};
    Image fusion{fusionCouleurs(std::vector<ImageGris>{r,g,b})};
    vector<ImageGris> split{splitCouleurs(fusion)};
    assert(split[0]==r);
    assert(split[1]==g);
    assert(split[2]==b);
    vector<vector<int>> conv = {{1,0,-1},{2,0,-2}, {1,0,-1}};
    CHECK(ImageGrisEgal(convolution(imgGrisTest, conv),
              ImageGris( {
                  {0, 0, 0, 0},
                  {0, -373.47, 227.507, 0},
                  {0, -22.1312, 323.866, 0},
                  {0, 0, 0, 0}
              }),
              0.001));
    vector<vector<int>> flou = {{1,2,1},{2,4,2},{1,2,1}};
    split = splitCouleurs(convolution(lirePPM("images/Lena.512.ppm"), flou));
    for (int i{0}; i<3; i++)
        split[i]=renormalise(split[i]);
    ecrirePPM(fusionCouleurs(split), "ppm/LenaFlou.ppm");
    return 0;
}