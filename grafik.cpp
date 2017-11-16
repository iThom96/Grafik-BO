#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<iomanip>

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
    // cout << "Pushing vector for day " << i << endl;
    vector<Worker> freeWorkers( _workers );
    for(int j=0; j < _template[i].size(); j++){ // Dla każdej wymaganej umiejętności
      int req_skill = _template[i][j];
      Worker tmp;
      for(int k=0; k < freeWorkers.size(); k++){
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
  cout << endl << "Rozwiazanie: " << endl << endl;
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
