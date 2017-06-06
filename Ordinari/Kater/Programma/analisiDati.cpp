#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<iomanip>
#include<cmath>

#define  USE_MATH_DEFINES

using namespace std;

double compatibility(double a, double b, double siga, double sigb);

struct point{

    double x;
    double y;

};

int main(){

    cout << "\tANALISI DATI - Pendolo di Kater per g" << endl;
    cout << endl;
    vector<point> tmp;
    point tmpVal;
    vector<point> rettaO;
    vector<point> rettaP;
    vector<point> rettaOP;
    vector<point> misureT;
    double Tpes;
    double sigTpes;
    double sig_T = 0.0001; //s
    double sig_T_OP = 0.0001; //s
    double sig_x = 0.001; //m
    double xInt = 0.212057; // intersezione tra la retta deltaT e l'asse
    double sig_xInt;
    double gS; //g sperimentale
    double sig_gS;
    double gAtt = 9.806; //g atteso [m/s^2]
    double sig_gAtt = 0.001;
    double l = 0.9945;
    double sig_l = 0.0002;

    //Caricamento dati da file
    string url = "input/rettaO.txt";
    ifstream iFile(url.c_str());
    if (!iFile) {

        cout << "Errore durante l'apertura del file" << endl;
        return -1;

    }
    while (!iFile.eof()) {

        iFile >> tmpVal.x;
        iFile >> tmpVal.y;
        tmpVal.x /= 100;
        rettaO.push_back(tmpVal);

    }
    rettaO.pop_back();

    url = "input/rettaP.txt";
    ifstream iFile1(url.c_str());
    if (!iFile1) {

        cout << "Errore durante l'apertura del file" << endl;
        return -1;

    }
    while (!iFile1.eof()) {

        iFile1 >> tmpVal.x;
        tmpVal.x /= 100;
        iFile1 >> tmpVal.y;
        rettaP.push_back(tmpVal);

    }
    rettaP.pop_back();

    //calcolo retta OP

    for(int i=0; i<rettaP.size(); ++i){

        tmpVal.x = rettaO[i].x;
        tmpVal.y = rettaO[i].y - rettaP[i].y;
        rettaOP.push_back(tmpVal);
    }

    url = "output/graph/rettaOP.txt";
    ofstream oFile(url.c_str());
    if (!oFile) {

        cout << "Errore durante l'apertura del file" << endl;
        return -1;

    }

    url = "input/misureRipetuteT.txt";
    ifstream iFile2(url.c_str());
    if (!iFile2) {

        cout << "Errore durante l'apertura del file" << endl;
        return -1;

    }
    while (!iFile2.eof()) {

        iFile2 >> tmpVal.x;
        iFile2 >> tmpVal.y;
        misureT.push_back(tmpVal);

    }

    //calcolo di T
    double sum=0, sum1=0;
    for(int i=0; i<misureT.size(); ++i){

        sum += misureT[i].x/(sig_T*sig_T);
        sum1 += 1./(sig_T*sig_T);
    }
    Tpes = sum/sum1;
    sigTpes = sqrt(1./sum1);

    //calcolo di g e sig e compatibilità
    gS = 4.*M_PI*M_PI*(l/(Tpes*Tpes));
    sig_gS = sqrt(pow(4.*M_PI*M_PI*sig_l/(Tpes*Tpes),2)+pow(4.*M_PI*M_PI*l*sigTpes/(3.*Tpes*Tpes*Tpes),2));


    //for(int i=0; i<misureT.size(); ++i)cout << misureT[i].x << endl;

    //stampa dati
    oFile << "#rettaOP = rettaO - rettaP, sig_x = 0.001 m, sig_T = 0.0001 s" << endl;
    oFile << "#x [m]\tT [s]" << endl;
    for(int i=0; i<rettaOP.size(); ++i){

        oFile << fixed << setprecision(3);
        oFile << rettaOP[i].x << "\t";
        oFile << fixed << setprecision(4);
        oFile << rettaOP[i].y << "\t" << sig_T_OP << endl;
    }
    oFile.close();

    cout << "Retta OP" << endl;
    cout << "x [m]\tT [s]" << endl;
    for(int i=0; i<rettaOP.size(); ++i) {

        cout << fixed << setprecision(3);
        cout << rettaOP[i].x << "\t";
        cout << fixed << setprecision(4);
        cout << rettaOP[i].y << endl;
    }
    cout << endl;

    cout << "Retta O" << endl;
    cout << "x [m]\tT [s]" << endl;
    for(int i=0; i<rettaO.size(); ++i) {

        cout << fixed << setprecision(3);
        cout << rettaOP[i].x << "\t";
        cout << fixed << setprecision(4);
        cout << rettaOP[i].y << endl;
    }
    cout << endl;

    cout << endl << "Retta P" << endl;
    cout << "x [m]\tT [s]" << endl;
    for(int i=0; i<rettaP.size(); ++i) {

        cout << fixed << setprecision(3);
        cout << rettaOP[i].x << "\t";
        cout << fixed << setprecision(4);
        cout << rettaOP[i].y << endl;
    }
    cout << endl;
    cout << "DATI CALCOLATI" << endl;
    cout << fixed << setprecision(3);
    cout << "Errore su x: " << sig_x << " s" << endl;
    cout << fixed << setprecision(4);
    cout << "Errore su T: " << sig_T << " s" << endl;
    cout << "Errore su T di OP: " << sig_T_OP << " s" << endl;
    cout << "Periodo <T>: (" << Tpes << " +- " << sigTpes << ")s"<< endl;
    cout << "Punto di intersezione DT e asse x: " << xInt << " m"<< endl;
    cout << "l: (" << l << " +- " << sig_l << ") m"<< endl;
    cout << "Stima di g: (" << gS << " +- " << sig_gS << ") m/s^2"<< endl;
    cout << "Valore atteso di g: (" << gAtt << " +- " << sig_gAtt << ") m/s^2"<< endl;
    cout << fixed << setprecision(2);
    cout << "lamda(g_A,g_S) = " << compatibility(gAtt,gS,sig_gAtt,sig_gS) << endl;
    return 0;

}

double compatibility(double a, double b, double siga, double sigb)
{
	double lambda;
	lambda = abs(a-b) / sqrt((siga*siga)+(sigb*sigb));
	return lambda;
}
