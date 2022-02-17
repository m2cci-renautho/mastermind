
OBJ1 = fon.o  masterMind.o client.o 
OBJ2 = fon.o masterMind.o serveur.o 
OPTIONS	=
# Adaptation a Darwin / MacOS X avec fink
# Du fait de l'absence de libtermcap on se fait pas mal
# cracher dessus mais ca marche...
ifeq ($(shell uname), Darwin)
LFLAGS	+= -L/opt/local/lib
CFLAGS	+= -I /opt/local/include
endif
#Changer si necessaire le chemin d'acces aux librairies

# Adaptation a Linux
ifeq ($(shell uname), Linux)
OPTIONS	+= -ltermcap
endif

# Adaptation a FreeBSD
# Attention : il faut utiliser gmake...
ifeq ($(shell uname),FreeBSD)
OPTIONS	+= -ltermcap
endif

# Adaptation a Solaris

ifeq ($(shell uname),SunOS)
OPTIONS	+= -ltermcap  -lsocket -lnsl
CFLAGS	+= -I..
endif

EXEC = ${OBJ1} client ${OBJ2} serveur
all: ${EXEC} 	


#Ajout des primitives du jeu de Mastermind
masterMind.o :  masterMind.h masterMind.c
	gcc -Wall -c masterMind.c  -L /usr/ccs/lib -L /usr/ucblib -lncurses -ltermcap  

fon.o :  fon.h fon.c
# active le mode trace debug au cas où.
#	gcc -DDEBUG -c fon.c 
	gcc -c fon.c
	
client.o : fon.h client.c 
	gcc  $(CFLAGS) -c  client.c	

serveur.o : fon.h serveur.c 
	gcc  $(CFLAGS) -c  serveur.c	

client : ${OBJ1}	
	gcc $(LFLAGS) ${OBJ1} -o client -lcurses  -ggdb3  $(OPTIONS)

serveur : ${OBJ2}	
	gcc $(LFLAGS) ${OBJ2} -o serveur -lcurses   $(OPTIONS)

clean : 
	rm -f ${EXEC} ${OBJ1} ${OBJ2} core

