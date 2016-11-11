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
using namespace std;

int main(){
    int i;
    cout << "How many terms do you want? ";
    cin >> i;
    double term = pow(1. + ( 1. / double(i) ), i);
    cout << "The " << i << "-th term of the sequence is " << term << endl;
    double error = (M_E - term) / M_E;
    cout << "The error with respect to " << M_E << " is " << error*100 << "%" << endl;
    return 0;
}
