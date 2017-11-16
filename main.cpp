#include<iostream>
#include<vector>

#include "worker.h"

using namespace std;

int main() {

  vector<Worker> workers = loadWorkers("workers.txt");

  for(int i=0; i<workers.size(); i++){
    workers[i].printDetails();
  }

  return 0;
}
