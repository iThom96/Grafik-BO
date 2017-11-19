#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<iomanip>
#include <map>
#include <cstdlib>

#include "worker.h"
#include "grafik.h"

Grafik::Grafik(){}

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
    vector<Worker> freeWorkers( _workers );
    for(int j=0; j < _template[i].size(); j++){ // Dla każdej wymaganej umiejętności

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
}

void Grafik::printWorkers(){
  cout << endl << "Lista pracownikow: " << endl << endl;
  for(int i=0; i<_workers.size(); i++){
    _workers[i].printDetails();
  }
}

void Grafik::printTemplate(){
  cout << endl << "Szablon: " << endl << endl;
  cout << "Dzien | Wymagane umiejetnosci" << endl;
  for(int i=0; i<_template.size(); i++){
    cout << setw(5) << i;
    cout << " | ";

    for(int j=0; j< _template[i].size(); j++){
      cout << setw(2) << _template[i][j] << " ";
    }

    cout << endl;
  }
  cout << endl;
}

void Grafik::printSolution(){
  cout << endl << "Rozwiazanie: " << endl;
  cout << "Dzien | ID Pracownika" << endl;
  for(int i=0; i < _solution.size(); i++){
    cout << setw(5) << i;
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
  cout << endl;
}

void Grafik::createNewSolution(){
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

    for(int j=0; j<newSolution[i].size(); j++){
      if( (rand() % 100 + 1) < 20 ){

        int reqSkill = _template[i][j];
        for(int k=0; k < freeWorkers.size(); k++){ // Sprawdź wolnych pracowników
          if( freeWorkers[k].hasSkill( reqSkill ) ){
            freeWorkers.push_back( newSolution[i][j] );
            newSolution[i][j] = freeWorkers[k];
            freeWorkers.erase(freeWorkers.begin() + k);
            break;
          }
        }

      }
    }

    // if( true || (rand() % 100 + 1) < 70 ){
    //   int changing = rand() % (newSolution[i].size()-1);
    // }

  }

  if( calculateObjectiveFunction(newSolution) <= calculateObjectiveFunction(_solution) ) {
    _solution = newSolution;
  }

}

int Grafik::calculateObjectiveFunction( vector< vector<Worker> > solution ){
  int minShifts = 1000;
  int maxShifts = -1;

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

  return maxShifts - minShifts;

}

int Grafik::getObjectiveFunction(){
  return calculateObjectiveFunction(_solution);
}
