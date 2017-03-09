all: main.c
	gcc -o main main.c
clean:
	rm -rf *.0
	rm main
