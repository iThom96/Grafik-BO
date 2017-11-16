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
    while(getline(inFile, line)){
      string name;
      string skill;
      stringstream ss(line);
      ss >> name;
      Worker w(name);
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

void Grafik::printWorkers(){
  for(int i=0; i<_workers.size(); i++){
    _workers[i].printDetails();
  }
}

void Grafik::printTemplate(){
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
