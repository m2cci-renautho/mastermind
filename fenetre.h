#ifndef FENETRES
#define FENETRES

void initColors();
void afficherCombinaison(WINDOW *win, int noLigne, Combinaison combinaison);
void afficherToutesCouleurs(WINDOW *win);
void printResultat(WINDOW *win, int ligne, Resultat res);
void init_fenetres(WINDOW **propositions, WINDOW **reponses, WINDOW **colors, WINDOW **saisie);
void saisirProposition(WINDOW *win, Combinaison *prop);

#endif