#ifndef GRAFIK_H
#define GRAFIK_H

using namespace std;

class Grafik {
  public:
    Grafik();
    void loadWorkers(string fileName);
    void loadTemplate(string fileName);
    void createFirstSolution();

    void printWorkers();
    void printTemplate();
    void printSolution();

    void createNewSolution();

    int calculateObjectiveFunction(vector< vector<Worker> > solution );
    int getObjectiveFunction();

  private:
    vector<Worker> _workers;
    vector< vector<int> > _template;
    vector< vector<Worker> > _solution;
};

#endif
