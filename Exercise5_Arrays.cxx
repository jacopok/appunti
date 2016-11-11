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
#include <string>
using namespace std;

struct student {
    string name;
    string surname;
    long int id;
    int * grades = new int[i];
    double average;
};

int main(){
    student first;
    cout << "Insert the name of the student: ";
    cin >> first.name;
    cout << "Insert the surname of the student: ";
    cin >> first.surname;
    cout << "Insert the id number of the student: ";
    cin >> first.id;
    int i;
    cout << "How many exams did you take? ";
    cin >> i;
    int max, min, sum;
    for(int k=0; k!=i; k++){
        cout << "Insert the grade of exam " << k + 1;
        cin >> first.grades[k];
        if(first.grades[k]>max){
            max = first.grades[k];
        }
        if(first.grades[k]<min){
            min = first.grades[k];
        sum += first.grades[k];
        }
    }
    double average = (double) sum / i;
    cout << "The average is " << average << ", the minimum is " << min << ", the maximum is " << max << endl;
    return 0;
}
