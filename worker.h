#ifndef WORKER_H
#define WORKER_H

#include<vector>
#include<string>

using namespace std;

class Worker {
  public:
    Worker();
    Worker(int id);
    Worker(string name);
    Worker(int id, string name);

    vector<int> getSkills();
    int getID();
    bool isEmpty();
    bool hasSkill(int skillID);
    void addSkill(int skillID);
    void printDetails();
  private:
    int _id;
    string _name;
    vector<int> _skills;
};

#endif
