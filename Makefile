grafik: worker.o grafik.o main.o
	gcc worker.o grafik.o main.o -o grafik -lstdc++

main.o: main.cpp
	gcc -c main.cpp

worker.o: worker.cpp worker.h
	gcc -c worker.cpp

grafik.o: grafik.cpp grafik.h worker.cpp worker.h
	gcc -c grafik.cpp

clean:
	rm -rf *o grafik
