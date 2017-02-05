/*
 * testing.cxx
 * 
 * Copyright 2017 jacopo <jacopo@vikingmetal>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

/*

#include <iostream>
#include <vector>

using namespace std;

struct pippo{
    int a, b, c;
    float d;
    bool e, f;
    string g;
    vector<int> h; 
    
    pippo(int n, int val){
        for(int i=0; i<n; i++)
        h.push_back(val);
    }
};

double media (vector<int> dati);

int main(){
    pippo a(20, 30);
    vector<int> dati {2, 3, 5};
    double media1 = media (dati);
    cout << media1 << endl;
}

double media (vector<int> dati){
    double sum = 0;
    for (int a : dati)
        sum += a;
    double size = dati.size();
    double average = sum / size;
    return average;
};


*/

#include <iostream>
#include <vector>

using namespace std;

int main (){
    cout << "Quanti numeri interi vuoi inserire?" << endl;
    vector<int> dati;
    int counter = 1;
    int numero;
    while(cin >> numero){
        cout << "Inserire il numero " << counter << " : ";
        ++counter;
        dati.push_back(numero);
    }
    vector < vector < int > > freq {{0, 0}};
    double somma = 0;
    for (int x : dati){
        somma += x;
        bool check = false;
        for (auto y : freq){
            if (y[0] == x){
                ++y[1];
                check = true;
            }
            if (check == false){
                vector<int> temp {x, 1};
                freq.push_back(temp);
            }
        }
    }
    double media = somma / dati.size();
    cout << "\nLa media vale " << media << "." << endl;
    for (unsigned int i = 0; i < freq.size(); i++){
        cout << "Il valore " << freq[i][0] << " ha frequenza " << freq[i][1] << endl;
    }
    
//    cout << "La moda vale " << moda << "." << endl;
}
