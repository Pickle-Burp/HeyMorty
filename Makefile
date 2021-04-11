TARGET   = hey_morty
OBJS	 = src/main.o src/neural_network/layer.o src/neural_network/neuron.o src/neural_network/nn.o src/sprec/recognize.o src/sprec/web_client.o src/utils/file.o src/utils/string_builder.o
SOURCE	 = $(shell find $(SOURCEDIR) -regex ' .*\.\(c\)$')
HEADER	 = $(shell find $(SOURCEDIR) -regex ' .*\.\(h\)$')
OUT	     = hey_morty
CC	     = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = -lm -lcurl

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

%.o: %.c
	$(CC) $(FLAGS) $^ -o $@


clean:
	rm -f $(OBJS) $(OUT)
	rm -f *.o