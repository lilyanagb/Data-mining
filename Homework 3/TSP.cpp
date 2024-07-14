#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <cmath>

using namespace std;

struct City {
    string name; 
    double x;
    double y;
};

vector<int> init(int N){
    vector<int> population(N);

    for(int i = 0; i < N; i++){
        population[i] = i;
    }

    random_shuffle(population.begin(), population.end()); 

    return population;
}

void generateRandomCoordinates(vector<City>& cities) {
    int N = cities.size();

    for (int i = 0; i < N; i++) {
        cout << "Enter the name and coordinates of city " << i + 1 << ": ";
        cin >> cities[i].name >> cities[i].x >> cities[i].y;
    }

	// for (int i = 0; i < N; i++) {
	// 	cities[i].x = static_cast<double>(rand()) / RAND_MAX;
	// 	cities[i].y = static_cast<double>(rand()) / RAND_MAX;
	// }
}

double distance(const City& city1, const City& city2){
    return sqrt((city2.x - city1.x) * (city2.x - city1.x) + (city2.y - city1.y) * (city2.y - city1.y));
}

double coordsDistance(const vector<int>& parent, const vector<City>& cities){
    double len = 0;

    for(int i = 0; i < parent.size() - 1; i++){
        len += distance(cities[parent[i]], cities[parent[i + 1]]);
    }

    len += distance(cities[parent.back()], cities[parent.front()]);

    return len;
}

struct Comparator{
    const vector<City>& cities;

    Comparator(const vector<City>& c) : cities(c) {}

    bool operator()(const vector<int>& fParent, const vector<int>& sParent) const {
        return coordsDistance(fParent, cities) < coordsDistance(sParent, cities);
    }
};

vector<int> selectParents(const vector<vector<int>>& population, const vector<City>& cities) {
    vector<int> fParent = population[rand() % population.size()];
    vector<int> sParent = population[rand() % population.size()];

    if(coordsDistance(fParent, cities) < coordsDistance(sParent, cities)){
        return fParent;
    }

    return sParent;
}

vector<int> reproduction(const vector<int>& fParent, const vector<int>& sParent){
    vector<int> child(fParent.size(), -1);
    int crossoverStart = rand() % fParent.size();
    int crossoverEnd = rand() % (fParent.size() - crossoverStart) + crossoverStart;

    for (int i = crossoverStart; i <= crossoverEnd; ++i) {
        child[i] = fParent[i];
    }

    int currentIndex = 0;
    for (int i = 0; i < sParent.size(); i++) {
        if (child[i] == -1) {
            while (find(child.begin(), child.end(), sParent[currentIndex]) != child.end()) {
                currentIndex = (currentIndex + 1) % sParent.size();
            }
            child[i] = sParent[currentIndex];
            currentIndex = (currentIndex + 1) % sParent.size();
        }
    }

    return child;
}

void mutate(vector<int>& child){
    int x = rand() % child.size();
    int y = rand() % child.size();

    swap(child[x], child[y]);
}

void printPath(const vector<int>& path, const vector<City>& cities) {
    for (int i : path) {
        cout << cities[i].name << " ";
    }
}

vector<int> buildNextGeneration(const vector<vector<int>>& children, const vector<vector<int>>& population, const vector<City>& cities){
    vector<vector<int>> combinedPopulation = population;
    combinedPopulation.insert(combinedPopulation.end(), children.begin(), children.end());

    sort(combinedPopulation.begin(), combinedPopulation.end(), Comparator(cities));

    vector<int> bestGene = combinedPopulation.front();

    return bestGene;
}

int main(){
    int N;
    cin >> N;
     
    srand(time(NULL));

    vector<City> cities(N);
    generateRandomCoordinates(cities);

    int populationSize = 100;
    int iter = 5000;

    vector<vector<int>> population(populationSize, vector<int>(N));
    for (int i = 0; i < populationSize; i++) {
        population[i] = init(N);
    }

clock_t start = clock();

    int t = 0;
    while (t < iter) {
        vector<vector<int>> children;

        for (int i = 0; i < populationSize; i++) {
            vector<int> fParent = selectParents(population, cities);
            vector<int> sParent = selectParents(population, cities);

            vector<int> child = reproduction(fParent, sParent);

            if (rand() % 2 == 0) 
                mutate(child);

            children.push_back(child);
        }

        population.front() = buildNextGeneration(children, population, cities);

        if(t % 500 == 0){
            cout << "Generation " << t << ": Current best distance: " << coordsDistance(population.front(), cities) << endl;
        }
        t++;
    }

    clock_t end = clock();
    double duration = (end - start) / (double) CLOCKS_PER_SEC;
    printPath(population.front(), cities);

    printf("Execution time: %f secs.\n", duration);
    return 0;
}