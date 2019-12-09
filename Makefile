GCC=/usr/bin/gcc

scull_test: consumer.o producer.o main.o
	$(GCC) consumer.o producer.o main.o -o scull_test

consumer.o: consumer.c consumer.h
	$(GCC) -Wall consumer.c -c -o consumer.o -g

producer.o: producer.c producer.h
	$(GCC) -Wall producer.c -c -o producer.o -g

main.o: main.c
	$(GCC) -Wall main.c -c -o main.o -g

clean:
	rm scull_test consumer.o producer.o main.o
