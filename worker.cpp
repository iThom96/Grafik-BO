#include<iostream>
#include<algorithm>
#include<string>
#include<sstream>
#include<fstream>

#include "worker.h"

using namespace std;

Worker::Worker():_name("No Name"){
  // cout << "Worker created with no skills" << endl << endl;
}

Worker::Worker(string name):_name(name) {
  // cout << "Worker created with name " << name << endl << endl;
}

vector<int> Worker::getSkills(){
  return _skills;
}

void Worker::addSkill(int skillID){
  _skills.push_back(skillID);
  sort(_skills.begin(), _skills.end());
}

void Worker::printDetails(){
  cout << "Name: " << _name << endl;
  cout << "Skills: ";
  for(int i=0; i<_skills.size(); i++){
    cout << _skills[i] << ", ";
  }
  cout << endl << endl;
}


vector<Worker> loadWorkers(string fileName){
  ifstream inFile;
  vector<Worker> workers;

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
      workers.push_back(w);
    }
  }
  return workers;
}
