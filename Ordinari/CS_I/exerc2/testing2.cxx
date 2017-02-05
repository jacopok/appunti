#include <iostream>
//#include <cmath>
#include <math.h>
#include <vector>

using namespace std;

struct pilota{
    string nome;
    string cognome;
    string nat;
    int points;
    vector<int> par;
    bool champ;
    pilota(){
        for (int i = 0; i<20; i++)
            par.push_back(0);
    }
};

int main(){
    cout << cos (30);
}
