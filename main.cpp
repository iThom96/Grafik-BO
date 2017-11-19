#include<iostream>

#include "worker.h"
#include "grafik.h"

using namespace std;

int main() {

  Grafik G;

  G.loadWorkers("workers.txt");
  G.printWorkers();

  G.loadTemplate("template.txt");
  G.printTemplate();

  cout << endl << endl << "ROZWIAZANIE POCZATKOWE" << endl;

  G.createFirstSolution();
  G.printSolution();
  cout << "Funkcja celu: " << G.getObjectiveFunction() << endl;

  cout << endl << endl << "NOWE ROZWIAZANIE" << endl;

  G.createNewSolution();
  G.printSolution();
  cout << "Funkcja celu: " << G.getObjectiveFunction() << endl;

  for(int i=0; i < 1000; i++){
    G.createNewSolution();
    // system("clear");
    // G.printSolution();
    // cout << "Funkcja celu: " << G.getObjectiveFunction() << endl;
  }

  G.printSolution();
  cout << "Funkcja celu: " << G.getObjectiveFunction() << endl;


  return 0;
}
