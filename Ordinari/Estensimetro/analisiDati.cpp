#include<iostream>
#include<fstream>
#include<cmath>
#include<vector>
#include<string>
#include<sstream>
#include<iomanip>

#define _USE_MATH_DEFINES

using namespace std;

double compatibility(double, double, double, double);

struct datat{

	double grPeso;
	double Dx;

};

class estensimetro {

  public:

	int numero; //numero estensimetro
	int materiale; //materiale filo
	int tipo; //categoria estensimetro
	double l; //lunghezza filo a riposo
	double sigl; //errore sul l
	double d; //diametro filo
	double sigd; //errore su d
	vector<datat> dataCont; //dati in contrazione [g] [10^-5 m]
	vector<datat> dataAll;  //dati in allungamento [g] [10^-5 m]
	vector<datat> dataCont1; //dati in contrazione [N] [10^-5 m]
	vector<datat> dataAll1;  //dati in allungamento [N] [10^-5 m]
	double sigDx; //errore su Dx
	double siggrPeso; //errore su grPeso
	double sigForzaElastica; //errore sulla forza elastica
	double a; //intercetta con l'ordinata della retta interpolante in allungamento
	double siga; //errore su a
	double b; //coefficiente angolare della retta interpolante in allungamento
	double sigb; //errore su b
	double aCont; //intercetta con l'ordinata della retta interpolante in contrazione
	double sigaCont; //errore su a
	double bCont; //coefficiente angolare della retta interpolante in contrazione
	double sigbCont; //errore su b
	double E; //modulo di young ottenuto con la media pesata degli E
	double sigE; //errore su E
	double k; //costante della molla ottenuta con la media pesata dei k
	double sigk; //errore su k
	double Epes; //E ricavata dalla media pesata dei k
	double sigEpes; //errore su Epes
	void print();

};

int main() {


	int n = 13; //numero di estensimetri
	vector<estensimetro> est; //dati su ogni estensimetro
	vector<double> tmpData;
	string url = "data/input/est";
	string nomeFile;
	double val;
	double delta, sum1 = 0, sum2 = 0, sum3 = 0, sum4 = 0, sum5 = 0; //variabile per l'interpolazione lineare
	double g = 9.80665; //costante esatta del NIST
	double eAll, eCont, sigeAll, sigeCont;
	double s; //sezione filo ricavata da d
	int i, j;
	cout << "--- ANALISI DATI Estensimetro ---" << endl;

	//caricamento dati da file ed elaborazione base
	for (i = 1; i < n + 1; i++) {

		tmpData.clear();
		datat tmp1;
		estensimetro tmp2;
		ostringstream convert;
		convert << i;
		nomeFile = convert.str() + ".txt";
		ifstream iFile((url + nomeFile).c_str());
		if (!iFile) {

			cout << "Errore durante l'apertura del file" << endl;
			getchar();
			return -1;

		}
		while (!iFile.eof()) {

			iFile >> val;
			tmpData.push_back(val);

		}
		tmp2.numero = tmpData[0];
		tmp2.materiale = tmpData[1];
		tmp2.d = tmpData[2];
		tmp2.sigd = tmpData[3];
		tmp2.l = tmpData[4];
		tmp2.sigl = tmpData[5];
		for (j = 6; j < 25; j += 2) {

			tmp1.grPeso = tmpData[j];
			tmp1.Dx = tmpData[j + 1];
			tmp2.dataAll.push_back(tmp1);
			tmp1.grPeso = ((tmpData[j] - tmpData[6]) * g * 4) / 1000.;
			tmp2.dataAll1.push_back(tmp1);

		}
		for (j = 24; j < tmpData.size() - 1; j += 2) {

			tmp1.grPeso = tmpData[j];
			tmp1.Dx = tmpData[j + 1];
			tmp2.dataCont.push_back(tmp1);
			tmp1.grPeso = ((tmpData[j] - tmpData[6])* g * 4) / 1000.;
			tmp2.dataCont1.push_back(tmp1);

		}
		tmp2.sigDx = 1. / sqrt(12);
		tmp2.siggrPeso = 10. / sqrt(24); //un decimo della distanza tra le tacche di lettura a cui poi e' applicata la distribuzione triangolare
		tmp2.sigForzaElastica = (tmp2.siggrPeso*sqrt(2)*4*g)/1000.;
		est.push_back(tmp2);
		iFile.close();

	}

	//interpolazione lineare in allungamento
	for (i = 0; i < est.size(); i++) {

		sum1 = 0; sum2 = 0; sum3 = 0; sum4 = 0; sum5 = 0;
		for (j = 0; j < est[i].dataAll1.size(); j++) sum1 += (est[i].dataAll1[j].Dx * est[i].dataAll1[j].Dx) / (est[i].sigForzaElastica * est[i].sigForzaElastica);
		for (j = 0; j < est[i].dataAll1.size(); j++) sum3 += est[i].dataAll1[j].Dx / (est[i].sigForzaElastica * est[i].sigForzaElastica);
		for (j = 0; j < est[i].dataAll1.size(); j++) sum2 += est[i].dataAll1[j].grPeso / (est[i].sigForzaElastica * est[i].sigForzaElastica);
		for (j = 0; j < est[i].dataAll1.size(); j++) sum4 += (est[i].dataAll1[j].grPeso * est[i].dataAll1[j].Dx) / (est[i].sigForzaElastica * est[i].sigForzaElastica);
		for (j = 0; j < est[i].dataAll1.size(); j++) sum5 += 1. / (est[i].sigForzaElastica * est[i].sigForzaElastica);
		delta = (sum5*sum1) - (sum3*sum3);
		est[i].a = (1. / delta)*((sum1*sum2) - (sum3*sum4));
		est[i].b = (1. / delta)*((sum5*sum4) - (sum3*sum2));
		est[i].siga = sqrt(sum1 / delta);
		est[i].sigb = sqrt(sum5 / delta);

	}

	//interpolazione lineare in contrazione

	for (i = 0; i < est.size(); i++) {

		sum1 = 0; sum2 = 0; sum3 = 0; sum4 = 0; sum5 = 0;
		for (j = 0; j < est[i].dataCont1.size(); j++) sum1 += (est[i].dataCont1[j].Dx * est[i].dataCont1[j].Dx) / (est[i].sigForzaElastica * est[i].sigForzaElastica);
		for (j = 0; j < est[i].dataCont1.size(); j++) sum3 += est[i].dataCont1[j].Dx / (est[i].sigForzaElastica * est[i].sigForzaElastica);
		for (j = 0; j < est[i].dataCont1.size(); j++) sum2 += est[i].dataCont1[j].grPeso / (est[i].sigForzaElastica * est[i].sigForzaElastica);
		for (j = 0; j < est[i].dataCont1.size(); j++) sum4 += (est[i].dataCont1[j].grPeso * est[i].dataCont1[j].Dx) / (est[i].sigForzaElastica * est[i].sigForzaElastica);
		for (j = 0; j < est[i].dataCont1.size(); j++) sum5 += 1. / (est[i].sigForzaElastica * est[i].sigForzaElastica);
		delta = (sum5*sum1) - (sum3*sum3);
		est[i].aCont = (1. / delta)*((sum1*sum2) - (sum3*sum4));
		est[i].bCont = (1. / delta)*((sum5*sum4) - (sum3*sum2));
		est[i].sigaCont = sqrt(sum1 / delta);
		est[i].sigbCont = est[i].sigForzaElastica * sqrt( est[i].dataCont1.size() / delta);

	}

	//calcolo del modulo di young e della costante della molla

	for (i = 0; i < est.size(); i++) {

        eAll = (est[i].l * 4)/(est[i].d*est[i].d*(1./est[i].b)*M_PI);
        sigeAll = eAll*sqrt((est[i].sigl/est[i].l)*(est[i].sigl/est[i].l)+((est[i].sigb)/(1./est[i].b))*(est[i].sigb/(1./est[i].b))+4*(est[i].sigd/est[i].d)*(est[i].sigd/est[i].d));
        eCont = (est[i].l * 4)/(est[i].d*est[i].d*(1./est[i].bCont)*M_PI);
        sigeCont = eCont*sqrt((est[i].sigl/est[i].l)*(est[i].sigl/est[i].l)+((est[i].sigbCont)/(1./est[i].bCont))*((est[i].sigbCont)/(1./est[i].bCont))+4*(est[i].sigd/est[i].d)*(est[i].sigd/est[i].d));
        est[i].E = ((eAll/(sigeAll*sigeAll))+(eCont/(sigeCont*sigeCont)))/((1./(sigeAll*sigeAll))+(1./(sigeCont*sigeCont)));
        est[i].sigE = sqrt(1./((1./(sigeAll*sigeAll))+(1./(sigeCont*sigeCont))));
        est[i].k = ((1./est[i].b)/(est[i].sigb*est[i].sigb) + (1./est[i].bCont)/(est[i].sigbCont*est[i].sigbCont))/(1./(est[i].sigb*est[i].sigb) + 1./(est[i].sigbCont*est[i].sigbCont));
        est[i].sigk = 1./(1./(est[i].sigb*est[i].sigb) + 1./(est[i].sigbCont*est[i].sigbCont));
        est[i].Epes = (4*est[i].l)/(M_PI*est[i].d*est[i].d*(est[i].k));
        est[i].sigEpes = est[i].Epes*sqrt((est[i].sigl/est[i].l)*(est[i].sigl/est[i].l)+(est[i].sigk/(est[i].k))*(est[i].sigk/(est[i].k))+4*(est[i].sigd/est[i].d)*(est[i].sigd/est[i].d));

	}

	//verifica della dipende lineare tra <k> e 1/S e l=950mm per l'acciaio

	url = "data/output/kvsS.txt";
    ofstream oFile3(url.c_str());
	if (!oFile3) {

        cout << "Errore durante l'apertura del file" << endl;
		return -1;

    }
    oFile3 << "set termopt enhanced" << endl;
    oFile3 << "#1/stuff[10^{-6} m] k[10^{-5} m/N] sigk[10^{-5} m/N]" << endl;
	for (i = 0; i < est.size(); i++) {

        if (est[i].materiale == 1 && est[i].l == 950) {

            s = (M_PI*est[i].d*est[i].d)/4.;
            oFile3 << 1./s << " " << est[i].k << " " << est[i].sigk << endl;

        }

	}
	oFile3.close();

	//verifica della costanza del rapporto tra <k> e l e d=0.279mm

	url = "data/output/lvsR.txt";
    ofstream oFile4(url.c_str());
	if (!oFile4) {

        cout << "Errore durante l'apertura del file" << endl;
		return -1;

    }
    oFile4 << "#l[10^-3 m] <k>/l[10^-8 N^-1] sig(<k>/l)[10^-8 N^-1]" << endl;
	for (i = 0; i < est.size(); i++) {

        if (est[i].materiale == 1 && est[i].d == 0.279) {

            oFile4 << est[i].l << " " << est[i].k/est[i].l << " " << (est[i].k/est[i].l)*sqrt((est[i].sigl/est[i].l)*(est[i].sigl/est[i].l)+(est[i].sigk/est[i].k)*(est[i].sigk/est[i].k)) << endl;

        }

	}
	oFile4.close();

	//verifica della dipende lineare tra <k> e d^2 e L=950mm

	url = "data/output/d2vsP.txt";
    ofstream oFile5(url.c_str());
	if (!oFile5) {

        cout << "Errore durante l'apertura del file" << endl;
		return -1;

    }
    oFile5 << "#d^2[10^-6 m^2] <k>*d^2[10^-11 (m^3)/N] sig(<k>*d^2)[10^-11 (m^3)/N]" << endl;
	for (i = 0; i < est.size(); i++) {

        if (est[i].materiale == 1 && est[i].l == 950) {

            oFile5 << est[i].d*est[i].d << " " << est[i].k*est[i].d*est[i].d << " " << (est[i].k*est[i].d*est[i].d)*sqrt(4*(est[i].sigd/est[i].d)*(est[i].sigd/est[i].d)+((est[i].sigk)/est[i].k)*(est[i].sigk)/est[i].k) << endl;

        }

	}
	oFile4.close();

	url = "data/output/kvsl.txt";
    ofstream oFile6(url.c_str());
	if (!oFile6) {

        cout << "Errore durante l'apertura del file" << endl;
		return -1;

    }
    oFile6 << "#l[10^-3 m] k[10^-5 m/N] sigk[10^-5 m/N]" << endl;
	for (i = 0; i < est.size(); i++) {

        if (est[i].materiale == 1 && est[i].d == 0.279) {

            oFile6 << est[i].l << " " << est[i].k << " " << est[i].sigk << endl;

        }

	}
	oFile6.close();

	url = "data/output/kEval.txt";
    ofstream oFile7(url.c_str());
	if (!oFile7) {

        cout << "Errore durante l'apertura del file" << endl;
		return -1;

    }
    oFile7 << fixed << setprecision(1) << "# k[10^-5 m/N] sigk[10^-5 m/N] err" << endl;
	for (i = 0; i < est.size(); i++) oFile7 << est[i].k << " & " << est[i].sigk << " & " << 100*est[i].sigk/est[i].k << "\\%  \\\\" << endl;
	oFile7 << endl << "#<E>[10^8 Pa] sig<E>[10^8 Pa] acciaio" << endl;
	for (i = 0; i < est.size(); i++) if(est[i].materiale==1) oFile7 << est[i].E << " & " << est[i].sigE << " & " << 100*est[i].sigE/est[i].E << "\\% \\\\" << endl;
	oFile7 << endl << "#Epes[10^8 Pa] sigEpes[10^8 Pa]" << endl;
	for (i = 0; i < est.size(); i++) if(est[i].materiale==1) oFile7 << est[i].Epes << " & " << est[i].sigEpes << " & " << 100*est[i].sigEpes/est[i].Epes << "\\% \\\\" << endl;
	oFile7 << endl << "#<E>[10^8 Pa] sig<E>[10^8 Pa] tungsteno" << endl;
	for (i = 0; i < est.size(); i++) if(est[i].materiale==2) oFile7 << est[i].E << " & " << est[i].sigE << " & " << 100*est[i].sigE/est[i].E << "\\% \\\\" << endl;
	oFile7 << endl << "#Epes[10^8 Pa] sigEpes[10^8 Pa]" << endl;
	for (i = 0; i < est.size(); i++) if(est[i].materiale==2) oFile7 << est[i].Epes << " & " << est[i].sigEpes << " & " << 100*est[i].sigEpes/est[i].Epes << "\\%  \\\\" << endl;
	oFile7 << endl << "#<E>[10^8 Pa] sig<E>[10^8 Pa] ottone" << endl;
	for (i = 0; i < est.size(); i++) if(est[i].materiale==3) oFile7 << est[i].E << " & " << est[i].sigE << " & " << 100*est[i].sigE/est[i].E << "\\%  \\\\" << endl;
	oFile7 << endl << "#Epes[10^8 Pa] sigEpes[10^8 Pa]" << endl;
	for (i = 0; i < est.size(); i++) if(est[i].materiale==3) oFile7 << est[i].Epes << " & " << est[i].sigEpes << " & " << 100*est[i].sigEpes/est[i].Epes << "\\%  \\\\" << endl;

	oFile7.close();
    
    url = "data/output/lambda.txt";
    ofstream oFile8(url.c_str());
	if (!oFile8) {

        cout << "Errore durante l'apertura del file" << endl;
		return -1;

    }
    oFile8 << fixed << setprecision(1);
    for (i = 0; i < est.size(); i++) if(est[i].materiale==1) oFile8 << compatibility(est[i].E, est[i].Epes, est[i].sigE, est[i].sigEpes) << " & " << compatibility(est[i].E, 2050, est[i].sigE, 10) << "\\\\" << endl;
    for (i = 0; i < est.size(); i++) if(est[i].materiale==2) oFile8 << compatibility(est[i].E, est[i].Epes, est[i].sigE, est[i].sigEpes) << " & " << compatibility(est[i].E, 3790, est[i].sigE, 150)<< "\\\\" << endl;
    for (i = 0; i < est.size(); i++) if(est[i].materiale==3) oFile8 << compatibility(est[i].E, est[i].Epes, est[i].sigE, est[i].sigEpes) << " & " << compatibility(est[i].E, 960, est[i].sigE, 20)<< "\\\\" << endl;
    double sumE = 0, sumsig = 0;
    for (i = 0; i < est.size(); i++) if(est[i].materiale==1 && i != 7) {
        sumE += est[i].E / (est[i].sigE * est[i].sigE);
        sumsig += 1./(est[i].sigE * est[i].sigE);
    }
    oFile8 << sumE / sumsig << "+-" << sqrt(1./sumsig) << endl;
    oFile8 << compatibility(sumE / sumsig, 2050, sqrt(1./sumsig), 10) << endl;
    
    
    oFile8.close();

	//stampa dei dati su console e file

	for (i = 0; i < est.size(); i++) {

		cout << endl << "--- FILE #" << i + 1 <<" ---"<< endl;
		est[i].print();
		cout << endl << "Compatibilita'"<<endl;
		cout << " lambda(a1,a2) = " << compatibility(est[i].a, est[i].aCont, est[i].siga, est[i].sigaCont) << endl;
		cout << " lambda(b1,b2) = " << compatibility(est[i].b, est[i].bCont, est[i].sigb, est[i].sigbCont) << endl;
		cout << " lambda(<E>,Epes) = " << compatibility(est[i].E, est[i].Epes, est[i].sigE, est[i].sigEpes) << endl;

		url = "data/output/allung/est";
		ostringstream convert;
		convert << i + 1;
		nomeFile = convert.str() + ".txt";
		ofstream oFile((url + nomeFile).c_str());
		if (!oFile) {

			cout << "Errore durante l'apertura del file" << endl;
			return -1;

		}
		oFile << "#Allungamento [10^-5 m] [N] [N]"<<endl;
		for (j = 0; j < est[i].dataAll1.size(); j++) oFile << fixed << setprecision(1) << est[i].dataAll1[j].Dx << " " << est[i].dataAll1[j].grPeso << " " << est[i].sigForzaElastica << endl;
		oFile.close();

		url = "data/output/cont/est";
		ofstream oFile1((url + nomeFile).c_str());
		if (!oFile1) {

			cout << "Errore durante l'apertura del file" << endl;
			return -1;

		}
		oFile1 <<"#Contrazione [10^-5 m] [N] [N]"<<endl;
		for (j = 0; j < est[i].dataCont1.size(); j++) oFile1 << fixed << setprecision(1) << est[i].dataCont1[j].Dx << " " << est[i].dataCont1[j].grPeso << " " << est[i].sigForzaElastica << endl;
        oFile1.close();



	}

	url = "data/output/coeff.txt";
    ofstream oFile2(url.c_str());
    if (!oFile2) {

        cout << "Errore durante l'apertura del file" << endl;
        return -1;

    }

	for (i = 0; i < est.size(); i++) {

        oFile2 << "set terminal png" << endl << "set output \"allung/graf/est" << i+1 << ".png\"" << endl;
        oFile2 << "set termopt enhanced" << endl;
		oFile2 << "a=" << est[i].a << endl;
		oFile2 << "b=" << est[i].b << endl;
		oFile2 << "f(x)=b*x+a" << endl;
		oFile2 << "set ylabel \"Forza elastica [N]\"" << endl;
		oFile2 << "set xlabel \"Allungamento [10^{-5} m]\"" << endl;
		oFile2 << "plot f(x) title \"retta interpolante\" , 'allung/est" << i+1 << ".txt' title \"punti sperimentali\" with yerrorbars" << endl;
		oFile2 << "set terminal png" << endl << "set output \"cont/graf/estcont" << i+1 << ".png\"" << endl;
		oFile2 << "a=" << est[i].aCont << endl;
		oFile2 << "b=" << est[i].bCont << endl;
		oFile2 << "f(x)=b*x+a" << endl;
		oFile2 << "set ylabel \"Forza elastica [N]\"" << endl;
		oFile2 << "set xlabel \"Allungamento [10^{-5} m]\"" << endl;
        oFile2 << "plot f(x) title \"retta interpolante\" , 'cont/est" << i+1 << ".txt' title \"punti sperimentali\" with yerrorbars" << endl;

    }
    oFile2.close();
    
    for (i = 0; i < est.size(); i++) {
    cout << i+1 << "&" ;
	switch (est[i].materiale) {

	 case 1: cout << "acciaio"; break;
	 case 2: cout << "tungsteno"; break;
	 case 3: cout << "ottone"; break;

	}
    cout << fixed << setprecision(3);
    cout << "&" << est[i].d << "$\\pm$ 1\\%" << "&" << fixed << setprecision(0) << est[i].l << "$\\pm 2$ \\\\" << endl;
    }
    system("./createGraph.sh");
	
    cout << endl << endl;
    
    for (int i = 0; i < 13; i++){
        cout << "\\begin{figure}[h] \n\\begin{center} \n \\begin{tabular}{|c|c|c|}\n\\hline\n\$ x [\\SI{e-5}{m}]\$ & \$ \\Delta F \$ [N] & \$\\sigma_{F} $ [N] \\\\ \\hline" << endl;
        for (j = 0; j < est[i].dataCont1.size(); j++){
        cout << fixed << setprecision(1) << est[i].dataCont1[j].Dx << " & " << est[i].dataCont1[j].grPeso << " & " << est[i].sigForzaElastica << " \\\\" << endl;
        }
        cout << "\\hline\n\\end{tabular}\n\\caption{" << "Estensimetro numero " << i+1 << ", fase di contrazione." << "}\n\\end{center}\n\\end{figure}" << endl << endl << endl;
    }
    return 0;
}

void estensimetro::print() {

	int j;
	cout << endl;
	cout << "Estesimetro " << numero <<endl;
	switch (materiale) {

	 case 1: cout << " materiale filo: acciaio"; break;
	 case 2: cout << " materiale filo: tungsteno"; break;
	 case 3: cout << " materiale filo: ottone"; break;

	}
	cout << endl;
	cout << " diametro filo: (" << d << " +- " << sigd << ") mm" << endl;
	cout << " lunghezza filo: (" << l << " +- " << sigl << ") mm" << endl;
	cout << fixed << setprecision(0) << " errore sui grammi peso: " << siggrPeso << " g" << endl;
	cout << fixed << setprecision(1) << " errore sull' allungamento: " << sigDx << " *10^-5 m" << endl;
	cout << " errore sulla forza elastica: " << sigForzaElastica << " N" << endl;
	cout << endl;
	cout << fixed << setprecision(10) << "Retta interpolante in allungamento: y = " << b << "*x + " << a << endl;
	cout << " errore sul coefficiente angolare b: " << sigb << endl;
	cout << " errore sull' intercetta con le ordinate a: " << siga << endl;
	cout << endl;
	cout << "Retta interpolante in contrazione: y = " << bCont << "*x + " << aCont << endl;
	cout << " errore sul coefficiente angolare b: " << sigbCont << endl;
	cout << " errore sull' intercetta con le ordinate a: " << sigaCont << endl;
	cout << endl;
	cout << fixed << setprecision(1) << "Modulo di Young e Costanti" << endl;
	cout << " modulo di Young\t<E> = ( " << E << " +- " << sigE << " ) *10^8 Pa" << endl;
	cout << " costante elastica\t<k> = ( " << k << fixed << setprecision(8) << " +- " << sigk << " ) *10^-5 m/N" << endl;
	cout << fixed << setprecision(1) << " modulo di Young\tEpes = ( " << Epes << " +- " << sigEpes << " ) *10^8 Pa" << endl;
	//cout << "Dati allungamento [g], [10^-5 m]" << endl;
	//for (j = 0; j < dataAll.size(); j++) cout << dataAll[j].grPeso << "," << dataAll[j].Dx << endl;
	cout << fixed << setprecision(1) << endl << "Dati allungamento [N], [10^-5 m]" << endl;
	for (j = 0; j < dataAll1.size(); j++) cout <<" "<< dataAll1[j].grPeso << "," << dataAll1[j].Dx << endl;
	//cout << endl << "Dati contrazione [g], [10^-5 m]" << endl;
	//for (j = 0; j < dataCont.size(); j++) cout << dataCont[j].grPeso << "," << dataCont[j].Dx << endl;
	cout << endl << "Dati contrazione [N], [10^-5 m]" << endl;
	for (j = 0; j < dataCont1.size(); j++) cout << " "<< dataCont1[j].grPeso << "," << dataCont1[j].Dx << endl;
}

double compatibility(double a, double b, double siga, double sigb)
{
	double lambda;
	lambda = abs(a-b) / sqrt((siga*siga)+(sigb*sigb));
	return lambda;
}
