#include "grafik.h"

Grafik::Grafik(){
  srand (1);
  P_SWAP = 20;
  TABOO_AGE = 10;
  POGORSZENIE = 2;

  SHIFT_DIFF_K = 0.2;
  SHIFT_CHANGES_K = 0.1;
  PUNISHMENT_K = 0.7;

  EXPORT_TO_CSV = false;
}

Grafik::Grafik(INIReader settings){
  srand (1);
  P_SWAP = settings.GetReal("algorytm", "P_SWAP", 20);
  TABOO_AGE = settings.GetReal("algorytm", "TABOO_AGE", 10);
  POGORSZENIE = settings.GetReal("algorytm", "POGORSZENIE", 2);

  SHIFT_DIFF_K = settings.GetReal("f_celu", "SHIFT_DIFF_K", 5);
  SHIFT_CHANGES_K = settings.GetReal("f_celu", "SHIFT_CHANGES_K", 1);
  PUNISHMENT_K = settings.GetReal("f_celu", "PUNISHMENT_K", 10);

  EXPORT_TO_CSV = settings.GetBoolean("debug", "EXPORT_TO_CSV", false);

  if(EXPORT_TO_CSV){
    _results.open("f_celu.csv");
    if(!_results.good()){
      cout << "Nie mozna otworzyc pliku";
    }
    else {
      _results << "value;shiftDiff;shiftChanges;punishment;adj_shiftDiff;adj_shiftChanges;adj_punishment" << endl;
    }
  }
}

Grafik::~Grafik(){
  if(_results.is_open()) _results.close();
}

void Grafik::loadWorkers(string fileName){
  ifstream inFile;
  inFile.open(fileName);
  if(!inFile.good()){
    cout << "Nie mozna otworzyc pliku";
  }
  else {
    string line;
    for(int i=0; getline(inFile, line); i++){
      string name;
      string skill;
      stringstream ss(line);
      ss >> name;
      Worker w(i, name);
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

void Grafik::createFirstSolution(){

  for(int i=0; i < _template.size(); i++){ // Dla każdej zmiany

    _solution.push_back( vector<Worker>( _template[i].size() ) );
    _best_solution.push_back( vector<Worker>( _template[i].size() ) );
    _taboo_list.push_back( vector<int>( _template[i].size() ) );

    vector<Worker> freeWorkers( _workers );
    for(int j=0; j < _template[i].size(); j++){ // Dla każdej wymaganej umiejętności

      _taboo_list[i][j] = 0;

      int req_skill = _template[i][j];
      Worker tmp;
      for(int k=0; k < freeWorkers.size(); k++){ // Sprawdź wolnych pracowników
        if( freeWorkers[k].hasSkill( req_skill ) ){
          tmp = freeWorkers[k];
          freeWorkers.erase(freeWorkers.begin() + k);
          break;
        }
      }
      _solution[i][j] = tmp;
      _best_solution[i][j] = tmp;

    }
  }


  if(EXPORT_TO_CSV){
    saveData(_solution);
  }
}

void Grafik::printWorkers(){
  cout << endl << "Lista pracownikow: " << endl << endl;
  for(int i=0; i<_workers.size(); i++){
    _workers[i].printDetails();
  }
}

void Grafik::printTemplate(){
  cout << endl << "Szablon: " << endl;
  cout << "Zmiana | Wymagane umiejetnosci" << endl;
  for(int i=0; i<_template.size(); i++){
    cout << setw(6) << i;
    cout << " | ";

    for(int j=0; j< _template[i].size(); j++){
      cout << setw(2) << _template[i][j] << " ";
    }

    cout << endl;
  }
}

void Grafik::printSolution( vector< vector<Worker> > solution ){
  cout << endl << "Rozwiazanie: " << endl;
  cout << "Zmiana | ID Pracownika" << endl;

  for(int i=0; i < solution.size(); i++){

    if(i%3 == 0){
      cout << "-----------------" << endl;
    }

    cout << setw(6) << i;
    cout << " | ";

    for(int j=0; j < solution[i].size(); j++){
      if( solution[i][j].isEmpty() ){
        cout << setw(2) << "#" << " ";
      }
      else {
        cout << setw(2) << solution[i][j].getID() << " ";
      }
    }

    cout << endl;

  }
}

void Grafik::createNewSolution(){

  bool taboo_violation = false;

  // Decrement tabo list
  for(int i=0; i< _taboo_list.size(); i++){
    for(int j=0; j<_taboo_list[i].size(); j++){
      if(_taboo_list[i][j]>0) _taboo_list[i][j]--;
    }
  }

  // Copy existing solution
  vector< vector<Worker> > newSolution(_solution);

  // Free workers avaiable on this shift
  for(int i=0; i < newSolution.size(); i++){
    vector<Worker> freeWorkers(_workers);
    for(int j=0; j<newSolution[i].size(); j++){
      for(int k=0; k < freeWorkers.size(); k++){
        if( freeWorkers[k].getID() == newSolution[i][j].getID() ){
          freeWorkers.erase(freeWorkers.begin()+k);
        }
      }
    }
    random_shuffle ( freeWorkers.begin(), freeWorkers.end() );
    //freeWorkers ready

    for(int j=0; j<newSolution[i].size(); j++){
      if( (rand() % 100 + 1) < P_SWAP ){ //Randomly swap workers with free workers

        if(_taboo_list[i][j] != 0){
          taboo_violation = true;
        }

        int reqSkill = _template[i][j];
        for(int k=0; k < freeWorkers.size(); k++){ // Sprawdź wolnych pracowników
          if( freeWorkers[k].hasSkill( reqSkill ) ){
            freeWorkers.push_back( newSolution[i][j] );
            newSolution[i][j] = freeWorkers[k];

            _taboo_list[i][j] = TABOO_AGE; // Add that place to tabo list for TABOO_AGE iterations

            freeWorkers.erase(freeWorkers.begin() + k);
            break;
          }
        }

      }
    }

  }
  long newObjectiveFunction = getObjectiveFunction(newSolution);

  if( newObjectiveFunction <= getObjectiveFunction(_best_solution) || ( !taboo_violation && newObjectiveFunction <= getObjectiveFunction(_best_solution)+POGORSZENIE) ) { // If generated solution is better

    if(EXPORT_TO_CSV){
      saveData( newSolution );
    }

    _solution = newSolution;
  }

  if( newObjectiveFunction <= getObjectiveFunction(_best_solution) ) {
    _best_solution = newSolution;
  }

}

map<string, float> Grafik::calculateObjectiveFunction( vector< vector<Worker> > solution ){

  map<string, float> objectiveFunctionParameters;

  float shiftDiff = 0;

  float shiftChanges = 0;
  float punishment = 0;

  float objectiveFunction;

  // Wyrównanie liczby zmian dla każdego

  for(int i=0; i<_workers.size(); i++){
    _shiftCount[ _workers[i].getID() ].resize(3);
    _shiftCount[ _workers[i].getID() ][0] = 0;
    _shiftCount[ _workers[i].getID() ][1] = 0;
    _shiftCount[ _workers[i].getID() ][2] = 0;
  }

  int shift = 0;
  for(int i=0; i < solution.size(); i++){
    for(int j=0; j < solution[i].size(); j++){
      int workerID = solution[i][j].getID();
      if(_shiftCount.find(workerID) != _shiftCount.end()){ // Znaleziono
        _shiftCount[workerID][shift]++;
      }
    }
    shift++;
    if(shift==3) shift=0;
  }

  int shiftSum = 0;
  for (map<int,vector<int> >::iterator it=_shiftCount.begin(); it!=_shiftCount.end(); it++){
    for(int i=0; i<(it->second).size(); i++){
      shiftSum += (it->second)[i];
    }
  }

  float avgShifts = shiftSum / (3*_workers.size());

  for (map<int,vector<int> >::iterator it=_shiftCount.begin(); it!=_shiftCount.end(); it++){
    for(int i=0; i<(it->second).size(); i++){
      float diff = avgShifts - (it->second)[i];

      shiftDiff += fabs( diff );
    }
  }

  // Minimalizacja liczby zmian zmian oraz zabronienie pracy zmiana po zmianie
  for(int i=0; i<solution.size(); i++){
    for(int j=0; j < solution[i].size(); j++){
      int workerID = solution[i][j].getID();

      if(i+1 <solution.size()) for(int k=0; k<solution[i+1].size(); k++){
        if(solution[i+1][k].getID() == workerID){ //Pracownik pracuje jedną zmianę później - zabronione
          punishment += 1;
          break;
        }
      }

      if(i+2 <solution.size()) for(int k=0; k<solution[i+2].size(); k++){
        if(solution[i+2][k].getID() == workerID){ //Pracownik pracuje dwie zmiany później - zabronione
          punishment += 1;
          break;
        }
      }

      if(i+4 <solution.size()) for(int k=0; k<solution[i+4].size(); k++){
        if(solution[i+4][k].getID() == workerID){ //Pracownik pracuje cztery zmiany później - dopuszczalne, ale pogorsza f.celu
          shiftChanges += 1;
          break;
        }
      }

      if(i+5 <solution.size()) for(int k=0; k<solution[i+5].size(); k++){
        if(solution[i+5][k].getID() == workerID){ //Pracownik pracuje pięć zmian później - dopuszczalne, ale pogorsza f.celu
          shiftChanges += 1;
          break;
        }
      }

    }
  }

  objectiveFunction = (SHIFT_DIFF_K * shiftDiff) + (SHIFT_CHANGES_K * shiftChanges) + (PUNISHMENT_K * punishment);

  objectiveFunctionParameters["value"] = objectiveFunction;

  objectiveFunctionParameters["shiftDiff"] = shiftDiff;
  objectiveFunctionParameters["shiftChanges"] = shiftChanges;
  objectiveFunctionParameters["punishment"] = punishment;

  objectiveFunctionParameters["adj_shiftDiff"] = SHIFT_DIFF_K * shiftDiff;
  objectiveFunctionParameters["adj_shiftChanges"] = SHIFT_CHANGES_K * shiftChanges;
  objectiveFunctionParameters["adj_punishment"] = PUNISHMENT_K * punishment;

  return objectiveFunctionParameters;

}

float Grafik::getObjectiveFunction( vector< vector<Worker> > solution ){
  return calculateObjectiveFunction(solution)["value"];
}

void Grafik::printShiftCount( vector< vector<Worker> > solution ){

  map<int, vector<int> > shiftCount;

  for(int i=0; i<_workers.size(); i++){
    shiftCount[ _workers[i].getID() ].resize(3);
    shiftCount[ _workers[i].getID() ][0] = 0;
    shiftCount[ _workers[i].getID() ][1] = 0;
    shiftCount[ _workers[i].getID() ][2] = 0;
  }

  int shift = 0;
  for(int i=0; i < solution.size(); i++){
    for(int j=0; j < solution[i].size(); j++){
      int workerID = solution[i][j].getID();
      if(shiftCount.find(workerID) != shiftCount.end()){ // Znaleziono
        shiftCount[workerID][shift]++;
      }
    }
    shift++;
    if(shift==3) shift=0;
  }

  for (map<int,vector<int> >::iterator it=shiftCount.begin(); it!=shiftCount.end(); it++){
    cout << it->first << ": ";
    for(int i=0; i<(it->second).size(); i++){
      cout << (it->second)[i] << " ";
    }
    cout << endl;
  }
}

vector< vector<Worker> > Grafik::getCurrentSolution(){
  return _solution;
}

vector< vector<Worker> > Grafik::getBestSolution(){
  return _best_solution;
}

void Grafik::saveData( vector< vector<Worker> > solution ){

  map<string, float> objectiveFunction = calculateObjectiveFunction(solution);

  if(!_results.good()){
    cout << "Nie mozna otworzyc pliku";
  }
  else {
    _results.imbue(std::locale(std::cout.getloc(), new punct_facet<char, ','>));
    _results <<
      objectiveFunction["value"] << ";" <<
      objectiveFunction["shiftChanges"] << ";" <<
      objectiveFunction["shiftDiff"] << ";" <<
      objectiveFunction["punishment"] << ";" <<
      objectiveFunction["adj_shiftChanges"] << ";" <<
      objectiveFunction["adj_shiftDiff"] << ";" <<
      objectiveFunction["adj_punishment"] <<
      endl;
  }
}
