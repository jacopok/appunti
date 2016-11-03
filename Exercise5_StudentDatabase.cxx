/*
 * Exercise3.cxx
 * 
 * Copyright 2016 jacopot <jacopot@dip075.studenti.math.unipd.it>
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


#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

struct student {
    string name;
    string surname;
    long int id;
    vector<int> grades;
    double average;
};

int main(){
    student first;
    int i = 0, grade;
    cout << "Give me a grade or press CTRL-D: ";
    while(cin >> grade){
        cout << "Give me a grade or press CTRL-D: ";
        first.grades.push_back(grade);
        i++;
    }
    double sum = 0;
    for(int k = 0; k<i; k++){
        sum += first.grades[k];
        }
    double average = sum / i;
    cout << "The average is " << average << endl;
return 0;
}
