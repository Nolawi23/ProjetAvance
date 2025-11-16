# Makefile pour le projet TSP
# Génère l'exécutable 'tsp'

CC = gcc
# CFLAGS : -Wall (tous les avertissements), -Wextra (plus d'avertissements), -std=c99 (standard), -lm (lier la bibliothèque mathématique)
# L'option -lm pour la librairie mathématique est ajoutée au LDFLAGS
CFLAGS = -Wall -Wextra -std=c99
# Utilisez -g pour le débogage avec gdb
# CFLAGS += -g

# LDFLAGS: Options passées à l'éditeur de liens, comme -lm pour la librairie mathématique
LDFLAGS = -lm

# Utiliser 'wildcard' pour trouver TOUS les fichiers .c et .h dans le répertoire
# -----------------------------------------------------------------------------
SOURCES = $(wildcard *.c)
OBJETS = $(SOURCES:.c=.o)
EN_TETES = $(wildcard *.h)
# -----------------------------------------------------------------------------

# Nom de l'exécutable
EXECUTABLE = tsp

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJETS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJETS) $(LDFLAGS)

# Règle pour compiler les fichiers .c en fichiers .o
# Dépend du .c et de tous les .h
%.o: %.c $(EN_TETES)
	$(CC) $(CFLAGS) -c $< -o $@

# Cible clean : supprimer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJETS) $(EXECUTABLE)

.PHONY: all clean