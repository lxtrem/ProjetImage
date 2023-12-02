#include <stdexcept>
/** @file
 * Lecture et ecriture d'images binaires au format PBM
 **/
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
using namespace std;

/** Structure de données pour représenter une image binaire **/
typedef vector<vector<int> > ImageNB;

#define TESTIMAGE(img)  if (img.size()==0) { throw runtime_error("Tableau vide"); } if (img[0].size() == 0) { throw runtime_error("Tableau vide"); }


/** Construire une image binaire depuis un fichier PBM
 * @param source le nom d'un fichier PBM
 * @return une image binaire (0/1)
 **/
ImageNB lirePBM(string source) {
    ifstream image;
    image.open(source);
    if (!image) {throw runtime_error("Fichier non trouve: " + source);}
    string magicNumber,l,L;
    int i=0;
    while (i<3) { // On a besoin de 3 informations avant de commencer à lire les données
        switch (i){
            case 0: // nombre magique
                image >> magicNumber;
                if (magicNumber[0] == 'P' and magicNumber[1] == '1' and magicNumber.size() == 2) {i=1;}
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
                if (L[0] != '#') {i=3;}
                else {getline(image, L);}
                break;
        }
    }
    int largeur, hauteur;
    try {
        largeur = stoi(l);
        hauteur = stoi(L); // conversions str->int
    }
    catch (const invalid_argument& e) {
        throw runtime_error("La largeur ou la longueur n'est pas un entier comme attendu.");
    }
    if ( largeur == 0 or hauteur == 0 ) { // Les entiers seront toujours positifs, sinon ils n'auraient pas été reconnus comme des nombres plus haut.
        throw runtime_error("La largeur, ou la longueur n'ont pas une valeur gerable. (dimension nulles)");
    }
    ImageNB imageNbTableau = ImageNB(hauteur);
    string s; // On veut pouvoir traiter aisément les commentaires et donc les caractères
    for (int i{0}; i < hauteur; i++ ){
        imageNbTableau[i] = vector<int>(largeur);
        for (int j{0}; j< largeur; j++) {
            if (image >> s) {
                if (s[0] == '#') { // Gestion des commentaires dans le fichier
                    getline (image, s); // On se debarasse de la ligne
                    j--; // On décrémente pour ne pas laisser cette case vide
                } 
                else if ( s == "0" || s == "1") { imageNbTableau[i][j] = stoi(s); }
                else { throw runtime_error("Entier innatendu dans la lecture du fichier");}
            }
            else { throw runtime_error("Erreur innatendue dans la lecture du fichier (fichier trop petit ?).");}
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
    return imageNbTableau;
}


/** Ecrit une image binaire dans un fichier PBM
 * @param img une image binaire (0/1)
 * @param cible le nom d'un fichier PBM
 **/
void ecrirePBM(ImageNB img, string cible) {
    ofstream output;
    output.open(cible);
    if (!output) {throw runtime_error("Fichier non trouve: " + cible);}
    TESTIMAGE(img);
    output << "P1\n" << img.size() << " " << img[0].size() << "\n"; // Header
    for (int i{0}; i < img.size(); i++ ){
        for (int j{0}; j< img[0].size(); j++) {
            if (img[i][j] != 1 and img[i][j] != 0) { throw runtime_error("Entier innatendu dans la lecture du tableau");}
            else { output << img[i][j] << " "; }
        }
        output << "\n"; // Pas necessaire, plus pratique à la lecture du fichier
    }
    output.close();
}



/** Affiche une image binaire PBM à l'écran avec ' ' pour 0 et '@' pour 1
 * @param img une image binaire (0/1)
 **/
void affichePBM(ImageNB img) {
    TESTIMAGE(img);
    for (int i{0}; i < img.size(); i++ ){
        for (int j{0}; j< img[0].size(); j++) {
                if (img[i][j] == 1) { cout << "@"; }
                else if (img[i][j] == 0) { cout << " ";}
                else { throw runtime_error("Entier innatendu dans la lecture du tableau.");}
        }
        cout << endl;
    }
}


/** Echange le noir et le blanc dans une image PBM
 * @param img une image binaire (0/1)
 * @return l'image où le blanc et le noir ont été inversés
 **/
ImageNB inversePBM(ImageNB img) {
    TESTIMAGE(img);
    for (int i{0}; i < img.size(); i++ ){
        for (int j{0}; j< img[0].size(); j++) {
                if (img[i][j] == 1) { img[i][j] = 0; }
                else if (img[i][j] == 0) { img[i][j] = 1;}
                else { throw runtime_error("Entier innatendu dans la lecture du fichier");}
        }
    }
    return img;
}

void testLirePBM(){
    // cout << "Vérifier que les images obtenues dans 'pbm/' sont semblables à celles fournies dans 'pbm/correction/'" << endl;
    ecrirePBM(lirePBM("images/smiley.pbm"),  "pbm/smiley.pbm");
    ecrirePBM(lirePBM("images/cercle.pbm"),  "pbm/cercle.pbm");
    ecrirePBM(lirePBM("images/code.pbm"), "pbm/code.pbm");
    ecrirePBM(lirePBM("images/damier.pbm"), "pbm/damier.pbm");
}

int main(){
    testLirePBM();
    ImageNB smiley(lirePBM("./pbm/smiley.pbm"));
    affichePBM(smiley);
    ImageNB smileyInverse(inversePBM(smiley));
    affichePBM(smileyInverse);
    ecrirePBM(smileyInverse, "./pbm/smiley_inverse.pbm");
    return 0;
}