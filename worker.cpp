#include<iostream>
#include<algorithm>
#include<string>
#include<iomanip>

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
    cout << setw(2) << _skills[i] << " ";
  }
  cout << endl << endl;
}
