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

int main(){
    vector<double> array;
    int i = 0;
    double value;
    cout << "Give me another number or press CTRL-D: ";
    while(cin >> value){
        cout << "Give me another number or press CTRL-D: ";
        array.push_back(value);
        i++;
    }
    double sum;
    double number = i;
    for(int ind = i; ind>= 0; ind--){
        sum += array[ind];
    }
    sum = sum / number;
    cout << "\nThe average of these " << number << " numbers is: " << sum << endl;
    return 0;
}
