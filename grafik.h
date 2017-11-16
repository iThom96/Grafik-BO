#ifndef GRAFIK_H
#define GRAFIK_H

using namespace std;

class Grafik {
  public:
    Grafik();
    void loadWorkers(string fileName);
    void loadTemplate(string fileName);
    void printWorkers();
    void printTemplate();
  private:
    vector<Worker> _workers;
    vector< vector<int> > _template;
    vector< vector<int> > _solution;
};

#endif
