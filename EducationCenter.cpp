#include<iostream>
#include<iomanip>
#include<cstring>

using namespace std;
#pragma warning(disable:4996)

enum enumPredmeti { Matematika, Historija, Geografia, Hemija, Biologija, Engleski };
enum enumRazredi { I, II, III, IV };
char *predmetChar[] = { "Matematika","Historija","Geografija","Hemija","Biologija","Engleski" };
char *razrediChar[] = { "I","II","III","IV" };
char *crt = "\n--------------------------------------------\n";

template<class T>
T *expandArray(T *niz, int brc) {
	T *temp = new T[brc];
	for (int i = 0; i < brc; i++)
		swap(temp[i], niz[i]);
	delete[] niz; niz = nullptr;
	niz = new T[brc + 1];
	for (int i = 0; i < brc; i++)
		swap(temp[i], niz[i]);
	delete[] temp; temp = nullptr;
	return niz;
}

struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; }
	int dani() { return *_godina * 365 + *_mjesec * 30 + *_dan; }
};
int compareDate(Datum d1, Datum d2) {
	if (*d1._godina > *d2._godina)return 1;
	else if (*d1._godina < *d2._godina)return -1;
	else if (*d1._mjesec > *d2._mjesec)return 1;
	else if (*d1._mjesec < *d2._mjesec)return -1;
	else if (*d1._dan > *d2._dan)return 1;
	else if (*d1._dan < *d2._dan)return -1;
	else return 0;
}
struct Kurs {
	enumPredmeti _predmet;
	enumRazredi _razredi;
	Datum * _pocetak;
	Datum * _kraj;
	char * _imePredavaca;
	void Unos(enumPredmeti predmet, enumRazredi razred, Datum pocetak, Datum kraj, char * imePredavaca) {
		_predmet = predmet;
		_razredi = razred;
		_pocetak = new Datum, _kraj = new Datum;
		_pocetak->Unos(*pocetak._dan, *pocetak._mjesec, *pocetak._godina);
		_kraj->Unos(*kraj._dan, *kraj._mjesec, *kraj._godina);
		int size = strlen(imePredavaca) + 1;
		_imePredavaca = new char[size];
		strcpy_s(_imePredavaca, size, imePredavaca);
	}
	void Ispis() {
		cout << "-----> Kurs iz " << predmetChar[_predmet] << " <------\n";
		cout << "Razred: " << razrediChar[_razredi] << endl;
		cout << "Datum pocetka: ";
		_pocetak->Ispis();
		cout << "Datum zavrsetka: ";
		_kraj->Ispis();
		cout << "Ime predavaca: " << _imePredavaca << endl;
	}
	void Dealociraj() {
		delete[] _imePredavaca; _imePredavaca = nullptr;
		_pocetak->Dealociraj();
		_kraj->Dealociraj();
		_pocetak = _kraj = nullptr;
	}
};
struct Polaznik {
	Datum * _datumRodjenja;
	char * _imePrezime;
	void Unos(Datum datumRodjenja, char * imePrezime) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_datumRodjenja = new Datum;
		_datumRodjenja->Unos(*datumRodjenja._dan, *datumRodjenja._mjesec, *datumRodjenja._godina);
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		_datumRodjenja->Dealociraj(); _datumRodjenja = nullptr;
	}
	void Ispis() {
		cout << "Ime i prezime: " << _imePrezime << endl << "Datum rodjenja: ";
		_datumRodjenja->Ispis();
	}
};
struct Polaganja {
	Polaznik * _polaznik;
	Kurs * _kurs;
	Datum * _datumPolaganja;
	int _ocjena; //ocjene su u rasponu od 1 do 5	
	void Unos(Polaznik p, Kurs k, Datum d, int ocjena) {
		_polaznik = new Polaznik;
		_polaznik->Unos(*p._datumRodjenja, p._imePrezime);
		_kurs = new Kurs;
		_kurs->Unos(k._predmet, k._razredi, *k._pocetak, *k._kraj, k._imePredavaca);
		_datumPolaganja = new Datum;
		_datumPolaganja->Unos(*d._dan, *d._mjesec, *d._godina);
		_ocjena = ocjena;
	}
	void Ispis() {
		cout << "--> POLAGANJe INFO <---\n";
		cout << "--Podaci polaznika--\n";
		_polaznik->Ispis();
		cout << "Datum polaganja: ";
		_datumPolaganja->Ispis();
		cout << "Ocjena: " << _ocjena << endl;
	}
};

struct EdukacijskiCentar {
	char * _nazivCentra;
	Kurs * _kursevi[50];//JEDAN EDUKACIJSKI CENTAR MOZE NUDITI NAJVISE 50 KURSEVA
	int _trenutnoKurseva;
	Polaganja * _polaganja;
	int _trenutnoPolaganja;
	void Unos(const char *naziv) {
		int v = strlen(naziv) + 1;
		_nazivCentra = new char[v];
		strcpy_s(_nazivCentra, v, naziv);
		_trenutnoKurseva = 0;
		_polaganja = nullptr;
		_trenutnoPolaganja = 0;
	}
	
	bool DodajKurs(Kurs k) {
		if (_trenutnoKurseva == 50)return false;
		if (_trenutnoKurseva > 0) {
			for (int i = _trenutnoKurseva - 1; i >= 0; i--)
				if (k._predmet == _kursevi[i]->_predmet)
					if (compareDate(*k._pocetak, *_kursevi[i]->_kraj) == -1)
						return false;
					else break;
		}
		_kursevi[_trenutnoKurseva] = new Kurs;
		_kursevi[_trenutnoKurseva++]->Unos(k._predmet, k._razredi, *k._pocetak, *k._kraj, k._imePredavaca);
		return true;
	}
	
	bool DodajPolaganje(Polaganja p) {
		if (_trenutnoKurseva == 0)return false;
		bool kursPostoji = false;
		for (int i = 0; i < _trenutnoKurseva; i++) {
			if (p._kurs->_predmet == _kursevi[i]->_predmet)
				if (compareDate(*_kursevi[i]->_pocetak, *p._kurs->_pocetak) == 0 && strcmp(p._kurs->_imePredavaca, _kursevi[i]->_imePredavaca) == 0)
					kursPostoji = true;
		}
		if (!kursPostoji || (p._datumPolaganja->dani() - _kursevi[_trenutnoKurseva - 1]->_kraj->dani()) > 15)return false;
		if (_trenutnoPolaganja > 0)
			for (int i = 0; i < _trenutnoPolaganja; i++)
				if (strcmp(_polaganja[i]._polaznik->_imePrezime, p._polaznik->_imePrezime) == 0)
					if (p._kurs->_predmet == _polaganja[i]._kurs->_predmet && p._ocjena <= _polaganja[i]._ocjena)return false;
		
		if (_trenutnoPolaganja > 0)_polaganja = expandArray(_polaganja, _trenutnoPolaganja);
		else _polaganja = new Polaganja[_trenutnoPolaganja + 1];
		_polaganja[_trenutnoPolaganja++].Unos(*p._polaznik, *p._kurs, *p._datumPolaganja, p._ocjena);
		return true;
	}
	//PolaganjaByDatumIRazred - funkcija vraca niz uspjesnih polaganja koja su realizovana u periodu OD - DO, a odnose se na bilo koji predmet u 
	//definisanom razredu (npr. II)

	Polaganja *PolaganjaByDatumIRazred(Datum OD,Datum DO, enumRazredi er, int &brp) {
		if (_trenutnoPolaganja == 0)return nullptr;
		Polaganja *niz = new Polaganja[_trenutnoPolaganja];
		for (int i = 0; i < _trenutnoPolaganja; i++) {
			if (_polaganja[i]._kurs->_razredi == er)
			{
				int u1 = compareDate(OD, *_polaganja[i]._datumPolaganja), u2 = compareDate(DO, *_polaganja[i]._datumPolaganja);
				if (u1 == -1 || u1 == 0 || u2 == 0 || u2 == 1)
					niz[brp++].Unos(*_polaganja[i]._polaznik, *_polaganja[i]._kurs, *_polaganja[i]._datumPolaganja, _polaganja[i]._ocjena);
			}
		}
		return niz;
	}
	void Ispis() {
		int temp = _trenutnoPolaganja;
		int iz;
		do {
			iz = 1;
			for (int i = 0; i < temp - 1; i++)
				if (_polaganja[i]._ocjena > _polaganja[i]._ocjena) {
					swap(_polaganja[i], _polaganja[i + 1]);
					iz = 0;
				}
			temp -= 1;
		} while (temp > 1 && iz == 0);
		cout << "\n\n-----> EDUKACIJSKI CENTAR '" << _nazivCentra << "' <------" << crt;
		cout << "Trenutno kurseva: " << _trenutnoKurseva << endl;
		if (_trenutnoKurseva > 0) {
			cout << "--Lista kurseva--" << crt;
			for (int i = 0; i < _trenutnoKurseva; i++)
				_kursevi[i]->Ispis();
		}
		cout << "Trenutno polaganja: " << _trenutnoPolaganja << endl;
		if (_trenutnoPolaganja > 0) {
			cout << "--Lista polaganja--" << crt;
			for (int i = 0; i < _trenutnoPolaganja; i++)
				_polaganja[i].Ispis();
		}

	}
};
//PretragaRekurzivno - rekurzivna funkcija koja pronalazi prosjecni uspjeh koji su polaznici tokom godine (npr.2016) ostvarili na odredjenom kursu 

float PretragaRekurzivno(EdukacijskiCentar ec,Kurs k,int g,int i=0,float ocj=0,int j=0) {
	if (i == ec._trenutnoPolaganja)return (float)ocj / j;
	if (k._predmet == ec._polaganja[i]._kurs->_predmet) {
		if (compareDate(*ec._polaganja[i]._kurs->_pocetak, *k._pocetak) == 0 && strcmp(k._imePredavaca, ec._polaganja[i]._kurs->_imePredavaca) == 0)
			if (g == *ec._polaganja[i]._datumPolaganja->_godina)
				return PretragaRekurzivno(ec, k, g, i + 1, ocj + ec._polaganja[i]._ocjena, j + 1);
	}
	else return PretragaRekurzivno(ec, k, g, i + 1, ocj, j);
			
}
void main() {

	Datum datum1, datum2, datum3, datum4, datum5, datum6;
	datum1.Unos(26, 11, 2015);
	datum2.Unos(29, 5, 2016);
	datum3.Unos(5, 6, 2016);
	datum4.Unos(15, 8, 2016);
	datum5.Unos(13, 6, 2016);
	datum6.Unos(22, 9, 2016);

	Kurs matematika, hemija, biologija, engleski;
	matematika.Unos(Matematika, II, datum1, datum2, "Nina Bijedic");
	hemija.Unos(Hemija, III, datum2, datum3, "Murat Praso");
	biologija.Unos(Biologija, IV, datum3, datum4, "Jasmin Azemovic");
	engleski.Unos(Engleski, II, datum3, datum4, "Iris Memic");

	Datum rodjenje1, rodjenje2, rodjenje3;
	rodjenje1.Unos(22, 8, 1982);
	rodjenje2.Unos(30, 9, 1991);
	rodjenje3.Unos(31, 9, 1988);
	Polaznik denis, zanin, indira;
	denis.Unos(rodjenje1, "Denis Music");
	zanin.Unos(rodjenje2, "Zanin Vejzovic");
	indira.Unos(rodjenje3, "Indira Hamulic");


	Polaganja denisMatematika, zaninHemija, denisBiologija;
	denisMatematika.Unos(denis, matematika, datum5, 3);//3 predstavlja ostvareni uspjeh/ocjenu, a uspjesno polozenim se smatra svaki kurs na kome je polaznik ostvario ocjenu veću od 1
	zaninHemija.Unos(zanin, hemija, datum6, 93);

	EdukacijskiCentar mostar;
	mostar.Unos("ECM - Edukacijski Center Mostar");

	if (mostar.DodajKurs(matematika)) {
		cout << crt << "KURS USPJESNO REGISTROVAN" << crt;
		matematika.Ispis();
		cout << crt;
	}
	//nije moguce dodati polaganje onog kursa koji nije evidentiran (registrovan, ponudjen) u tom edukacijskom centru
	//kurseve je moguce najkasnije 15 dana od dana zavrsetka (kraja) posljednje dodanog kursa
	//jedan polaznik moze vise puta polagati isti kurs, ali nova ocjena mora biti veca od prethodne ocjene na tom istom kursu
	if (mostar.DodajPolaganje(denisMatematika)) {
		cout << crt << "POLAGANJE EVIDENTIRANO" << crt;
		denisMatematika.Ispis();
	}
	Datum OD, DO;
	int brojPolaganja = 0;
	OD.Unos(1, 6, 2016);
	DO.Unos(1, 8, 2016);
	//PolaganjaByDatumIRazred - funkcija vraca niz uspjesnih polaganja koja su realizovana u periodu OD - DO, a odnose se na bilo koji predmet u definisanom razredu (npr. II)
	Polaganja * polaganjaByDatumiRazred = mostar.PolaganjaByDatumIRazred(OD, DO, II, brojPolaganja);
	cout << "\n\nU periodu od ";
	OD.Ispis();
	cout << " do ";
	DO.Ispis();
	cout << " uspjesno je polozeno " << brojPolaganja << " predmeta --->>>\n ";
	for (int i = 0; i < brojPolaganja; i++)
		polaganjaByDatumiRazred[i].Ispis();

	cout << "\n\nProsjecan uspjeh na kursu iz predmeta Matematika u 2016 godini je " << PretragaRekurzivno(mostar,matematika,2016) << endl;
	//ispisuje sve informacije o edukacijskom centru. prije ispisa sortirati sve kurseve
	mostar.Ispis();

	//izvrsiti potrebne dealokacije
	system("pause");
}