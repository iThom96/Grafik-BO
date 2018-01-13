#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#define NUM_WORKERS 10
#define NUM_SKILLS 3
#define NUM_SHIFTS 21
#define MAX_WORKPLACES 3

#define P_WORKPLACE 60
#define P_WORKER 80

int main(){

  srand(time(NULL));

  ofstream szablon;
  ofstream workers;

  szablon.open("template.txt");
  if(szablon.good()){
    for(int i=0; i<NUM_SHIFTS; i++){
      stringstream line;
      int count = 1;
      for(int j=1; j<=NUM_SKILLS; j++){
        if(rand()%100 < P_WORKPLACE && count < MAX_WORKPLACES){
          line << j << " ";
          count++;
        }
      }
      szablon << line.str() << endl;
    }
  }
  szablon.close();


  workers.open("workers.txt");
  if(workers.good()){
    for(int i=0; i<NUM_WORKERS; i++){
      stringstream line;
      line << "Name" << i << " ";
      for(int j=1; j<=NUM_SKILLS; j++){
        if(rand()%100 + 1 < P_WORKER){
          line << j << " ";
        }
      }
      workers << line.str() << endl;
    }
  }
  workers.close();
}
