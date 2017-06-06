/*
 * analisi2.cxx
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
#include <iomanip>

struct datum {
    double value;
    int position;
    int position2;
    char charge;
};

struct singleposition {
    double average;
    double standarddev;
    int dist;
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
vector <singleposition> getspeed (vector <singleposition> times); // Calculates the speed between two points
void printout (vector <singleposition> times, vector <singleposition> speeds);
couple regress_singledataset (vector <singleposition> times_set);
couple regression_error (vector <couple> variables, couple parameters);


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
    for (unsigned int i = 0; i<alldata.size(); ++i)
        cout << alldata[i].position << ".0 & " << alldata[i].position + 20 << ".0 &  " << alldata[i].value << " \\\\" << endl;
    //Check whether all the data is read properly
    char potentials[4] = {'s', 'n',  'c', 'g'};
    for (auto i : potentials){
        vector <singleposition> t = analyse(i);
        for (unsigned int i = 0; i<t.size(); i++)
            cout << t[i].average << "   " << t[i].standarddev << endl;
        vector <couple> to_regress;
        vector <singleposition> speeds = getspeed(t);
        for (unsigned int i = 0; i < 6; i++){
            couple h;
            h.x = (10 *i) + 50;
            h.y = speeds[i].average;
            to_regress.push_back(h);
            cout << " & " << (10*i)+40 << " & " << (10*i)+60  << " & " << fixed << setprecision(4) << t[i].average << " & " << setprecision(1) << speeds[i].average << " & " << setprecision(4) << t[i].standarddev << " & " << setprecision(1) << speeds[i].standarddev << " \\\\" << endl; 
            cout << (10 * i) + 50 << " " << speeds[i].average << " " << speeds[i].standarddev << endl;
        }
        couple s = regression(to_regress);
        cout << i << " & " << fixed << setprecision(3) << s.x << " & " << regression_error(to_regress, s).x << " & " << s.y << " & " << regression_error(to_regress, s).y << " \\\\" << endl;
    }
    int positions[] = {40, 50, 60, 70, 80, 90};

    for (auto i : potentials) // Warnings
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
        pos.standarddev = stdev(temp) / sqrt(temp.size());
        pos.dist = i;
        set.push_back(pos);
    }
    return set;
};

/* (auto k : data)
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
*/

couple regression (vector <couple> data){ //Sada's formula
    vector <double> x;
    vector <double> y;
    for (auto k : data) // Generate vectors for both categories of data 
        x.push_back(k.x); 
    for (auto k : data)
        y.push_back(k.y);
    double sumx2 = 0;
    double sumx = 0;
    double sumy = 0;
    double sumxy = 0;
    for (double k : x){
        sumx2 += (k * k);
        sumx += k;
    }
    for (double k : y)
        sumy += k;
    for (unsigned int i = 0; i < x.size(); i++)
        sumxy += (x[i] * y[i]);
    double delta = x.size() * sumx2 - (sumx * sumx);
    
    couple res;
    res.y = (x.size() * sumxy - sumx * sumy) / delta; // Coefficient of the least-sqares line
    res.x = (sumx2 * sumy - sumx * sumxy) / delta; // Intercept
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

vector <singleposition> getspeed (vector <singleposition> times){
    singleposition temp;
    vector <singleposition> res;
    for (unsigned int i = 0; i<times.size(); i++){
        temp.dist = times[i].dist +10;
        temp.average = 20 / (times[i].average);
        temp.standarddev =  sqrt((0.01)/(times[i].average * times[i].average) + (400 * times[i].standarddev * times[i].standarddev)/(pow(times[i].average, 4)) );
        res.push_back(temp);
    }
    return res;
};

void printout (vector <singleposition> times, vector <singleposition> speeds){
    if (times.size() != speeds.size())
        cout << "Different size vectors!" <<endl;
    else {
        for (unsigned int i = 0; i<times.size(); i++){
            cout << times[i].average << " " << speeds[i].average << " " << speeds[i].standarddev << endl;
        }
    }
};

couple regress_singledataset (vector <singleposition> times_set){
    vector <couple> line;
    vector <singleposition> speed_set = getspeed(times_set);
    couple temporary_couple;
    for (unsigned int i = 0; i < speed_set.size(); i++){
        temporary_couple.x = times_set[i].average;
        temporary_couple.y = speed_set[i].average;
        line.push_back(temporary_couple);
    }
    return regression(line);
};

couple regression_error (vector <couple> variables, couple parameters){ //Parameters: a, b in that order
    vector <double> x;
    vector <double> y;
    for (auto k : variables) // Generate vectors for both categories of data 
        x.push_back(k.x); 
    for (auto k : variables)
        y.push_back(k.y);
    vector <double> distance_from_line;
    for (unsigned int i = 0; i < x.size(); i++){
        double k = ((parameters.x + parameters.y * x[i]) -y[i]) * ((parameters.x + parameters.y * x[i]) -y[i]);
        distance_from_line.push_back(k);
    }
    double sum = 0;
    for (double k : distance_from_line)
        sum += k;
    double sigmay = sqrt(sum / (x.size() -2));
        double sumx2 = 0;
    double sumx = 0;
    double sumy = 0;
    double sumxy = 0;
    for (double k : x){
        sumx2 += (k * k);
        sumx += k;
    }
    for (double k : y)
        sumy += k;
    for (unsigned int i = 0; i < x.size(); i++)
        sumxy += (x[i] * y[i]);
    double delta = x.size() * sumx2 - (sumx * sumx);
    double sigmaa = sigmay * sqrt(sumx2 / delta);
    double sigmab = sigmay * sqrt(x.size() / delta);
    couple res;
    res.x = sigmaa;
    res.y = sigmab;
    return res;
};
