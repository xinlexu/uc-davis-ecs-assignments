CC = gcc
CFLAG = -m32 -g -Wall

all: testfiboi testfibor testmul

testfiboi: testfiboi.o fiboi.o
	$(CC) $^ -o $@ $(CFLAG)

testfibor: testfibor.o fibor.o
	$(CC) $^ -o $@ $(CFLAG)

testmul: testmul.o mul.o
	$(CC) $^ -o $@ $(CFLAG)

%.o: %.s
	$(CC) -c $< -o $@ $(CFLAG)
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAG)

.PHONY: clean
clean: 
	rm -rf *.o testfiboi testfibor testmul

