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

  return 0;
}
