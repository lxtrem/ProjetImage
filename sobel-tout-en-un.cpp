#include <stdexcept>
/** @file
 * Filtres de Sobel
 **/
#include <cmath>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <fstream>
using namespace std;
#define GRAYMAX 255

/** Structure de donnee pour representer une image en teintes de gris **/
typedef vector<vector<double> > ImageGris;

/** Infrastructure minimale de test **/
#define CHECK(test) if (!(test)) cout << "Test failed in file " << __FILE__ << " line " << __LINE__ << ": " #test << endl

/** Une image 4x4 en teintes de gris pour faire des tests **/
ImageGris imgGrisTest = {
    {0, 255, 54.213, 236.589},
    {18.411, 182.376, 200.787, 120},
    {139.583, 172.841, 94.0878, 88.4974},
    {158.278, 172.841, 89.0236, 80.0384}
};

/** maximum d'un tableau
 * @param tableau le tableau a parcourir
 * @return la plus grande valeur trouvée
 **/
double maxVector2D(vector<vector<double>> tableau) {
    if (tableau.size() == 0) {
        throw runtime_error("Tableau vide");
    }
    if (tableau[0].size() == 0) {
        throw runtime_error("Tableau vide");
    }
    double maximum{tableau[0][0]};
    for (vector<double> i: tableau) {
        for (double j: i) {
            maximum = max(j, maximum);
        }
    }
    return maximum;
}

/** Construire une image en teintes de gris depuis un fichier PGM
 * @param source le nom d'un fichier PGM
 * @return une image en teintes de gris
 **/
ImageGris lirePGM(string source) {
    ifstream image;
    image.open(source);
    if (!image) {throw runtime_error("Fichier non trouve: " + source);}
    string magicNumber,l,L,gray;
    int i=0;
    while (i<4) { // 4 informations sont ici necessaires
        switch (i){
            case 0: // nombre magique
                image >> magicNumber;
                if (magicNumber[0] == 'P' and magicNumber[1] == '2' and magicNumber.size() == 2) {i=1;}
                else if (magicNumber[0] == '#') {
                    getline(image, magicNumber); // On passe la ligne, le retour n'importe pas
                }
                else {throw runtime_error("Le nombre magique n'a pas ou mal été renseigné."); }
                break;
            case 1: // largeur
                image >> l;
                if ((char)l[0] != (char)'#') { i=2;}
                else {getline(image, l);}
                break;
            case 2: // hauteur
                image >> L;
                if ((char)L[0] != (char)'#') {i=3;}
                else {getline(image, L);}
                break;
            case 3: // Valeur du pixel blanc
                image >> gray;
                if ((char)L[0] != (char)'#') {i=4;}
                else {getline(image, L);}
                break;
        }
    }
    int largeur, hauteur, maxGris;
    try {
        largeur = stoi(l);
        hauteur = stoi(L); // conversions str->int
        maxGris = stoi(gray);
    }
    catch (const invalid_argument& e) {
        throw runtime_error("La largeur, le niveau de gris ou la longueur n'est pas un entier comme attendu.");
    }
    if (maxGris > 65536 or maxGris == 0 or largeur == 0 or hauteur == 0 ) { // Les entiers seront toujours positifs, sinon ils n'auraient pas été reconnus comme des nombres plus haut.
        throw runtime_error("La largeur, le niveau de gris ou la longueur n'ont pas une valeur gerable. (dimensions nulles ? niveau de gris nul ?)");
    }
    ImageGris img = ImageGris(hauteur);
    string s;
    bool err{false}; // Permet de verifier si on a déja affiché une erreur
    for (int i{0}; i < hauteur; i++ ){
        img[i] = vector<double>(largeur);
        for (int j{0}; j< largeur; j++) {
            if (image >> s) {
                if (s[0] == '#') { // Gestion des commentaires dans le fichier
                    getline (image, s); // On se debarasse de la ligne
                    j--; // On décrémente pour ne pas laisser cette case vide
                    continue;
                }
                int nb;
                try {
                    nb=stoi(s);
                }
                catch (const invalid_argument& e) {
                    throw runtime_error("La largeur, le niveau de gris ou la longueur n'est pas un entier comme attendu.");
                }
                if ( err == false && not(nb <= maxGris && nb >= 0 )) {
                    cerr << "Entier innatendu dans la lecture du fichier (valeur superieure au max indiqué ?)." << endl;
                    err = true;
                }
                img[i][j] = nb;
            }
            else { throw runtime_error("Caractere innatendu dans la lecture du fichier (Mauvaise longeur ? Valeur non entiere ?).");}
        }
    }
    while (image>>s) { //On verifie ce qui n'a pas été lu
        if ( s[0] == '#' ) { // Si ce sont des commentaires, pas de problème
            getline (image, s);
        }
        else {
            cerr << "Taille de l'image superieure à celle indiquée." << endl;
        }
    }
    image.close();
    return img;
}



/** Ecrit une image en teintes de gris dans un fichier PGM
 * @param img une image en teintes de gris
 * @param cible le nom d'un fichier PGM
 **/
void ecrirePGM(ImageGris img, string cible) {
    ofstream output;
    output.open(cible);
    if (!output) {throw runtime_error("Fichier non trouve: " + cible);}
    if (img.size() == 0) {throw runtime_error("Erreur: la hauteur de l'image fournie est nulle."); } //verification de la taille
    else { 
        if (img[0].size() == 0) {throw runtime_error("Erreur: la largeur de l'image fournie est nulle."); } //verification de la taille
    }
    bool err{false}; // Permet de verifier si on a déja affiché une erreur
    output << "P2\n" << img[0].size() << " " << img.size() << "\n" << GRAYMAX << "\n"; // Header
    for (int i{0}; i < img.size(); i++ ){
        for (int j{0}; j< img[0].size(); j++) {
            if ( (img[i][j] > GRAYMAX || img[i][j] < 0) && err == false ) {
                cerr << "Entier innatendu dans la lecture de l'image (valeur superieure à 255 ?)." << endl;
                err = true;
            }
            output << (int)img[i][j] << " ";
        }
        output << "\n"; // Pas necessaire, plus pratique à la lecture du fichier
    }
    output.close();
}



/** Teste si deux images en teintes de gris sont égales modulo imprécision numérique
 * En cas de différence un message est affiché
 * @param a une image en teintes de gris
 * @param b une image en teintes de gris
 * @param precision un flottant positif: la précision souhaitée; typiquement 0.001
 * @return vrai si les images sont égales et faux sinon
 **/
bool ImageGrisEgal(ImageGris a, ImageGris b, float precision) {
    if (a.size() != b.size())  {
        cout << "Nombre de lignes différent" << endl;
        return false;
    }
    for (int i=0; i<a[0].size(); i++)
        if (a[0].size() != b[i].size()) {
            cout << "Nombre de colonnes différent" << endl;
            return false;
        }
    for (int i=0; i<a.size(); i++)
        for (int j=0; j<a[0].size(); j++)
            if (abs(a[i][j] - b[i][j]) > precision) {
                cout << "Valeur differentes en position " << i << "," << j
             << ": " << a[i][j] << " au lieu de " << b[i][j] << endl;
                return false;
            }
    return true;
}

/** filtre de Sobel horizontal
 * @param img une image en teintes de gris
 * @return une image en teintes de gris de l'intensite horizontale de img
 **/
ImageGris intensiteH(ImageGris img) {
    if (img.size()==0) { throw runtime_error("Tableau vide"); } 
    if (img[0].size() == 0) { throw runtime_error("Tableau vide"); }
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
    if (img.size()==0) { throw runtime_error("Tableau vide"); } 
    if (img[0].size() == 0) { throw runtime_error("Tableau vide"); }
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

/** Renormalise une image en teinte de gris, les ramenant dans l’intervalle [0,255]
 * * @param img un image en teintes de gris
 * * @return une image en teintes de gris**/
ImageGris renormalise(ImageGris img)
{
    if (img.size()==0) { throw runtime_error("Tableau vide"); } 
    if (img[0].size() == 0) { throw runtime_error("Tableau vide"); }
    double maximum{maxVector2D(img)};
    for(int i{0}; i <img.size(); i++) {
        for(int j{0}; j < img[0].size(); j++) {
            img[i][j]=img[i][j]*(GRAYMAX/maximum);
        }
    }
    return img;
}

/** filtre de Sobel
 * @param img une image en teintes de gris
 * @return une image en teintes de gris de l'intensite de img
 **/
ImageGris intensite(ImageGris img) {
    if (img.size()==0) { throw runtime_error("Tableau vide"); } 
    if (img[0].size() == 0) { throw runtime_error("Tableau vide"); }
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


void testSobel() {
    CHECK( ImageGrisEgal(intensiteH(imgGrisTest),
              ImageGris( {
                  {0, 0, 0, 0},
                  {0, -373.47, 227.507, 0},
                  {0, -22.1312, 323.866, 0},
                  {0, 0, 0, 0}
              }),
              0.001) );
    CHECK( ImageGrisEgal(intensiteV(imgGrisTest),
              ImageGris( {
                  {0, 0, 0, 0},
                  {0, -15.1398, 150.501, 0},
                  {0, -9.0336, 273.023, 0},
                  {0, 0, 0, 0}
              }),
              0.001) );
    CHECK( ImageGrisEgal(intensite(imgGrisTest),
              ImageGris( {
                  {0, 0, 0, 0},
                  {0, 373.777, 272.782, 0},
                  {0, 23.9039, 423.593, 0},
                  {0, 0, 0, 0}
              }),
              0.001) );

    cout << "Vérifier que les images obtenues dans 'sobel/' sont semblables à celles fournies dans 'sobel/correction/'" << endl;
    ecrirePGM(renormalise(lirePGM("images/Baboon.512.pgm" )), "sobel/Baboon.512.pgm"); // J'ai du ajouter la renormalisation avant écriture car ma fonction ecrirePGM n'accepte pas les images avec des seuils superieurs à 255
    ecrirePGM(renormalise(lirePGM("images/Billes.256.pgm" )), "sobel/Billes.256.pgm");
    ecrirePGM(renormalise(lirePGM("images/Embryos.512.pgm")), "sobel/Embryos.512.pgm");
    ecrirePGM(renormalise(lirePGM("images/House.256.pgm"  )), "sobel/House.256.pgm"); // Sur cette image apparait une bande blanche en haut, due à l'image de base sur laquelle une bande violette est visible en haut
    ecrirePGM(renormalise(lirePGM("images/Lena.512.pgm"   )), "sobel/Lena.512.pgm");
}


int main(){
    testSobel();
    return 0 ;
}

