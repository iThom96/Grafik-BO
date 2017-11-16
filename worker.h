#ifndef WORKER_H
#define WORKER_H

#include<vector>
#include<string>

using namespace std;

class Worker {
  public:

    Worker();
    Worker(string name);

    vector<int> getSkills();
    bool hasSkill(int skillID);
    void addSkill(int skillID);
    void printDetails();
  private:
    string _name;
    vector<int> _skills;
};

#endif
