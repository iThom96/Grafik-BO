grafik: worker.o grafik.o main.o ini.o INIReader.o
	gcc worker.o grafik.o main.o ini.o INIReader.o -o grafik -lstdc++

main.o: main.cpp
	gcc -c main.cpp

worker.o: worker.cpp worker.h
	gcc -c worker.cpp

grafik.o: grafik.cpp grafik.h worker.cpp worker.h
	gcc -c grafik.cpp

ini.o: inih-master/ini.c inih-master/ini.h
	gcc -c inih-master/ini.c

INIReader.o: inih-master/ini.c inih-master/ini.h inih-master/cpp/iniReader.cpp inih-master/cpp/iniReader.h
	gcc -c inih-master/cpp/iniReader.cpp

clean:
	rm -rf *o grafik *.csv
