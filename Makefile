# paths
MODULES = ../Ipli
INCLUDE = ../Ipli

# compiler
CC = gcc

# Compile options. Το -I<dir> λέει στον compiler να αναζητήσει εκεί include files
CFLAGS = -Wall -Werror -g -I$(INCLUDE)

# Αρχεία .o
OBJS = ipli.o $(MODULES)/iplfunction.o ipllexer.o

# Το εκτελέσιμο πρόγραμμα
EXEC = ipli

# Παράμετροι για δοκιμαστική εκτέλεση
ARGS = primes.ipl

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)

run: $(EXEC)
	./$(EXEC) $(ARGS)