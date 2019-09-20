

all:
	make mm_bench
	make pcrt
	ln -f pcrt tcrt	

	
pcrt: crt.o ep.o et.o etime.o 
	gcc -pthread -o pcrt crt.o ep.o et.o etime.o

crt.o: crt.c
	gcc -c crt.c

ep.o: ep.c etime.o
	gcc -c ep.c

et.o: et.c etime.o
	gcc -c et.c
	
	
mm_bench: mm_thr.o etime.o 
	gcc -pthread -o mm_bench mm_thr.o etime.o

mm_thr.o: mm_thr.c
	gcc -c mm_thr.c

etime.o: etime.c
	gcc -c etime.c


clean:
	rm pcrt tcrt mm_bench *.o
	

