TARGET   = hey_morty

# Compiler options
#CC       = gcc
#CFLAGS   = -std=c99 -g -c -Wall -Wextra

# Linker options
#LINKER   = gcc
#LFLAGS   = -lm -lcurl

# Directories
#SRCDIR   = src
#OBJDIR   = obj

#OBJECTS  := src/main.o src/neural_network/layer.o src/neural_network/neuron.o src/neural_network/nn.o src/sprec/recognize.o src/sprec/web_client.o src/utils/file.o src/utils/string_builder.o
#SOURCES  := src/main.c src/neural_network/layer.c src/neural_network/neuron.c src/neural_network/nn.c src/sprec/recognize.c src/sprec/web_client.c src/utils/file.c src/utils/string_builder.c
#INCLUDES := src/neural_network/layer.h src/neural_network/neuron.h src/neural_network/nn.h src/sprec/sprec.h src/sprec/recognize.h src/sprec/web_client.h src/utils/file.h src/utils/string_builder.h

OBJS	= src/main.o src/neural_network/layer.o src/neural_network/neuron.o src/neural_network/nn.o src/sprec/recognize.o src/sprec/web_client.o src/utils/file.o src/utils/string_builder.o
SOURCE	= $(shell find $(SOURCEDIR) -regex ' .*\.\(c\)$')
HEADER	= $(shell find $(SOURCEDIR) -regex ' .*\.\(h\)$')
OUT	= hey_morty
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lm -lcurl

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

src/main.o: src/main.c
	$(CC) $(FLAGS) src/main.c -o $@

src/neural_network/layer.o: src/neural_network/layer.c
	$(CC) $(FLAGS) src/neural_network/layer.c -o $@

src/neural_network/neuron.o: src/neural_network/neuron.c
	$(CC) $(FLAGS) src/neural_network/neuron.c -o $@

src/neural_network/nn.o: src/neural_network/nn.c
	$(CC) $(FLAGS) src/neural_network/nn.c -o $@

src/sprec/recognize.o: src/sprec/recognize.c
	$(CC) $(FLAGS) src/sprec/recognize.c -o $@

src/sprec/web_client.o: src/sprec/web_client.c
	$(CC) $(FLAGS) src/sprec/web_client.c -o $@

src/utils/file.o: src/utils/file.c
	$(CC) $(FLAGS) src/utils/file.c -o $@

src/utils/string_builder.o: src/utils/string_builder.c
	$(CC) $(FLAGS) src/utils/string_builder.c -o $@


clean:
	rm -f $(OBJS) $(OUT)
	rm -f *.o
