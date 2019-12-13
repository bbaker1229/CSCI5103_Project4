GCC=/usr/bin/gcc

all: consumer producer prodcons

consumer: consumer.o
	$(GCC) consumer.o -o consumer

producer: producer.o
	$(GCC) producer.o -o producer

prodcons: prodcons.o
	$(GCC) prodcons.o -o prodcons

consumer.o: consumer.c
	$(GCC) -Wall consumer.c -c -o consumer.o -g

producer.o: producer.c
	$(GCC) -Wall producer.c -c -o producer.o -g

prodcons.o: prodcons.c
	$(GCC) -Wall prodcons.c -c -o prodcons.o -g

clean:
	rm consumer producer prodcons *.o *.log
