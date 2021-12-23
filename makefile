all: Measure Sender

Measure: measure.c
	gcc -o Measure measure.c

Sender: sender.c
	gcc -o Sender sender.c

clean:
	rm -f *.o Measure Sender

runs:
	./Measure

runc:
	./Sender

runs-strace:
	strace -f ./Measure
	
runc-strace:
	strace -f ./Sender
