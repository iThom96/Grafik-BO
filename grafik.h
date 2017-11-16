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

  private:
    vector<Worker> _workers;
    vector< vector<int> > _template;
    vector< vector<Worker> > _solution;
};

#endif
