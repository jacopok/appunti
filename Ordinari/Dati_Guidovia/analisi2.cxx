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
 
// Significato sigle dei dati:
// s = slitta scarica senza spessore
// n = slitta carica senza spessore
// c = slitta scarica con spessore sottile
// g = slitta scarica con spessore grosso

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
    int position2;
    char charge;
};

struct singleposition {
    double average;
    double standarddev;
};

struct couple {
    double x;
    double y;
};

double stdev (vector<double> data); // Calculates the standard deviation of a vector
double av (vector<double> data); // Calculates the average of a vector
vector <double> finddata (int position, char charge); // Filters through the data file to find the rows with the selected property
vector <singleposition> analyse (char charge); // Calculates the standard deviation and average of all the measurements of type charge
couple regression (vector <couple> data); // Calculates the pair (a, b) for the least-squares function y = a + b x
void rejectwarning (int position, char charge); // Prints a warning if a datum is found over the 3sigma boundary

vector <datum> alldata;

int main() {
//    cout << "Insert name of the file to be analyzed:" << endl;
    string filename = "datiraw2.txt";
//    cin >> filename;
    ifstream file;
    file.open(filename);
    while (!file.eof()){
        datum singlet;
        file >> singlet.value;
        file >> singlet.position;
        file >> singlet.position2;
        file >> singlet.charge;
        alldata.push_back(singlet);
        }
//   for (unsigned int i = 0; i<alldata.size(); ++i)
//        cout << alldata[i].value << " " <<  alldata[i].position << " " <<  alldata[i].charge << endl;
//   Check whether all the data is read properly
    char potentials[4] = {'s', 'n',  'c', 'g'};
    for (auto i : potentials){
        vector <singleposition> t = analyse(i);
        for (unsigned int i = 0; i<t.size(); i++)
            cout << t[i].average << "   " << t[i].standarddev << endl;
        vector <couple> to_regress;
        for (unsigned int i = 0; i < 6; i++){
            couple h;
            h.x = (10 *i) + 50;
            h.y = t[i].average;
            to_regress.push_back(h);
        }
        couple s = regression(to_regress);
        cout << "Intercept: " << s.x << ", angular coefficient: " << s.y << endl;
        cout << endl;
    }
    int positions[] = {40, 50, 60, 70, 80, 90};
    for (auto i : potentials)
        for (auto k : positions)
            rejectwarning(k, i);
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

vector <double> finddata (int position, char charge){
    vector <double> newdata;
    for (unsigned i = 0; i < alldata.size(); ++i){
        if (alldata[i].position == position && alldata[i].charge == charge)
            newdata.push_back(alldata[i].value);
    }
    return newdata;
};

vector <singleposition> analyse (char charge){
    vector <singleposition> set;
    for (int i = 40; i < 91; i+=10){
        singleposition pos;
        vector <double> temp = finddata(i, charge);
        pos.average = av(temp);
        pos.standarddev = stdev(temp);
        set.push_back(pos);
    }
    return set;
};

couple regression (vector <couple> data){
    vector <double> x;
    vector <double> y;
    for (auto k : data) // Generate vectors for both categories of data 
        x.push_back(k.x); 
    for (auto k : data)
        y.push_back(k.y);
    vector <double> xnorm;
    vector <double> ynorm;
    double av_x = av(x);
    double av_y = av(y);
    double sd_x = stdev(x);
    for (double k : x) // Normalize both vectors
        xnorm.push_back(k - av_x);
    for (double k : y)
        ynorm.push_back(k - av_y);
    vector <double> xy; // The term-by-term product of the two normalized vectors
    for (unsigned int i = 0; i < xnorm.size(); i++)
        xy.push_back(xnorm[i] * ynorm[i]);
    double sum_xy = 0;
    for (auto k : xy)
        sum_xy += k;
    couple res;
    res.y = sum_xy / (sd_x * sd_x); // Coefficient of the least-sqares line
    res.x = av_y - (res.y * av_x); // Intercept
    return res;
};

void rejectwarning (int position, char charge){
    vector <double> newdata = finddata (position, charge);
    double dev = stdev(newdata);
    double aver = av(newdata);
    for (double x : newdata){
        if (x > (3 * dev + aver) || x < (aver - 3 * dev))
            cout << "The value " << x << " is over the 3 sigma boundary." << endl; 
    }
};
