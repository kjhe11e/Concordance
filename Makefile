all: main.c concordance.o concordance.h
	gcc -o concordance.h main.c
clean:
	rm -rf *.0
	rm main
