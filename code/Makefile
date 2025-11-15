# Makefile pour le projet TSP
# Génère l'exécutable 'tsp'

CC = gcc
# CFLAGS : -Wall (tous les avertissements), -Wextra (plus d'avertissements), -std=c99 (standard), -lm (lier la bibliothèque mathématique)
CFLAGS = -Wall -Wextra -std=c99
# Utilisez -g pour le débogage avec gdb
# CFLAGS += -g

SOURCES = tsp.c parser.c distance.c matrice.c tsp_utils.c force_brute.c

OBJETS = $(SOURCES:.c=.o)
EN_TETES = tsp.h parser.h distance.h matrice.h tsp_utils.h force_brute.h

# Nom de l'exécutable
EXECUTABLE = tsp

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJETS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJETS)

# Règle pour compiler les fichiers .c en fichiers .o
# Dépend du .c et de tous les .h
%.o: %.c $(EN_TETES)
	$(CC) $(CFLAGS) -c $< -o $@

# Cible clean : supprimer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJETS) $(EXECUTABLE)