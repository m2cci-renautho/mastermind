/******************************************************************************/
/*			Application: ....			              */
/******************************************************************************/
/*									      */
/*			 programme  SERVEUR 				      */
/*									      */
/******************************************************************************/
/*									      */
/*		Auteurs :  ....						      */
/*		Date :  ....						      */
/*									      */
/******************************************************************************/

#include <stdio.h>

#include <sys/signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <ncurses.h>


#include "fon.h"  /* Primitives de la boite a outils */
#include "masterMind.h" /*Primitives du masterMind*/

#define SERVICE_DEFAUT "2111"
#define DEBUG 0

void serveur_appli(char *service); /* programme serveur */
void sendResultat(int socket, Resultat res, char score);
void readProposition(int socket, Combinaison *proposition);
void jouerMasterMindServeur(int socket_client);

/******************************************************************************/

/*---------------- programme serveur ------------------------------*/
int main(int argc, char *argv[])
{

	char *service = SERVICE_DEFAUT; /* numero de service par defaut */
	char q;
	/* Permet de passer un nombre de parametre variable a l'executable */
	switch (argc)
	{
	case 1:
		printf("defaut service = %s\n", service);
		break;
	case 2:
		service = argv[1];
		break;

	default:
		printf("Usage:serveur service (nom ou port) \n");
		exit(1);
	}

	/* service est le service (ou numero de port) auquel sera affecte
	ce serveur*/

	serveur_appli(service);
}

/** Procedure correspondant au traitement du serveur de votre application */
void serveur_appli(char *service)
{

	// Ouverture d'un socket en mode IPv4 avec SOCK_STREAM (TCP)
	int socket_passif = h_socket(AF_INET, SOCK_STREAM);
	int socket_client, code_retour;

	pid_t pid, fils;
	struct sockaddr_in *adress;
	char end;

	// On complète la structure sockaddrZ_in via la fonction adr_socket;
	adr_socket(service, NULL, SOCK_STREAM, &adress);

	// bind
	h_bind(socket_passif, adress);

	// listen
	h_listen(socket_passif, 10);
	printf("En écoute sur le port %s \n", SERVICE_DEFAUT);
	struct sockaddr_in adress_client;
	do
	{

		if ((socket_client = h_accept(socket_passif, &adress_client)) == -1)
		{
			fprintf(stderr, "erreur d'accept");
			h_close(socket_passif);
			exit(EXIT_FAILURE);
		}

		pid = fork();
		if (pid == 0)
		{
			//Processus fils
			h_close(socket_passif);
			printf("Nouvelle connexion socket attribué %d nouveau fils\n", socket_client);
			jouerMasterMindServeur(socket_client);
			h_close(socket_client);
			printf("déconnexion\n");
			exit(0);
		}
		else
		{
			//Processus père
			

			fils = waitpid(-1, &code_retour, WNOHANG);
			if(WIFEXITED(code_retour)){
				printf("Retour au father\n");
			}
			
		
			//Gerer les erreurs
		}
	} while (1);
}

/******************************************************************************/

// Ecrit dans proposition une chaine lue en provenance du client.
void readProposition(int socket, Combinaison *proposition)
{
	h_reads(socket, proposition->str, NBPIONS);
}

/**
 * @brief Renvoie les indications au client après avoir comparé à la combinaison courante.
 *
 * @param socket Le socket du client.
 * @param res Le résultat de la proposition.
 * @param score Le score en cours.
 */
void sendResultat(int socket, Resultat res, char score)
{
	char resultat[3];
	resultat[0] = res.bonneCouleur;
	resultat[1] = res.bonnePosition;
	resultat[2] = score;
	h_writes(socket, resultat, 3);
}

void jouerMasterMindServeur(int socket_client)
{
	Resultat result;
	char score = 0;
	Combinaison combinaison, proposition;

	creerCombinaisonAleatoire(&combinaison,0);
	printf("%s\n", combinaison.str);
	do
	{
		readProposition(socket_client, &proposition);
		comparerCombinaisons(combinaison, proposition, &result);
		score++;
		sendResultat(socket_client, result, score);
	} while (score < NBCOUPS_MAX && result.bonnePosition != NBPIONS);
}