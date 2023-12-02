#include <stdexcept>
/** @file
 * Affichage d'une image PBM
 **/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;


void affichePBM(string source)  {
    ifstream image;
    image.open(source);
    if (!image) {throw runtime_error("Fichier non trouve: " + source);}
    string magicNumber,l,L;
    int i=0;
    while (i<3) { // On a besion de 3 informations, nombre magique, longueur et largeur
        switch (i){
            case 0:
                image >> magicNumber;
                if (magicNumber[0] == 'P' and magicNumber[1] == '1' and magicNumber.size() == 2) {i=1;}
                else if (magicNumber[0] == '#') {
                    getline(image, magicNumber); // On passe la ligne, le retour n'importe pas
                }
                else {throw runtime_error("Le nombre magique n'a pas ou mal été renseigné."); }
                break;
            case 1:
                image >> l;
                if ((char)l[0] != (char)'#') { i=2;}
                else {getline(image, l);}
                break;
            case 2:
                image >> L;
                if (L[0] != '#') {i=3;}
                else {getline(image, L);}
                break;
        }
    }
    int largeur, hauteur;
    try {
        largeur = stoi(l); // La gestion des valeurs nulles sera faite dans l'implémentation des fichiers suivants.
        hauteur = stoi(L); // conversions str->int
    }
    catch (const invalid_argument& e) {
        throw runtime_error("La largeur ou la longueur n'est pas un entier comme attendu.");
    }
    int s;
    for (int i{0}; i < hauteur; i++ ){
        for (int j{0}; j< largeur; j++) {
            if (image >> s) { // Les commentaires ne sont pas gérés, dans ce fichier de test, ils le seront dans les fichiers utilisant les types ImageNB, ImageGris etc.
                if (s == 1) { cout << "@"; }
                else if (s == 0) { cout << " ";}
                else { throw runtime_error("Entier innatendu dans la lecture du fichier");}
            }
            else { throw runtime_error("Caractere innatendu dans la lecture du fichier (Mauvaise longeur ? Valeur non entiere ?).");}
        }
        cout << endl;
    }
    image.close();
}

void inversePBM(string source, string cible) {
    ifstream image;
    image.open(source);
    if (!image) {throw runtime_error("Fichier non trouve: " + source);}
    ofstream output;
    output.open(cible);
    if (!output) {throw runtime_error("Fichier non trouve: " + cible);}
    string magicNumber,l,L;
    int i=0;
    while (i<3) { // Idem, 3 informations doivent etre récoltées
        switch (i){
            case 0:
                image >> magicNumber;
                if (magicNumber[0] == 'P') {i=1;}
                else if (magicNumber[0] == '#') {
                    getline(image, magicNumber); // On passe la ligne, le retour n'importe pas
                }
                else {throw runtime_error("Le nombre magique n'a pas ou mal été renseigné."); }
                break;
            case 1:
                image >> l;
                if ((char)l[0] != (char)'#') { i=2;}
                else {getline(image, l);}
                break;
            case 2:
                image >> L;
                if (L[0] != '#') {i=3;}
                else {getline(image, L);}
                break;
        }
    }
    int largeur, hauteur;
    try {
        largeur = stoi(l); // conversions str->int
        hauteur = stoi(L); // La gestion des valeurs nulles sera faite dans l'implémentation des fichiers suivants.
    }
    catch (const invalid_argument& e) {
        throw runtime_error("La largeur ou la longueur n'est pas un entier comme attendu.");
    }
    output << magicNumber << "\n" << largeur << " " << hauteur << "\n";
    int s;
    for (int i{0}; i < hauteur; i++ ){ // A partir d'ici, on pourrait utiliser un while et utiliser 1-s, mais on préfère s'assurer que les valeurs sont bonnes
        for (int j{0}; j< largeur; j++) {
            if (image >> s) { // Les commentaires ne sont pas gérés, dans ce fichier de test, ils le seront dans les fichiers utilisant les types ImageNB, ImageGris etc.
                if (s == 1) { output << 0 << " "; }
                else if (s == 0) { output << 1 << " ";}
                else { throw runtime_error("Entier innatendu dans la lecture du fichier");}
            }
            else { throw runtime_error("Caractere innatendu dans la lecture du fichier (Mauvaise longeur ? Valeur non entiere ?).");}
        }
        output << "\n"; // Pas necessaire, plus pratique à la lecture du fichier
    }
    image.close();
    output.close();
}


int main(){
    cout << "Affichage du smiley " << endl;
    affichePBM("./images/smiley.pbm");
    cout << "Fabrication du smiley inversé (vérifiez que le nouveau fichier a été créé dans le dosier pbm/smiley-inverse.pbm" << endl;
    inversePBM("images/smiley.pbm","pbm/smiley-inverse.pbm");
    cout << "Affichage du smiley inversé" << endl;
    affichePBM("./pbm/smiley-inverse.pbm");
    return 0;
}

