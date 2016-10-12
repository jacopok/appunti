/*
 * COB.c
 * 
 * Copyright 2016 jacopo <jacopo@vikingmetal>
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


#include <stdio.h>
// #include <iostream.h>
// using namespace std;

int main(){

int cob(int k, int b) 
{
	while(k != 0){
        int r;
        r=k%b;
        k = (k-r)/b;
        printf("%d", r); 
    }
	return 0;
}

cob(5, 4);
getchar();
return 0;
}
