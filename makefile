CC = gcc
RM = rm
CFLAGS = -std=c18 -lpthread -Wall -Wconversion -D_XOPEN_SOURCE=500 -Werror -Wextra -Wpedantic -Wwrite-strings -O2

objects_serveur = main.o

executable_serveur = main

all:  $(executable_serveur)


$(executable_serveur): $(objects_serveur)
	$(CC) $(objects_serveur) -o $(executable_serveur)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM)  $(objects_serveur) $(executable_serveur)
