#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<iomanip>
#include <map>
#include <cstdlib>

#include "worker.h"
#include "grafik.h"

#define P_SWAP 20
#define TABOO_AGE 10

Grafik::Grafik(){
  srand (time(NULL));
}

void Grafik::loadWorkers(string fileName){
  ifstream inFile;
  inFile.open(fileName);
  if(!inFile.good()){
    cout << "Nie mozna otworzyc pliku";
  }
  else {
    string line;
    for(int i=0; getline(inFile, line); i++){
      string name;
      string skill;
      stringstream ss(line);
      ss >> name;
      Worker w(i, name);
      while (ss >> skill) {
        w.addSkill( stoi(skill) );
      }
      _workers.push_back(w);
    }
  }
}

void Grafik::loadTemplate(string fileName){
  ifstream inFile;
  inFile.open(fileName);
  if(!inFile.good()){
    cout << "Nie mozna otworzyc pliku";
  }
  else {
    string line;
    while(getline(inFile, line)){
      string skill;
      stringstream ss(line);
      vector<int> skills;
      while (ss >> skill) {
        skills.push_back( stoi(skill) );
      }
      _template.push_back(skills);
    }
  }
}

void Grafik::createFirstSolution(){
  for(int i=0; i < _template.size(); i++){ // Dla każdego dnia

    _solution.push_back( vector<Worker>( _template[i].size() ) );
    _taboo_list.push_back( vector<int>( _template[i].size() ) );

    vector<Worker> freeWorkers( _workers );
    for(int j=0; j < _template[i].size(); j++){ // Dla każdej wymaganej umiejętności

      _taboo_list[i][j] = 0;

      int req_skill = _template[i][j];
      Worker tmp;
      for(int k=0; k < freeWorkers.size(); k++){ // Sprawdź wolnych pracowników
        if( freeWorkers[k].hasSkill( req_skill ) ){
          tmp = freeWorkers[k];
          freeWorkers.erase(freeWorkers.begin() + k);
          break;
        }
      }
      _solution[i][j] = tmp;

    }
  }
  ofstream results("f_celu.csv");
  if(!results.good()){
    cout << "Nie mozna otworzyc pliku";
  }
  else {
    results << calculateObjectiveFunction(_solution) << ",";
  }
  results.close();

}

void Grafik::printWorkers(){
  cout << endl << "Lista pracownikow: " << endl << endl;
  for(int i=0; i<_workers.size(); i++){
    _workers[i].printDetails();
  }
}

void Grafik::printTemplate(){
  cout << endl << "Szablon: " << endl;
  cout << "Zmiana | Wymagane umiejetnosci" << endl;
  for(int i=0; i<_template.size(); i++){
    cout << setw(6) << i;
    cout << " | ";

    for(int j=0; j< _template[i].size(); j++){
      cout << setw(2) << _template[i][j] << " ";
    }

    cout << endl;
  }
}

void Grafik::printSolution(){
  cout << endl << "Rozwiazanie: " << endl;
  cout << "Zmiana | ID Pracownika" << endl;

  for(int i=0; i < _solution.size(); i++){

    if(i%3 == 0){
      cout << "-----------------" << endl;
    }

    cout << setw(6) << i;
    cout << " | ";

    for(int j=0; j < _solution[i].size(); j++){
      if( _solution[i][j].isEmpty() ){
        cout << setw(2) << "#" << " ";
      }
      else {
        cout << setw(2) << _solution[i][j].getID() << " ";
      }
    }

    cout << endl;

  }
}

void Grafik::createNewSolution(){

  for(int i=0; i< _taboo_list.size(); i++){ // Decrement tabo list
    for(int j=0; j<_taboo_list[i].size(); j++){
      if(_taboo_list[i][j]>0) _taboo_list[i][j]--;
    }
  }

  vector< vector<Worker> > newSolution(_solution);

  for(int i=0; i < newSolution.size(); i++){

    vector<Worker> freeWorkers(_workers);
    for(int j=0; j<newSolution[i].size(); j++){
      for(int k=0; k < freeWorkers.size(); k++){
        if( freeWorkers[k].getID() == newSolution[i][j].getID() ){
          freeWorkers.erase(freeWorkers.begin()+k);
        }
      }
    } // freeWorkers ready

    random_shuffle ( freeWorkers.begin(), freeWorkers.end() );

    for(int j=0; j<newSolution[i].size(); j++){
      if( _taboo_list[i][j]==0 && (rand() % 100 + 1) < P_SWAP ){

        int reqSkill = _template[i][j];
        for(int k=0; k < freeWorkers.size(); k++){ // Sprawdź wolnych pracowników
          if( freeWorkers[k].hasSkill( reqSkill ) ){
            freeWorkers.push_back( newSolution[i][j] );
            newSolution[i][j] = freeWorkers[k];

            _taboo_list[i][j] = TABOO_AGE;

            freeWorkers.erase(freeWorkers.begin() + k);
            break;
          }
        }

      }
    }

  }
  long newObjectiveFunction = calculateObjectiveFunction(newSolution);
  if( newObjectiveFunction <= calculateObjectiveFunction(_solution) ) {

    ofstream results("f_celu.csv", ios_base::app);
    if(!results.good()){
      cout << "Nie mozna otworzyc pliku";
    }
    else {
      results << newObjectiveFunction << ",";
    }
    results.close();

    _solution = newSolution;
  }

}

long Grafik::calculateObjectiveFunction( vector< vector<Worker> > solution ){
  int minShifts = 1000;
  int maxShifts = -1;
  int shiftDiff;

  long shiftChanges = 0;
  long punishment = 0;

  long objectiveFunction;

  map<int, int> shiftCount;

  for(int i=0; i<_workers.size(); i++){
    shiftCount[ _workers[i].getID() ] = 0;
  }

  for(int i=0; i < solution.size(); i++){
    for(int j=0; j < solution[i].size(); j++){
      int workerID = solution[i][j].getID();
      if(shiftCount.find(workerID) != shiftCount.end()){ // Znaleziono
        shiftCount[workerID]++;
      }
    }
  }

  for (map<int,int>::iterator it=shiftCount.begin(); it!=shiftCount.end(); it++){
    if( it->second < minShifts ) minShifts = it->second;
    if( it->second > maxShifts ) maxShifts = it->second;
  }

  shiftDiff = maxShifts - minShifts;

  for(int i=0; i<solution.size(); i++){
    for(int j=0; j < solution[i].size(); j++){
      int workerID = solution[i][j].getID();

      if(i+1 <solution.size()) for(int k=0; k<solution[i+1].size(); k++){
        if(solution[i+1][k].getID() == workerID){ //Pracownik pracuje jedną zmianę później - zabronione
          punishment += 1;
          break;
        }
      }

      if(i+2 <solution.size()) for(int k=0; k<solution[i+2].size(); k++){
        if(solution[i+2][k].getID() == workerID){ //Pracownik pracuje dwie zmiany później - zabronione
          punishment += 1;
          break;
        }
      }

      if(i+4 <solution.size()) for(int k=0; k<solution[i+4].size(); k++){
        if(solution[i+4][k].getID() == workerID){ //Pracownik pracuje cztery zmiany później - dopuszczalne
          shiftChanges += 1;
          break;
        }
      }

      if(i+5 <solution.size()) for(int k=0; k<solution[i+5].size(); k++){
        if(solution[i+5][k].getID() == workerID){ //Pracownik pracuje pięć zmian później - dopuszczalne
          shiftChanges += 1;
          break;
        }
      }

    }
  }

  objectiveFunction = 0.5 * shiftDiff + 0.5 * shiftChanges + 2*punishment;
  return objectiveFunction;

}

int Grafik::getObjectiveFunction(){
  return calculateObjectiveFunction(_solution);
}
