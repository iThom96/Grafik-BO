#ifndef GRAFIK_H
#define GRAFIK_H

using namespace std;

class Grafik {
  public:
    Grafik();
    Grafik(INIReader settings);

    ~Grafik();

    void loadWorkers(string fileName);
    void loadTemplate(string fileName);
    void createFirstSolution();

    void printWorkers();
    void printTemplate();
    void printSolution( vector< vector<Worker> > solution );

    void createNewSolution();

    map<string, float> calculateObjectiveFunction( vector< vector<Worker> > solution );
    long getObjectiveFunction( vector< vector<Worker> > solution );
    void printShiftCount( vector< vector<Worker> > solution );

    vector< vector<Worker> > getCurrentSolution();
    vector< vector<Worker> > getBestSolution();

    void saveData( vector< vector<Worker> > solution );

  private:
    vector<Worker> _workers;
    vector< vector<int> > _template;
    vector< vector<int> > _taboo_list;
    vector< vector<Worker> > _solution;

    vector< vector<Worker> > _best_solution;

    map<int, vector<int> > _shiftCount;

    // ofstream _results;

    // settings
    float P_SWAP;
    float TABOO_AGE;
    float POGORSZENIE;
    float SHIFT_DIFF_K;
    float SHIFT_CHANGES_K;
    float PUNISHMENT_K;
    bool EXPORT_TO_CSV;
};

#endif
