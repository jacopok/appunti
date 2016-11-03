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

int scale(double temperature, double temperatures[3]);

int main(){
/*
    cout << "First part: circles.\nGive me the radius:" << endl;
    double radius;
    cin >> radius;
    double circ = radius * 2. * M_PI;
    double area = ( radius * radius ) * M_PI;
    cout << "Circumference: " << circ << ", area: " << area << endl;
    
    cout << "Now, for the Body Mass Weight:\nInsert your mass [kg]: ";
    double mass;
    cin >> mass;
    cout << "Insert your height [m]: " ;
    double height;
    cin >> height;
    double bmw = mass / ( height * height );
    cout << "Your BMW is: " << bmw << endl;
*/

    cout << "Let us treat temperatures. Give me a temperature (I will ask which scale you used later): ";
    double temperature;
    cin >> temperature;
    double temperatures [3];
    int type_index = scale(temperature, temperatures);
    
    cout << "Your temperature in the other scales is:" << endl;
    char letters [3] = {'K', 'C', 'F'};
    for (int i = 0; i < 3; i++){
        if (i != type_index){
            cout << temperatures[i] << letters [i] << endl;
        }
    }
    return 0;
}

int scale(double temperature, double temperatures[3]){
    cout << "Which temperature scale did you use? Insert 'K', 'C' or 'F'. " << endl; 
    char temp_type;
    cin >> temp_type;
    int type_index;
    switch (temp_type){
        case 'K':
            temperatures[1] = temperature - 273.15;
            temperatures[2] = 32. + ( 9. / 5. ) * temperatures[1];
            temperatures[0] = temperature;
            type_index = 0;
        break;
        case 'C':
            temperatures[1] = temperature;
            temperatures[0] = temperature + 273.15;
            temperatures[2] = 32. + ( 9. / 5. ) * temperatures[1];
            type_index = 1;
        break;
        case 'F':
            temperatures[1] = (5. / 9. ) * (temperature - 32);
            temperatures[0] = temperatures[1] + 273.15;
            temperatures[2] = temperature;
            type_index = 2;
        break;
        default:
            cout << "That is not a temperature scale!" << endl;
            scale(temperature, temperatures);
        break;
    }
    return type_index;
}
