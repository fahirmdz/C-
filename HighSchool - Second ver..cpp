#include<iostream>
#include <memory>
#include <iomanip>
#include <cstring>
using namespace std;

#pragma warning(disable:4996)

char *crt = "\n-------------------------------------------\n";

enum eNacinStudiranja { REDOVAN, DL };
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
const char *nacinStudiranja[] = { "REDOVAN","DL" };
const char *razred[] = { "PRVI","DRUGI","TRECI","CETVRTI" };

struct Datum {
	int _dan, _mjesec, _godina;
	void Unos(int d, int m, int g) { _dan = d; _mjesec = m; _godina = g; }
	void Ispis() { cout << _dan << "/" << _mjesec << "/" << _godina << endl; }
};
int compareDate(Datum d1, Datum d2) {
	if (d1._godina > d2._godina)return 1;
	else if (d1._godina < d2._godina)return -1;
	else if (d1._mjesec > d2._mjesec)return 1;
	else if (d1._mjesec < d2._mjesec)return -1;
	else if (d1._dan > d2._dan)return 1;
	else if (d1._dan < d2._dan)return -1;
	else return 0;
}
const Datum rokZaPrijavu = { 5, 7, 2017 };

char * errVecDodan = "Istoimeni predmet se moze dodati najvise dva puta!";
char * errProsaoRok = "Zao nam je. Rok za prijavu je vec prosao!";
char * uspjesnoDodan = "Predmet uspjesno dodan!";

struct Predmet {
	char * _naziv;
	int * _ocjena;
	Datum _datumUnosa;
	void Unos(char * naziv, int ocjena, Datum datumUnosa) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_ocjena = new int(ocjena);
		_datumUnosa.Unos(datumUnosa._dan, datumUnosa._mjesec, datumUnosa._godina);
	}
	void Dealociraj() {
		delete[] _naziv; _naziv = nullptr;
		delete _ocjena; _ocjena = nullptr;
	}

	void Ispis() {
		cout << _naziv << " (" << *_ocjena << ") "; _datumUnosa.Ispis();
	}
	void PromijeniOcjenu(int ocjena) { *_ocjena = ocjena; }
};

struct Uspjeh {
	eRazred  _razred;
	bool _najboljiUcenik; // ako je kandidat u tom razredu proglasen najboljim ucenikom
	shared_ptr<Predmet> * _predmeti;
	int _brojPredmeta;
	void Unos(eRazred razred, bool najboljiUcenik) {
		_razred = razred;
		_najboljiUcenik = najboljiUcenik;
		_brojPredmeta = 0;
		_predmeti = nullptr;
	}
	void Dealociraj() {
		if (_brojPredmeta > 0) {
			for (int i = 0; i < _brojPredmeta; i++) {
				_predmeti[i]->Dealociraj();
				_predmeti[i].reset();
			}
			delete[] _predmeti;
			_predmeti = nullptr;
		}
	}
	void Ispis() {
		cout << crt << "Razred -> " << razred[(int)_razred - 1] << "\nNajbolji -> ";
		if(_najboljiUcenik)cout<<"DA" << crt;
		else cout << "NE" << crt;
		if (_brojPredmeta > 0)
			for (size_t i = 0; i < _brojPredmeta; i++)
				_predmeti[i]->Ispis();
	}
};

struct Kandidat {
	eNacinStudiranja _nacinStudiranja;
	char * _imePrezime;
	Uspjeh * _uspjeh[4];

	void Unos(eNacinStudiranja nacinStudiranja, char * imePrezime) {
		int vel = strlen(imePrezime) + 1;
		_imePrezime = new char[vel];
		strcpy_s(_imePrezime, vel, imePrezime);
		_nacinStudiranja = nacinStudiranja;
		for (size_t i = 0; i < 4; i++)
			_uspjeh[i] = nullptr;
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < 4; i++) 
			if (_uspjeh[i] != nullptr) {
				_uspjeh[i]->Dealociraj();
				delete _uspjeh[i];
			}
	}
	void Ispis() {
		cout << crt <<"Ime i prezime: "<< _imePrezime << "\nNacin studiranja: " << nacinStudiranja[_nacinStudiranja];
		for (size_t i = 0; i < 4; i++)
			if (_uspjeh[i] != nullptr)
				_uspjeh[i]->Ispis();
	}
	bool DodajPredmet(eRazred r, Predmet p) {
		auto dodajPr = [p](Uspjeh *usp) {
			if (usp->_brojPredmeta > 0) {
				Predmet *temp = new Predmet[usp->_brojPredmeta];
				for (int i = 0; i < usp->_brojPredmeta; i++)
					swap(*usp->_predmeti[i], temp[i]);
				delete[] usp->_predmeti;
				usp->_predmeti = nullptr;
				usp->_predmeti = new shared_ptr<Predmet>[usp->_brojPredmeta + 1];
				for (int i = 0; i < usp->_brojPredmeta; i++)
				{
					usp->_predmeti[i] = make_shared<Predmet>();
					usp->_predmeti[i]->Unos(temp[i]._naziv, *temp->_ocjena, temp->_datumUnosa);
				}
				usp->_predmeti[usp->_brojPredmeta] = make_shared<Predmet>();
				delete[] temp; temp = nullptr;
			}
			else {
				usp->_predmeti = new shared_ptr<Predmet>[usp->_brojPredmeta + 1];
				usp->_predmeti[usp->_brojPredmeta] = make_shared<Predmet>();
			}
			usp->_predmeti[usp->_brojPredmeta++]->Unos(p._naziv, *p._ocjena, p._datumUnosa);
		};
		int br = 0;
		for (int i = 0; i < 4; i++)
			if (_uspjeh[i] != nullptr)
				for (int j = 0; j < _uspjeh[i]->_brojPredmeta; j++)
					if (strcmp(p._naziv, _uspjeh[i]->_predmeti[j]->_naziv) == 0)
						br++;
		if (br > 1) {
			cout << errVecDodan << endl;
			return false;
		}
		if (compareDate(p._datumUnosa, rokZaPrijavu) == 1 || compareDate(p._datumUnosa, rokZaPrijavu) == 0) {
			cout << errProsaoRok << endl;
			return false;
		}
		int t = -1;
		for (int i = 0; i < 4; i++)
			if (_uspjeh[i] != nullptr)
				if (_uspjeh[i]->_razred == r)
					t = i;
		if (t > -1) {
			dodajPr(_uspjeh[t]);
			cout << uspjesnoDodan << endl;
			return true;
		}
			
		if(t==-1)
			while (_uspjeh[t] != nullptr) {
				t++;
			} 
		
		_uspjeh[t] = new Uspjeh;
		_uspjeh[t]->Unos(r, false);
		dodajPr(_uspjeh[t]);
		cout << uspjesnoDodan << endl;
		return true;
	}
};
/*
napisati rekurzivnu funkciju koja ce vratiti pokazivac na kandidata sa najvecim prosjekom u razredu koji je
proslijedjen kao parametar. ukoliko je vise kandidata ostvarilo isti prosjek, funkcija treba da vrati posljednje
pronadjenog kandidata. u slucaju da niti jedan kandidat nije evidentirao uspjeh u trazenom razredu, funkcija vraca
nullptr. u nastavku je prikazan primjer poziva rekurzivne funkcije, a ostale parametre dodajte po potrebi. */

Kandidat *rekNajboljiProsjek(Kandidat *niz, int brk, eRazred r, Kandidat *trazeni = nullptr, float prosjTrazenog = 0,float temp=0, int i = 0,int j=0,int z=0) {
	if (i == brk)return trazeni;
	if (niz[i]._uspjeh[i]->_brojPredmeta == 0) {
		j++;
		if (j == 4) {
			j = 0;
			i++;
		}
		return rekNajboljiProsjek(niz, brk, r, trazeni, prosjTrazenog, temp, i, j, z);
	}
	if (niz[i]._uspjeh[j] != nullptr && niz[i]._uspjeh[j]->_brojPredmeta>0 && niz[i]._uspjeh[j]->_razred == r) {
		if (z == niz[i]._uspjeh[j]->_brojPredmeta) {
			if (temp > prosjTrazenog || temp == prosjTrazenog)
				return rekNajboljiProsjek(niz, brk, r, &niz[i], temp, 0, i + 1, j, 0);
			else return rekNajboljiProsjek(niz, brk, r, trazeni, prosjTrazenog, 0, i + 1, j, 0);
		}
		else {
			temp += *niz[i]._uspjeh[j]->_predmeti[z]->_ocjena;
			return rekNajboljiProsjek(niz, brk, r, trazeni, prosjTrazenog, temp, i, j, z + 1);
		}
	}
	else return rekNajboljiProsjek(niz, brk, r, trazeni, prosjTrazenog, temp, i, j + 1, z);
}
void main() {
	Datum datum19062017, datum20062017, datum30062017, datum05072017;
	datum19062017.Unos(19, 6, 2017);
	datum20062017.Unos(20, 6, 2017);
	datum30062017.Unos(30, 6, 2017);
	datum05072017.Unos(5, 7, 2017);

	Predmet Matematika, Fizika, Hemija, Engleski;
	//2 - ocjena na predmetu; datum - datum evidentiranja uspjeha na predmetu jer postoji krajnji rok za evidentiranje
	Matematika.Unos("Matematika", 2, datum19062017);
	Fizika.Unos("Fizika", 5, datum20062017);
	Hemija.Unos("Hemija", 2, datum20062017);
	Engleski.Unos("Engleski", 5, datum05072017);

	int brojKandidata = 2;

	Kandidat * prijave2017 = new Kandidat[brojKandidata];
	prijave2017[0].Unos(DL, "Jasmin Azemovic");
	prijave2017[1].Unos(REDOVAN, "Indira Hamulic");
	
	if (prijave2017[0].DodajPredmet(DRUGI, Engleski)) //ne bi trebao dodati jer je prosao postavljeni rok za dodavanje predmeta
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(DRUGI, Matematika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(PRVI, Fizika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(PRVI, Hemija))
		cout << "Predmet uspjesno dodan!" << crt;

	Matematika.PromijeniOcjenu(5);
	Hemija.PromijeniOcjenu(3);

	if (prijave2017[1].DodajPredmet(PRVI, Matematika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(DRUGI, Matematika))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(TRECI, Matematika))//ne bi trebalo ponovo dodati Matematiku!
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(TRECI, Hemija))
		cout << "Predmet uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(DRUGI, Engleski))
		cout << "Predmet uspjesno dodan!" << crt;
		
	auto najboljiUspjeh = [brojKandidata,prijave2017](eNacinStudiranja ns,bool naj) {
		float **prosj = new float *[brojKandidata];
		for (int i = 0; i < brojKandidata; i++)prosj[i] = new float[4];
		for (int i = 0; i < brojKandidata; i++)
			if (ns == prijave2017[i]._nacinStudiranja) {
				for (int j = 0; j < 4; j++)
					if (prijave2017[i]._uspjeh[j] != nullptr && prijave2017[i]._uspjeh[j]->_najboljiUcenik==naj)
						for (int z = 0; z < prijave2017[i]._uspjeh[j]->_brojPredmeta; z++) {
							prosj[i][j] += *prijave2017[i]._uspjeh[j]->_predmeti[z]->_ocjena;
							if (z == prijave2017[i]._uspjeh[j]->_brojPredmeta - 1)(float)prosj[i][j] /= prijave2017[i]._uspjeh[j]->_brojPredmeta;
						}
					else
						prosj[i][j] = 0;
			}
		int najpri = 0, najprj = 0;
		for(int i=0;i<brojKandidata;i++)
			for(int j=1;j<4;j++)
				if (prosj[najpri][najprj] < prosj[i][j]) {
					najpri = i;
					najprj = j;
				}
		return prijave2017[najpri]._uspjeh[najprj];
	};
	Uspjeh * najbolji = najboljiUspjeh(DL, true);
	najbolji->Ispis();

	Kandidat * kandidatSaNajboljimProsjekom = rekNajboljiProsjek(prijave2017, brojKandidata, DRUGI);
	cout << "--Kandidat sa najboljim prosjekom--\n";
	cout << kandidatSaNajboljimProsjekom->_imePrezime << endl;
	for (size_t i = 0; i < brojKandidata; i++)
	{
		prijave2017[i].Ispis();
		prijave2017[i].Dealociraj();
	}
	system("pause>0");
}
