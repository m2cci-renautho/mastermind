---
title: "Réseaux  -  Compte-rendu TP4"
author: "Thomas Renaud & Jonathan Delhomme"
geometry: margin=2cm
output: pdf_document
---

# Cahier des charges du Mastermind.

## Choix du serveur et des protocoles
L'objectif est de créer un serveur itératif, de le lancer, puis d'attendre une connexion client. La communication sera faite grâce aux protocoles TCP/IP. Le choix du protocole TCP, avec création de processus fils, permet à plusieurs clients de se connecter simultanément au serveur.


## Algorithme du serveur

L'algorithme du serveur est le suivant :
```
socket
bind
listen
faire
    accept
    fork
    Si fils :
        shutdown
        read/write /*traitement*/
        close
        exit
    close
tant que(vrai)
close
```

## Algorithme du client
L'algorithme du client est le suivant :

```
socket
connect
read/write /*traitement, boucle du jeu*/
close
```