#include <stdexcept>
/** @file
 * Lecture et ecriture d'images en niveau de gris au format PGM
 **/
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
using namespace std;
#define GRAYMAX 255
#define TESTIMAGE(img)  if (img.size()==0) { throw runtime_error("Tableau vide"); } if (img[0].size() == 0) { throw runtime_error("Tableau vide"); }

/** Structure de donnees pour representer une image en teintes de gris **/
typedef vector<vector<double> > ImageGris;


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

/** Retourne le maximum d'un tableau 2D
 * @param tableau le tableau a explorer
 * @param max le maximum des valeurs trouvées
 **/
inline double maxVector2D(vector<vector<double>> tableau) {
    TESTIMAGE(tableau);
    double maximum{tableau[0][0]};
    for (vector<double> i: tableau) {
        for (double j: i) {
            maximum = (j>maximum) ? j : maximum;
        }
    }
    return maximum;
}

/** Ecrit une image en teintes de gris dans un fichier PGM
 * @param img une image en teintes de gris
 * @param cible le nom d'un fichier PGM
 **/
void ecrirePGM(ImageGris img, string cible) {
    ofstream output;
    output.open(cible);
    if (!output) {throw runtime_error("Fichier non trouve: " + cible);}
    TESTIMAGE(img);
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

/** Construit une image cliché d'une image en niveau de gris
 * @param img une image en niveau de gris
 * @return l'image cliché qui inverse les niveaux de gris
 **/
ImageGris inversePGM(ImageGris img) {
    TESTIMAGE(img);
    for (int i{0}; i < img.size(); i++ ){
        for (int j{0}; j< img[0].size(); j++) {
                if (img[i][j] >= 0 and img[i][j] <= GRAYMAX) { img[i][j] = GRAYMAX - img[i][j];}
                else { throw runtime_error("Entier innatendu dans la lecture du fichier");}
        }
    }
    return img;
}

void lirePGMTest(){
    // cout << "Vérifier que les images obtenues dans 'pgm/' sont semblables à celles fournies dans 'pgm/correction/'" << endl;
    ecrirePGM(lirePGM("images/brain.pgm"),  "pgm/brain.pgm");
    ecrirePGM(lirePGM("images/illusion.pgm"),  "pgm/illusion.pgm");
}

void inversePGMTest() {
    ImageGris imageOriginale;
    // cout << "Vérifier que les images obtenues dans 'pgm/' sont semblables à celles fournies dans 'pgm/correction/'" << endl;
    imageOriginale = lirePGM("images/brain.pgm");
    ecrirePGM(inversePGM(imageOriginale),  "pgm/brain-inverse.pgm");
    imageOriginale = lirePGM("images/illusion.pgm");
    ecrirePGM(inversePGM(imageOriginale),  "pgm/illusion-inverse.pgm");
}

int main() {
    lirePGMTest();
    inversePGMTest();
    return 0;
}