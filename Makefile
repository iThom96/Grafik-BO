grafik: worker.o main.o
	gcc worker.o main.o -o grafik -lstdc++

main.o: main.cpp
	gcc -c main.cpp

worker.o: worker.cpp worker.h
	gcc -c worker.cpp

clean:
	rm -rf *o grafik
