#include <iostream>
using std::cin;
using std::cout;
using std::endl;

int main(){
    int n1, n2, sumn, difn;
    cout << "Give me two integers." << endl;
    cin >> n1 >> n2;
    sumn = n1 + n2;
    if (n1 >= n2){
        difn = n1-n2;
    }
    else {
        difn = n2-n1;
    }
    cout << "Their sum and difference are respectively: " << sumn << " and " << difn << endl;
    float area;
    area = n1 * n2 / 2.;
    cout << "If those two numbers were the base and height of a triangle, its area would be " << area << ". Now give me another integer." << endl;
    int n3;
    cin >> n3;
    int vol = 1, area2 = 1;
    int vector[3] = {n1, n2, n3};
    for (int i=2; i>=0; i--){
        vol = vol * vector[i];
    }
    // area2 = 2 * (n1 * n2 + n2 * n3 + n1 * n3);
    for (int i=2; i>=0; i--) {
        for (int j = 2; j>=0 ; j--) {
            if (i != j) {
                area2 = area2 + 2* ( vector[i] * vector[j]);
            }
        }
    }
    cout << "The volume and total surface of a parallelogram \nwith your numbers as its sides are respectively " << vol << " and " << area2 << "\nNow what is your number?" << endl;
/*    int folly;
    cin >> folly;
    if (folly == 42) {
        while (true) {
                cout << "The devil is coming for you... ";
        }
    } */
    return 0;
}
