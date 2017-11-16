#include<iostream>
#include<algorithm>
#include<string>
#include<iomanip>

#include "worker.h"

using namespace std;

Worker::Worker():_name("No Name"),_id(-1){}
Worker::Worker(int id):_id(id){}
Worker::Worker(string name):_name(name){}
Worker::Worker(int id, string name):_id(id),_name(name){}

bool Worker::isEmpty(){
  return _id == -1;
}

vector<int> Worker::getSkills(){
  return _skills;
}

int Worker::getID(){
  return _id;
}

bool Worker::hasSkill(int skillID){
  for(int i=0; i < _skills.size(); i++){
    if( _skills[i] == skillID ) return true;
  }
  return false;
}

void Worker::addSkill(int skillID){
  _skills.push_back(skillID);
  sort(_skills.begin(), _skills.end());
}

void Worker::printDetails(){
  cout << "ID: " << _id << endl;
  cout << "Imie: " << _name << endl;
  cout << "Umiejetnosci: ";
  for(int i=0; i < _skills.size(); i++){
    cout << setw(2) << _skills[i] << " ";
  }
  cout << endl << endl;
}
