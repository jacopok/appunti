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
#include <iomanip>

struct datum {
    double value;
    int position;
    int primes;
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

double stdev (vector<double> data);
double av (vector<double> data);
vector <double> finddata (int position, int primes, char charge);
vector <singleposition> analyse (int primes, char charge);
void rejectwarning (int position, int primes, char charge);
couple regression (vector <couple> data); // Calculates the pair (a, b) for the least-squares function y = a + b x
vector <singleposition> getspeed (vector <singleposition> times); // Calculates the speed between two points
void printout (vector <singleposition> times, vector <singleposition> speeds);
couple regress_singledataset (vector <singleposition> times_set);
couple regression_error (vector <couple> variables, couple parameters);

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
   for (unsigned int i = 0; i<alldata.size(); ++i)
        cout << "40.0" << " & " << fixed << setprecision(1) << double(alldata[i].position)  << " &  " << setprecision(4) << alldata[i].value << " \\\\" << endl;
//   Check whether all the data is read properly
    vector <singleposition> set15 = analyse(15, 's');
    vector <singleposition> set30 = analyse(30, 's');
    vector <singleposition> set45 = analyse(45, 's');
    vector <singleposition> set45c = analyse(45, 'c');
    for (int pos = 50; pos <= 100; pos += 10){
        rejectwarning(pos, 15, 's');
        rejectwarning(pos, 30, 's');
        rejectwarning(pos, 45, 's');
        rejectwarning(pos, 45, 'c');
    }
    vector <singleposition> speed15 = getspeed(set15);
    vector <singleposition> speed30 = getspeed(set30);
    vector <singleposition> speed45 = getspeed(set45);
    vector <singleposition> speed45c = getspeed(set45c);
    printout(set15, speed15);
    printout(set30, speed30);
    printout(set45, speed45);
    printout(set45c, speed45c);
    couple coeff15 = regress_singledataset(set15);
    couple coeff30 = regress_singledataset(set30);
    couple coeff45 = regress_singledataset(set45);
    couple coeff45c = regress_singledataset(set45c);
    double g15 = coeff15.y / sin((15.0 * M_PI / 10800.0));
    double g30 = coeff30.y / sin((30.0 * M_PI / 10800.0));
    double g45 = coeff45.y / sin((45.0 * M_PI / 10800.0));
    double g45c = coeff45c.y / sin((45.0 * M_PI / 10800.0));
    cout << coeff15.x << " & " << coeff15.y << " & " << g15 << " \\\\" << endl;
    cout << coeff30.x << " & " << coeff30.y << " & " << g30 << " \\\\" << endl;
    cout << coeff45.x << " & " << coeff45.y << " & " << g45 << " \\\\" << endl;
    cout << coeff45c.x << " & " << coeff45c.y << " & " << g45c << " \\\\" << endl;
    vector <couple> var15;
    for (unsigned int k = 0; k < set15.size(); k++){
        couple temp;
        temp.x = set15[k].average;
        temp.y = speed15[k].average;
        var15.push_back(temp);
    }
    couple error15 = regression_error(var15, coeff15);
    cout << error15.x << " " << error15.y << endl;

    vector <couple> var30;
    for (unsigned int k = 0; k < set30.size(); k++){
        couple temp;
        temp.x = set30[k].average;
        temp.y = speed30[k].average;
        var30.push_back(temp);
    }
    couple error30 = regression_error(var30, coeff30);
    cout << error30.x << " " << error30.y << endl;

    vector <couple> var45;
    for (unsigned int k = 0; k < set45.size(); k++){
        couple temp;
        temp.x = set45[k].average;
        temp.y = speed45[k].average;
        var45.push_back(temp);
    }
    couple error45 = regression_error(var45, coeff45);
    cout << error45.x << " " << error45.y << endl;

    vector <couple> var45c;
    for (unsigned int k = 0; k < set45c.size(); k++){
        couple temp;
        temp.x = set45c[k].average;
        temp.y = speed45c[k].average;
        var45c.push_back(temp);
    }
    couple error45c = regression_error(var45c, coeff45c);
    cout << error45c.x << " " << error45c.y << endl;
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
        pos.standarddev = stdev(temp) / sqrt(temp.size());
        pos.dist = i;
        cout << temp.size() << endl;
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

couple regression (vector <couple> data){
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

vector <singleposition> getspeed (vector <singleposition> times){
    singleposition temp;
    vector <singleposition> res;
    for (unsigned int i = 0; i<times.size(); i++){
        temp.dist = times[i].dist - 5;
        double sumt = 0;
        if (i>0)
            sumt = times[i-1].average;
        temp.average = 10 / (times[i].average - sumt);
        if (i>0)
            temp.standarddev =  sqrt(0.2 / ((times[i].average - sumt)*(times[i].average - sumt)) + 100 * (times[i].standarddev * times[i].standarddev + times[i-1].standarddev * times[i-1].standarddev) / pow((times[i].average - sumt), 4) );
        else
            temp.standarddev =  sqrt(0.2 / ((times[i].average - sumt)*(times[i].average - sumt)) + 100 * (times[i].standarddev * times[i].standarddev) / pow((times[i].average - sumt), 4) );
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
