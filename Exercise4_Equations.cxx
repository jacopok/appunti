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
    double a, b, c;
    cout << "Give me the three coefficients a, b and c of a second degree polynomial." << endl;
    cin >> a >> b >> c;
    double delta = pow(b, 2) - 4. * a * c;
    if (delta > 0){
        double x1 = ( - b + sqrt(delta))/( 2. * a);
        double x2 = ( - b - sqrt(delta))/( 2. * a);
        cout << "Your solutions are: " << x1 << " and " << x2 << endl;
        }
    else if(delta == 0){
        double x = ( -b)/(2. * a);
        cout << "Your solutions are both: " << x << endl;
    }
    else if(delta < 0){
        cout << "There are no real solutions!" << endl;
    }
    else{
        cout << "What was that?" << endl;
    }
    return 0;
}
