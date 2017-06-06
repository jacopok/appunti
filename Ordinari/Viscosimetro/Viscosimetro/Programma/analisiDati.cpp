#include<iostream>
#include<fstream>
#include<cmath>
#include<vector>
#include<string>
#include<sstream>
#include<iomanip>

#define  USE_MATH_DEFINES

using namespace std;

double compatibility(double a, double b, double siga, double sigb);

struct datat {

    vector<double> val;

};

struct point {

    double t; //tempo [s]
    double s; //spazio [s]
    double v; //velocita' [m/s]
    double sigv; // errore su v [m/s]

};

struct ball {

    vector<datat> dataSet; //set dei tempi misurati [s]
    double d; //diamentro pallina [m]
    double sigd; //errore sul diametro [m]
    vector<point> dati; //punti t vs s [s] [m]
    double sigt; //errore sui tempi [s]
    double sigtMedio; //errore sulla media dei t [s]
    double sigs; //errore sui tratti di percorrenza calcolato con la distribuzione triangolare [m]
    double vL; //velcoità limite
    double sigvL; //errore su vL
    double eta; //viscosita'
    double sigEta;
    double rho; //densità sfera
    double sig_rho;

};

int main (){

    string url = "input/";
	string nomeFile = "dati.txt";
	string tmp;
	int i=0;
	int iBall=0;
	double nVal = 10;
	double val;
	datat tmpSet;
	ball tmpData;
	point tmpPoint;
	double rhoF = 1032; //densità fluido +- 1 Kg/m^3
	double sigrhoF = 1;
	double g = 9.806; //acc di gravità +- 0.001 m/s^2
	double etaPes; //viscosità ricavata come media pesata delle viscosità calcolate per ogni pallina
	double sig_etaPes;
	double etaInt; //viscosità ricavata per interpolazione
	double sigEtaInt;
	double sigg=0.001;
	vector<ball> data;
	vector<double> tmpVal;
	cout << "--- ANALISI DATI Viscosimetro ---" << endl;
   	ifstream iFile((url + nomeFile).c_str());
    if (!iFile) {

        cout << "Errore durante l'apertura del file" << endl;
        return -1;

    }
    while (!iFile.eof()) {

        iFile >> tmp;
        if(tmp == "d"){

            if (iBall != 0) data.push_back(tmpData);
            tmpData.dataSet.clear();
            iFile >> tmpData.d;
            tmpData.d /= 0.03937;
            iFile >> tmp;
            ++iBall;

        }
        if(tmp == "s"){ //inizio set di dati

            tmpSet.val.clear();
            for (i=0; i < nVal; ++i){

                iFile >> val;
                tmpSet.val.push_back(val);

            }
            tmpData.dataSet.push_back(tmpSet);
        }
    }
    iFile.close();
    tmpData.dataSet.pop_back();
    data.push_back(tmpData);

    //prima elbarazione dati
    for (i=0; i<data.size(); ++i){

        data[i].sigd = 0.00001;
        data[i].d /= 1000.;
        data[i].sigvL = 0.001;
        data[i].sigt = 0.1; //cambaire0
        data[i].rho = 7870; // kg/m^3
        data[i].sig_rho = 5;
        data[i].sigs = (2*data[i].d)/sqrt(24); // dist triangolare fatta sulla tacca di lettura e due volte il diametro della pallina

        for(int i2=0; i2 < nVal ; ++i2){

            double sum1 = 0;
            for(int i1=0; i1<data[i].dataSet.size(); ++i1){

                sum1 += data[i].dataSet[i1].val[i2];


            }
            tmpPoint.t = sum1 / data[i].dataSet.size(); //calcolo media
            tmpPoint.s = 0.05 * (i2+1);
            tmpPoint.v = tmpPoint.s / tmpPoint.t;
            tmpPoint.sigv = sqrt(((data[i].sigs*data[i].sigs)/(tmpPoint.t*tmpPoint.t))+((tmpPoint.s*tmpPoint.s*data[i].sigt*data[i].sigt)/(pow(tmpPoint.t,4))));
            data[i].dati.push_back(tmpPoint);
            data[i].sigtMedio = data[i].sigt / sqrt(data[i].dataSet.size());
        }

    }



    //stampa su file dati per il grafico e tabella DsvsDt
    url = "output/graph/DsvsDt/";
    for(i=0; i<data.size(); ++i){

		ostringstream convert;
		convert << i + 1 ;
		nomeFile = "ball" + convert.str() + ".txt";
		ofstream oFile1((url + nomeFile).c_str());
        if (!oFile1) {

            cout << "Errore durante l'apertura del file" << endl;
            return -1;

        }
        oFile1 << fixed << setprecision(3);
        oFile1 << "# Ds [m]\tDt medio [s]" << endl;
        for(int i1=0; i1<data[i].dati.size(); ++i1){

            oFile1 << data[i].dati[i1].s << "\t\t" << data[i].dati[i1].t << "\t\t" << data[i].sigtMedio << endl;

        }
    oFile1.close();
    }

    //crea grafici
    url = "output/gnuPlotCmd.txt";
    ofstream oFile2(url.c_str());
    if (!oFile2) {

        cout << "Errore durante l'apertura del file" << endl;
        return -1;

    }
    oFile2 << "set terminal png" << endl << "set output \"./graph/DsvsDt/graph" << ".png\"" << endl;
    oFile2 << "set xlabel \"Lunghezza percorsa [m]\"" << endl;
    oFile2 << "set ylabel \"Tempo trascorso [s]\"" << endl;
    oFile2 << "set key at 0.175, 290" << endl;

    oFile2 << "plot ";
    for(i=0; i<data.size(); ++i){

		oFile2 << "\"./graph/DsvsDt/ball" << i+1 << ".txt\" title \"pallina " << i+1 <<"\",";


    }
    oFile2.close();

    url = "output/table/DsvsDt.txt";
    ofstream oFile3(url.c_str());
    if (!oFile3) {

        cout << "Errore durante l'apertura del file" << endl;
        return -1;

    }
    oFile3 << fixed << setprecision(3);
    oFile3 << "%in ordine crescente di diametro" << endl;
    oFile3 << "%Ds[m]\tDt[s]" << endl;
    for(i=0; i<data.size(); ++i){

        oFile3 << endl;
        for(int i1=0; i1<data[i].dati.size(); ++i1) oFile3 << data[i].dati[i1].s << "\t& " << data[i].dati[i1].t << " \\\\" << endl;

    }
    oFile3.close();

    //stampa su file dati per il grafico e tabella vvsDs
    url = "output/table/vvsDs.txt";
    ofstream oFile4(url.c_str());
    if (!oFile4) {

        cout << "Errore durante l'apertura del file" << endl;
        return -1;

    }
    oFile4 << fixed << setprecision(3);
    oFile4 << "%in ordine crescente di diametro" << endl;
    oFile4 << "%Ds[m]\tv[m/s]\tsigv[m/s]" << endl;
    for(i=0; i<data.size(); ++i){

        oFile4 << endl;
        for(int i1=0; i1<data[i].dati.size(); ++i1) oFile4 << data[i].dati[i1].s << "\t& " << data[i].dati[i1].v << "\t& " << data[i].dati[i1].sigv << " \\\\" << endl;

    }
    oFile4.close();

    url = "output/graph/vvsDs/";
    for(i=0; i<data.size(); ++i){

		ostringstream convert;
		convert << i + 1 ;
		nomeFile = "ball" + convert.str() + ".txt";
		ofstream oFile5((url + nomeFile).c_str());
        if (!oFile5) {

            cout << "Errore durante l'apertura del file" << endl;
            return -1;

        }
        oFile5 << fixed << setprecision(3);
        oFile5 << "#Ds [m]\tv [m/s]\tsigv[m/s]" << endl;
        for(int i1=0; i1<data[i].dati.size(); ++i1){

            oFile5 << data[i].dati[i1].s << "\t" << data[i].dati[i1].v << "\t" << data[i].dati[i1].sigv << endl;

        }
    oFile5.close();
    }

    url = "output/gnuPlotCmd1.txt";
    ofstream oFile6(url.c_str());
    if (!oFile6) {

        cout << "Errore durante l'apertura del file" << endl;
        return -1;

    }
    oFile6 << "set terminal png" << endl << "set output \"./graph/vvsDs/graph" << ".png\"" << endl;
    oFile6 << "set ylabel \"Velocità [m/s]\"" << endl;
    oFile6 << "set xlabel \"Lunghezza percorsa [m]\"" << endl;
    oFile6 << "set yrange [0:0.2]" << endl;
    oFile6 << "plot ";
    for(i=0; i<data.size(); ++i){

		oFile6 << "\"./graph/vvsDs/ball" << i+1 << ".txt\" title \"pallina " << i+1 <<"\",";


    }
    oFile6.close();

    //stampa tabella dati grezzi
    url = "output/table/datiRilevati.txt";
    ofstream oFile7(url.c_str());
    if (!oFile7) {

        cout << "Errore durante l'apertura del file" << endl;
        return -1;

    }
    oFile7 << fixed << setprecision(3);
    oFile7 << "%in ordine crescente di diametro, set di dati per ogni diametro" << endl;
    oFile7 << "%Dt[s] err 0,001 s" << endl;


    for(i=0; i<data.size(); ++i){

        oFile7 << endl;
        int i1=0;
        for (int i2=0; i2<nVal; ++i2){

            oFile7 << endl;
            for(i1=0; i1<data[i].dataSet.size(); ++i1){

                if (i1 != data[i].dataSet.size() -1) oFile7 << data[i].dataSet[i1].val[i2] << " & ";
                else oFile7 << data[i].dataSet[i1].val[i2] << " \\\\";

            }

        }

    }
    oFile7.close();

    //velcità limite
    url = "output/graph/DsvsDt/vL.txt";
    ifstream iFile1(url);
    if (!iFile1) {

        cout << "Errore durante l'apertura del file" << endl;
        return -1;

    }
    i=0;
    while (!iFile1.eof()) {

        iFile1 >> val;
        data[i].vL = val;
        ++i;

    }
    iFile1.close();

    url = "output/graph/D-2vsvL-1/D-2vsvL-1.txt";
    ofstream oFile8(url.c_str());
    if (!oFile8) {

        cout << "Errore durante l'apertura del file" << endl;
        return -1;

    }
    oFile8 << fixed << setprecision(3);
    oFile8 << "#D^-2 [m^-2]\tvL^-1 [s/m]" << endl;
    for(i=0; i<data.size(); ++i) oFile8 << 1./(data[i].d*data[i].d) << "\t" << 1./data[i].vL << endl;
    oFile8.close();

    // calcolo di eta per ogni pallina
    for(i=0; i<data.size(); ++i){

        data[i].eta = data[i].d*data[i].d*g*((data[i].rho-rhoF)/(18*data[i].vL));
        data[i].sigEta = sqrt(pow(2*data[i].d*data[i].sigd*g*(data[i].rho-rhoF)/(18.*data[i].vL),2)+pow(data[i].d*data[i].d*sigg*(data[i].rho-rhoF)/(18.*data[i].vL),2)+pow(data[i].sig_rho*data[i].d*data[i].d*g/(18.*data[i].vL),2)+pow(sigrhoF*data[i].d*data[i].d*g/(18.*data[i].vL),2)+pow(data[i].sigvL*data[i].d*data[i].d*g*1./(data[i].rho-rhoF)*1./(data[i].vL*data[i].vL),2));

    }

    url = "output/table/vLvsDvseta.txt";
    ofstream oFile9(url.c_str());
    if (!oFile9) {

        cout << "Errore durante l'apertura del file" << endl;
        return -1;

    }

    oFile9 << "%vL [m/s]\tsigvL [m/s]\tD [m]\t\teta [Poise]\tsigeta [Poise]" << endl;
    for(i=0; i<data.size(); ++i){

     oFile9 << fixed << setprecision(3);
     oFile9 << data[i].vL << "&\t\t";
     oFile9 << data[i].sigvL << "&\t\t";
     oFile9 << fixed << setprecision(6);
     oFile9 << data[i].d << "&\t";
     oFile9 << data[i].eta << "&\t";
     oFile9 << data[i].sigEta << "\t";
     oFile9 << "\\\\" << endl;


    }
    oFile9.close();

    //calcolo eta pesato e interpolato

    double sum=0, sum1=0;
    for(i=0; i<data.size(); ++i){

            sum+=data[i].eta/(data[i].sigEta*data[i].sigEta);
            sum1+=1./(data[i].sigEta*data[i].sigEta);

    }
    etaPes = 10.*sum/sum1;
    sig_etaPes = 10.*sqrt(1./sum1);
    etaInt = 10.*0.0013016*g*(data[0].rho-rhoF)/18.;
    sigEtaInt = 10.*sqrt(pow(g*((data[0].rho-rhoF)/18.)*0.0000341,2)+pow((0.0013016*(data[0].rho-rhoF)/18.)*sigg,2)+pow((0.0013016*g/18.)*data[0].sig_rho,2)+pow((0.0013016*g/18.)*sigrhoF,2));


    for(i=0; i<data.size(); ++i){

        cout << endl << "- PALLINA " << i+1 << " -" << endl;
        cout << fixed << setprecision(6) << endl;
        cout << "Diametro: (" << data[i].d << " +- " << data[i].sigd << ") m" << endl;
        cout << "Errore sul singolo tempo: " << data[i].sigt << " s" << endl;
        cout << "Errore sul tratto percorso: " << data[i].sigs << " m" << endl;
        cout << "Errore sul tempo medio: " << data[i].sigtMedio << " s" << endl;
        cout << fixed << setprecision(3);
        cout << "Velocita' limite: (" << data[i].vL << " +- " << data[i].sigvL << ") m/s" << endl;
        cout << fixed << setprecision(2);
        cout << "Viscosita': (" << data[i].eta << " +- " << data[i].sigEta << ") Poise" << endl;
        cout << fixed << setprecision(3);
        cout << "Dt medio [s]\tDs [m]\tv [m/s]\tsigv [m/s]" << endl;
        for(int i1=0; i1<data[i].dati.size(); ++i1){

            cout << data[i].dati[i1].t << "\t\t" << data[i].dati[i1].s << "\t" << data[i].dati[i1].v << "\t" << data[i].dati[i1].sigv << endl;

        }

    }
    cout << fixed << setprecision(2);
    cout << endl << "DATI CALCOLATI " << endl;
    cout << "Viscosita' pesata: (" << etaPes << " +- " << sig_etaPes << ") Poise" << endl;
    cout << "Viscosita' interpolata: (" << etaInt << " +- " << sigEtaInt << ") Poise" << endl;
    cout << fixed << setprecision(2);
    cout << "Compatibilita' (etaPes,etaInt)= " << compatibility(etaPes,etaInt,sig_etaPes,sigEtaInt) << endl;

    return 0;
}

double compatibility(double a, double b, double siga, double sigb)
{
	double lambda;
	lambda = abs(a-b) / sqrt((siga*siga)+(sigb*sigb));
	return lambda;
}
