# Compiler
CC = gcc

# Options de compilation
CFLAGS = -Wall -Wextra -pedantic -std=c11

# Bibliothèques à lier
LIBS = -lncurses

# Nom de l'exécutable
TARGET = dungeon

# Fichiers source
SRCS = main.c player.c gen.c menu.c

# Fichiers objets (générés à partir des fichiers source)
OBJS = $(SRCS:.c=.o)

# Règle par défaut (ce qui est exécuté lorsque vous tapez `make`)
all: $(TARGET)

# Comment construire l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LIBS)

# Comment compiler les fichiers .c en fichiers .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers générés
clean:
	rm -f $(OBJS) $(TARGET)

# Règle pour tout nettoyer, y compris les fichiers de sauvegarde (~) et les fichiers temporaires
distclean: clean
	rm -f *~

run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: all clean distclean