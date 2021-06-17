TARGET	= hey_morty
OBJS	= src/main.o src/cmd/execution.o src/cmd/spell_check.o src/sprec/recognize.o src/sprec/web_client.o src/utils/file.o src/utils/string_builder.o src/ui/ui.o src/cnn/dataset.o src/cnn/functions.o src/cnn/loss_functions.o src/cnn/data.o src/cnn/training.o src/cnn/neuralnet.o src/cnn/optimizer.o src/cnn/tensor.o src/cnn/trainer.o src/cnn/dataset.o
SOURCE	= $(shell find $(SOURCEDIR) -regex ' .*\.\(c\)$')
HEADER	= $(shell find $(SOURCEDIR) -regex ' .*\.\(h\)$')
OUT		= hey_morty
CC		= gcc
FLAGS	= -g -c -Wall -Wextra `pkg-config --cflags glib-2.0` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags json-c`
LFLAGS	= -lm -lcurl -ltensorflow `pkg-config --libs glib-2.0` `pkg-config --libs gtk+-3.0` `pkg-config --libs json-c`

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

%.o: %.c
	$(CC) $(FLAGS) $^ -o $@

clean:
	rm -f $(OBJS) $(OUT)
	rm -f *.o
	rm -f $(TARGET)
