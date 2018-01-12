#include<iostream>
#include "inih-master/cpp/INIReader.h"

#include "worker.h"
#include "grafik.h"

using namespace std;

int main() {

  // TO DO:
  // [x] Dopisać zabronienia
  // [x] Poprawić f. celu na końcową
  // [ ] Kryterium aspiracji (?)
  // [ ] Interpretacja zabronien

  INIReader reader("config.ini");
  if (reader.ParseError() < 0) {
    cout << "Can't load 'config.ini'" << endl;
    return 1;
  }


  const int NUM_ITERATIONS = reader.GetInteger("algorytm", "iteracje", 10);


  system("clear");

  cout << "A" << endl;

  Grafik G(reader);

  cout << "B" << endl;


  G.loadWorkers("workers.txt");
  G.printWorkers();

  G.loadTemplate("template.txt");
  G.printTemplate();

  cout << endl << endl << "ROZWIAZANIE POCZATKOWE" << endl;

  G.createFirstSolution();
  G.printSolution( G.getBestSolution() );
  cout << "Funkcja celu: " << G.getObjectiveFunction( G.getBestSolution() ) << endl;

  cout << "Liczba zmian:" << endl;
  G.printShiftCount( G.getBestSolution() );

  for(int i=0; i < NUM_ITERATIONS; i++){
    G.createNewSolution();
    // system("clear");
    // G.printSolution();
    // cout << "Funkcja celu: " << G.getObjectiveFunction() << endl;
  }

  cout << endl << endl << "ROZWIAZANIE KONCOWE" << endl;

  G.printSolution( G.getBestSolution() );
  cout << "Funkcja celu: " << G.getObjectiveFunction( G.getBestSolution() ) << endl;

  cout << "Liczba zmian:" << endl;
  G.printShiftCount( G.getBestSolution() );

  return 0;
}
