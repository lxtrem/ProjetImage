#include <stdexcept>
/** @file
 * Lecture et écriture d'images au format PPM
 **/


#include <iostream>
#include <fstream>
#include <string.h>
#include "image.h"
#include "ppm.h"
using namespace std;

Image lirePPM(string source) {
    ifstream image;
    image.open(source);
    if (!image) {throw runtime_error("Fichier non trouve: " + source);}
    string magicNumber,l,L,couleur;
    int i=0;
    while (i<4) { // 4 informations sont ici necessaires
        switch (i){
            case 0: // nombre magique
                image >> magicNumber;
                if (magicNumber[0] == 'P' and magicNumber[1] == '3' and magicNumber.size() == 2) {i=1;}
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
                image >> couleur;
                if ((char)L[0] != (char)'#') {i=4;}
                else {getline(image, L);}
                break;
        }
    }
    int largeur, hauteur, maxCouleur;
    try {
        largeur = stoi(l);
        hauteur = stoi(L); // conversions str->int
        maxCouleur = stoi(couleur);
    }
    catch (const invalid_argument& e) {
        throw runtime_error("La largeur, le niveau de gris ou la longueur n'est pas un entier comme attendu.");
    }
    if (maxCouleur > 65536 or maxCouleur == 0 or largeur == 0 or hauteur == 0 ) { // Les entiers seront toujours positifs, sinon ils n'auraient pas été reconnus comme des nombres plus haut.
        throw runtime_error("La largeur, le niveau de couleur ou la longueur n'ont pas une valeur gerable. (dimensions nulles ? niveau de couleur nul ?)");
    }
    Image img = Image(hauteur);
    string s[3];
    bool err{false}; // Permet de verifier si on a déja affiché une erreur
    bool etatLecture{true};
    for (int i{0}; i < hauteur; i++ ){
        img[i] = vector<Couleur>(largeur);
        for (int j{0}; j< largeur; j++) {
            for (int i{0}; i<3; i++) {
                etatLecture = etatLecture && (image >> s[i]);
            }
            if (etatLecture) {
                if (s[0][0] == '#') { // Gestion des commentaires dans le fichier
                    getline (image, s[0]); // On se debarasse de la ligne
                    j--; // On décrémente pour ne pas laisser cette case vide
                    continue;
                }
                Couleur couleur;
                try {
                    couleur.r = stoi(s[0]);
                    couleur.g = stoi(s[1]);
                    couleur.b = stoi(s[2]);
                }
                catch (const invalid_argument& e) {
                    throw runtime_error("La largeur, le niveau de gris ou la longueur n'est pas un entier comme attendu.");
                }
                if ( err == false && not (couleur.r <= maxCouleur && couleur.r >= 0 && couleur.g <= maxCouleur && couleur.g >= 0 && couleur.b <= maxCouleur && couleur.b >= 0 )) {
                    cerr << "Entier innatendu dans la lecture du fichier (valeur superieure au max indiqué ?)." << endl;
                    err = true;
                }
                img[i][j] = couleur;
            }
            else { throw runtime_error("Caractere innatendu dans la lecture du fichier (Mauvaise longeur ? Valeur non entiere ?).");}
        }
    }
    while (image>>s[0]) { //On verifie ce qui n'a pas été lu
        if ( s[0][0] == '#' ) { // Si ce sont des commentaires, pas de problème
            getline (image, s[0]);
        }
        else {
            throw runtime_error("Taille de l'image superieure à celle indiquée.");
        }
    }
    image.close();
    return img;
}

void ecrirePPM(Image img, string cible) {
    ofstream output;
    output.open(cible);
    if (!output) {throw runtime_error("Fichier non trouve: " + cible);}
    TESTIMAGE(img);
    bool err{false}; // Permet de verifier si on a déja affiché une erreur
    output << "P3\n" << img[0].size() << " " << img.size() << "\n" << GRAYMAX << "\n"; // Header
    for (int i{0}; i < img.size(); i++ ){
        for (int j{0}; j< img[0].size(); j++) {
            if ( err == false && not(img[i][j].r <= GRAYMAX && img[i][j].r >= 0 && img[i][j].g <= GRAYMAX && img[i][j].g >= 0 && img[i][j].b <= GRAYMAX && img[i][j].b >= 0)) { 
                cerr << "Entier innatendu dans la lecture de l'image (valeur superieure à 255 ?)." << endl;
                err = true;
            }
            output << (int)img[i][j].r << " " << (int)img[i][j].g << " " << (int)img[i][j].b <<  " ";
        }
        output << "\n";
    }
    output.close();
}