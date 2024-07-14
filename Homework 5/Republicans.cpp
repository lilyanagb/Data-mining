#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <math.h>

using namespace std;

const int REPUBLICANS = 11; //length of republicans, starting point from where to read the data
const int DEMOCRATS = 9;

vector<string> dataset; //input
int democrats[3][16]; //y,n,? for all 16 features
int republicans[3][16];
double democratsNum = 0;
double republicansNum = 0;
int testSize;

// void print() {
//     for (int i = 0; i < 3; i++) {
//         for (int j = 0; j < 16; j++) {
//             cout << "D " << i << " " << j << " " << democrats[i][j] << endl;
//             cout << "R " << i << " " << j << " " << republicans[i][j] << endl;
//         }
//     }
// }

//ОБУЧАВАМЕ МОДЕЛА
void train(string line, int difference) {
    bool isRepublican = line[0] == 'r';

    isRepublican ? republicansNum += difference : democratsNum += difference;

    int initPos = isRepublican ? REPUBLICANS : DEMOCRATS;
    int counter = 0;

    for (int i = initPos; i < line.size(); i += 2) { //+2 -> ,
    //add +1 to the features which are y,n or ?
       switch (line[i]) {
            case 'y':
               isRepublican ? republicans[0][counter] += difference : democrats[0][counter] += difference;
               break;
            case 'n':
               isRepublican ? republicans[1][counter] += difference : democrats[1][counter] += difference;
               break;
            case '?':
               isRepublican ? republicans[2][counter] += difference : democrats[2][counter] += difference;
               break;
       }
       counter++;
    }
}

int predict(string line) {
    bool isRepublican = line[0] == 'r';
    int initPos = isRepublican ? REPUBLICANS : DEMOCRATS;
    double republicanLog = 0;
    double democratLog = 0;
    int index = 0;

    for (int i = initPos; i < line.size(); i += 2) {
        switch (line[i]) {
            case 'y':
               republicanLog += log((republicans[0][index] + 1) / (republicansNum + 2));
               democratLog += log((democrats[0][index] + 1) / (democratsNum + 2));
               break;
            case 'n':
               republicanLog += log((republicans[1][index] + 1) / (republicansNum + 2));
               democratLog += log((democrats[1][index] + 1) / (democratsNum + 2));
               break;
            case '?':
               republicanLog += log((republicans[2][index] + 1) / (republicansNum + 2));
               democratLog += log((democrats[2][index] + 1) / (democratsNum + 2));
               break;
            }
        index++;
    }

    //P(xi|Ck).P(Ck) -- P(Ck)
    republicanLog += log((republicansNum + 1) / (republicansNum + democratsNum + 2));
    democratLog += log((democratsNum + 1) / (republicansNum + democratsNum + 2));

    return isRepublican ? (republicanLog >= democratLog) : (republicanLog <= democratLog);
}

double calculate(int testinit) {
    string currLine;
    double rightPredictions = 0;

    for (int i = 0; i < testSize; i++) {
        rightPredictions += predict(dataset[testinit + i]);
    }

    return rightPredictions * 100 / testSize;
}

int main() {
    ifstream filein("house-votes-84.data", ios::in);
    string line;

    while (getline(filein, line)) {
        dataset.push_back(line);
        train(line, 1);
    }

    filein.close();

    //10-fold cross-validation - [testing set] [training set]
    testSize = dataset.size() / 10;
    int testinit;
    double sumperc = 0;
    double accuracy;

    for (int j = 0; j < 10; j++) {
        testinit = j * testSize;

        for (int i = testinit; i < testinit + testSize; i++) {
            if (testinit != 0) {
                train(dataset[i - testSize], 1);
            }

            train(dataset[i], -1); 
        }

        accuracy = calculate(testinit);
        cout << "Test " << j + 1 << ": " << accuracy << "%" << endl;
        sumperc += accuracy;
    }

    cout << "Average: " << sumperc/10 << "%" << endl;
}
