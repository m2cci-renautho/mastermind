#include <ncurses.h>

#include "masterMind.h"
#include "fenetre.h"


void initColors()
{
    start_color();

    for (int i = 0; i < NBCOULEURS; i++)
    {   
        switch (ENSEMBLE_COULEURS[i])
        {
        case 'Y':
            init_pair(i+1, COLOR_BLACK, 226);
            break;
        case 'B':
            init_pair(i + 1, 15, 21);
            break;
        case 'R':
            init_pair(i + 1, COLOR_BLACK, 196);
            break;
        case 'G':
            init_pair(i + 1, COLOR_BLACK, 40);
            break;
        case 'O':
            init_pair(i + 1, COLOR_BLACK, 208);
            break;
        case 'W':
            init_pair(i + 1, COLOR_BLACK, 15);
            break;
        case 'V':
            init_pair(i + 1, 15, 54);
            break;
        case 'F':
            init_pair(i + 1, 15, 201);
            break;
        }
    }
}

void init_fenetres(WINDOW **propositions, WINDOW **reponses, WINDOW **colors, WINDOW **saisie)
{
    initscr();
    initColors();

    int col = 0;
    int ligne = 0;

    *propositions = subwin(stdscr, 2 * NBCOUPS_MAX + 1, 2 * NBPIONS + 3, col, ligne);
    ligne += 2 * NBPIONS + 3;
    *reponses = subwin(stdscr, 2 * NBCOUPS_MAX + 1, NBPIONS + 2, 0, ligne);
    col += 2 * NBCOUPS_MAX + 1;
    *colors = subwin(stdscr, 3, 2 * NBCOULEURS + 1, col, 0);
    col += 3;
    *saisie = subwin(stdscr, 3, 17, col, 0);

    box(*propositions, ACS_VLINE, ACS_HLINE);
    box(*reponses, ACS_VLINE, ACS_HLINE);
    box(*colors, ACS_VLINE, ACS_HLINE);
    box(*saisie, ACS_VLINE, ACS_HLINE);
}

void afficherCombinaison(WINDOW *win, int noLigne, Combinaison comb)
{
    int col;
    for (int i = 0; i < NBPIONS; i++)
    {
        wattron(win, COLOR_PAIR(i + 1));
        mvwaddch(win, noLigne, 2 * i, ENSEMBLE_COULEURS[i]);
        wattroff(win, COLOR_PAIR(i + 1));  
    }
    wrefresh(win);
}

void saisirProposition(WINDOW *win, Combinaison *prop){

    wgetnstr(win, prop->str, NBPIONS);
}

void afficherToutesCouleurs(WINDOW *win)
{

    for (int i = 0; i < NBCOULEURS; i++)
    {
        wattron(win, COLOR_PAIR(i + 1));
        mvwaddch(win, 1, 2 * i + 1, ENSEMBLE_COULEURS[i]);
        wattroff(win, COLOR_PAIR(i + 1));
    }
    wrefresh(win);
}

void printResultat(WINDOW *win, int ligne, Resultat res)
{

    short blanc = NBCOULEURS + 1;
    short rouge = NBCOULEURS + 2;

    init_pair(rouge, 196, COLOR_BLACK);
    init_pair(blanc, 15, COLOR_BLACK);
    char col = 1;

    for (char i = 0; i < res.bonneCouleur; i++)
    {
        wattron(win, COLOR_PAIR(blanc));
        mvwaddch(win, ligne, col, '|');
        wattroff(win, COLOR_PAIR(blanc));
        col++;
    }
    for (char i = 0; i < res.bonnePosition; i++)
    {
        wattron(win, COLOR_PAIR(rouge));
        mvwaddch(win, ligne, col, '|');
        wattroff(win, COLOR_PAIR(rouge));
        col++;
    }
    wrefresh(win);
}