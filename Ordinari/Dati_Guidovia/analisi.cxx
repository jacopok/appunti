/*
 * analisi.cxx
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

using namespace std;

#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>

struct datum {
    double value;
    int position;
    int primes;
    char charge;
};

struct singleposition {
    double average;
    double standarddev;
};

double stdev (vector<double> data);
double av (vector<double> data);
vector <double> finddata (int position, int primes, char charge);
vector <singleposition> analyse (int primes, char charge);
void rejectwarning (int position, int primes, char charge);

vector <datum> alldata;

int main()
{
//    cout << "Insert name of the file to be analyzed:" << endl;
    string filename = "datiraw.txt";
//    cin >> filename;
    ifstream file;
    file.open(filename);
    while (!file.eof()){
        datum singlet;
        file >> singlet.value;
        file >> singlet.position;
        file >> singlet.primes;
        file >> singlet.charge;
        alldata.push_back(singlet);
        }
//   for (unsigned int i = 0; i<alldata.size(); ++i)
//        cout << alldata[i].value << " " <<  alldata[i].position << " " <<  alldata[i].primes << " " <<  alldata[i].charge << endl;
//   Check whether all the data is read properly
    vector <singleposition> set15 = analyse(15, 's');
    vector <singleposition> set30 = analyse(30, 's');
    vector <singleposition> set45 = analyse(45, 's');
    vector <singleposition> set45c = analyse(45, 'c');
    for (auto k: set15)
        cout << "Average: " << k.average << ", Stdev: " << k.standarddev << endl;
    for (int pos = 50; pos <= 100; pos += 10){
        rejectwarning(pos, 15, 's');
        rejectwarning(pos, 30, 's');
        rejectwarning(pos, 45, 's');
        rejectwarning(pos, 45, 'c');
    }
    vector <double> stuff = {1, 2, 3, 4, 5};
    cout << "Stuff: " << stdev(stuff);
    return 0;
}

double av (vector<double> data){
    double sum = 0;
    for (double x : data)
        sum += x;
    return (sum / data.size());
};

double stdev (vector<double> data){
    double sum = 0;
    double average = av(data);
    for (double x : data){
        sum += (x - average) * (x - average);
    };
    return sqrt(sum / (data.size() -1));
};

vector <double> finddata (int position, int primes, char charge){
    vector <double> newdata;
    for (unsigned i = 0; i < alldata.size(); ++i){
        if (alldata[i].position == position && alldata[i].primes == primes && alldata[i].charge == charge)
            newdata.push_back(alldata[i].value);
    }
    return newdata;
};

vector <singleposition> analyse (int primes, char charge){
    vector <singleposition> set;
    for (int i = 50; i < 101; i+=10){
        singleposition pos;
        vector <double> temp = finddata(i, primes, charge);
        pos.average = av(temp);
        pos.standarddev = stdev(temp);
        set.push_back(pos);
    }
    return set;
};

void rejectwarning (int position, int primes, char charge){
    vector <double> newdata = finddata (position, primes, charge);
    double dev = stdev(newdata);
    double aver = av(newdata);
    for (double x : newdata){
        if (x > (3 * dev + aver) || x < (aver - 3 * dev))
            cout << "The value " << x << " is over the 3 sigma boundary." << endl; 
    }
};
