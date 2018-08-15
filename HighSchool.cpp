#include <iostream>
#include <cstring>
#include <memory>
using namespace std;
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NEĆE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI ĆETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAĐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

char *crt = "\n-------------------------------------------\n";

enum eNacinStudiranja { REDOVAN, DL };
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
const char *razred[] = { "PRVI","DRUGI","TRECI","CETVRTI" };
const char *nacinStudiranja[] = { "REDOVAN","DL" };
struct DatumVrijeme {
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
	void Unos(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	void Dealociraj() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}
	void Ispis() {
		cout << *_dan << "." << *_mjesec << "." << *_godina << " " << *_sati << ":" << *_minuti << endl;
	}
	char *GetDatumKaoNizKaraktera() {
		char *day, *month, *hours, *minutes;
		day= new char[3];
		month = new char[3];
		hours = new char[3];
		minutes = new char[3];
		char *year = new char[5];
		itoa(*_dan, day, 10);
		itoa(*_mjesec, month, 10);
		itoa(*_godina, year, 10);
		itoa(*_sati, hours, 10);
		itoa(*_minuti, minutes, 10);
		if (*_dan  <10) {
			day[1] = day[0];
			day[0] = '0';
		}
		if (*_mjesec <10) {
			month[1] = month[0];
			month[0] = '0';
		}
		month[2] = '\0';
		if (*_sati <10) {
			hours[1] = hours[0];
			hours[0] = '0';
		}
		
		if (*_minuti <10) {
			minutes[1] = minutes[0];
			minutes[0] = '0';
		}
		char *datum = new char[25];
		strcpy_s(datum, 25, day);
		strcat_s(datum, 25, "/");
		strcat_s(datum,25, month);
		strcat_s(datum, 25, "/");
		strcat_s(datum, 25, year);
		strcat_s(datum, 25, ", ");
		strcat_s(datum, 25, hours);
		strcat_s(datum, 25, ":");
		strcat_s(datum, 25, minutes);
		delete[] day, month, year, minutes, hours; day = month = year = minutes = hours = nullptr;
		return datum;
	}
};
int compareDate(DatumVrijeme d1, DatumVrijeme d2) {
	if (*d1._godina > *d2._godina)return 1;
	else if (*d1._godina < *d2._godina)return -1;
	else if (*d1._mjesec > *d2._mjesec)return 1;
	else if (*d1._mjesec < *d2._mjesec)return -1;
	else if (*d1._dan > *d2._dan)return 1;
	else if (*d1._dan < *d2._dan)return -1;
	else if (*d1._sati > *d2._sati)return 1;
	else if (*d1._sati < *d2._sati)return -1;
	else if (*d1._minuti > *d2._minuti)return 1;
	else if (*d1._minuti < *d2._minuti)return -1;
	else return 0;
}

const DatumVrijeme rokZaPrijavu = { new int(5), new int(7), new int(2017), new int(12), new int(30) };


struct Predmet {
	char * _naziv;
	int _ocjena;
	DatumVrijeme * _datumUnosa;
	void Unos(char * naziv, int ocjena, DatumVrijeme datumUnosa) {
		int vel = strlen(naziv) + 1;
		_naziv = new char[vel];
		strcpy_s(_naziv, vel, naziv);
		_ocjena = ocjena;
		_datumUnosa = new DatumVrijeme;
		_datumUnosa->Unos(*datumUnosa._dan, *datumUnosa._mjesec, *datumUnosa._godina, *datumUnosa._sati, *datumUnosa._minuti);
	}
	void Dealociraj() {
		delete[] _naziv; _naziv = nullptr;
		_datumUnosa->Dealociraj(); delete _datumUnosa;
	}
	void Ispis() {	cout << _naziv << " (" << _ocjena << ") " << _datumUnosa->GetDatumKaoNizKaraktera() << endl;}
};

struct Uspjeh {
	eRazred _razred;
	Predmet * _predmeti;
	int _brojPredmeta;
	void Unos(eRazred razred) {
		_razred = razred;
		_predmeti = nullptr;
		_brojPredmeta = 0;
	}
	void Dealociraj() {
		for (size_t i = 0; i < _brojPredmeta; i++)
			_predmeti[i].Dealociraj();
		delete[] _predmeti; _predmeti = nullptr;
	}

	void Ispis() {
		cout << crt << "Razred -> " << razred[(int)_razred-1] << crt;
		for (size_t i = 0; i < _brojPredmeta; i++)
			_predmeti[i].Ispis();
	}
};

struct Kandidat {
	eNacinStudiranja _nacinStudiranja;
	char * _imePrezime;
	shared_ptr<Uspjeh> _uspjeh[4];

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
		for (size_t i = 0; i < 4; i++) {
			if (_uspjeh[i] != nullptr) {
				_uspjeh[i]->Dealociraj();
				_uspjeh[i].reset();
				_uspjeh[i] = nullptr;
			}
		}
	}
	void Ispis() {
		cout << crt << "Ime i prezime: " << _imePrezime << "\nNacin studiranja: " << nacinStudiranja[_nacinStudiranja];
		cout << "\n--Uspjeh--" << crt;
		for (size_t i = 0; i < 4; i++)
			if (_uspjeh[i] != nullptr)
				_uspjeh[i]->Ispis();
	}

	bool DodajPredmet(eRazred r,Predmet p) {
		if (compareDate(*p._datumUnosa, rokZaPrijavu) == 1)return false;
		int t = -1;
		for (int i = 0; i < 4; i++)
			if (_uspjeh[i] != nullptr && _uspjeh[i]->_razred == r)t = i;
		if (t == -1) 
			for (int i = 0; i < 4; i++) 
				if (_uspjeh[i] == nullptr) {
					_uspjeh[i] = make_shared<Uspjeh>();
					_uspjeh[i]->Unos(r);
					t = i;
					break;
				}
		if (t == -1)return false;
		for (int i = 0; i < _uspjeh[t]->_brojPredmeta; i++)
			if (strcmp(p._naziv, _uspjeh[t]->_predmeti[i]._naziv) == 0)return false;

		if (_uspjeh[t]->_predmeti == nullptr)
			_uspjeh[t]->_predmeti = new Predmet[_uspjeh[t]->_brojPredmeta + 1];
		else {
			Predmet *temp = new Predmet[_uspjeh[t]->_brojPredmeta];
			for (int i = 0; i < _uspjeh[t]->_brojPredmeta; i++)
				swap(_uspjeh[t]->_predmeti[i], temp[i]);
			delete[] _uspjeh[t]->_predmeti; _uspjeh[t]->_predmeti = nullptr;
			_uspjeh[t]->_predmeti = new Predmet[_uspjeh[t]->_brojPredmeta + 1];
			for (int i = 0; i < _uspjeh[t]->_brojPredmeta; i++)
				swap(_uspjeh[t]->_predmeti[i], temp[i]);
			delete[] temp; temp = nullptr;
		}
		_uspjeh[t]->_predmeti[_uspjeh[t]->_brojPredmeta++].Unos(p._naziv, p._ocjena, *p._datumUnosa);
		return true;
	}
};
/*
napisati rekurzivnu funkciju koja ce vratiti pokazivac na kandidata sa najvecom ocjenom na predmetu koji je
proslijedjen kao parametar. ukoliko je vise kandidata ostvarilo istu ocjenu, funkcija treba da vrati onog kandidata
koji je prvi evidentirao tu ocjenu (ako je isto vrijeme evidentiranja, onda funkcija vraca kandidata koji je prvi u nizu).
u slucaju da niti jedan kandidat nije evidentirao trazeni predmet funkcija vraca nullptr. u nastavku je prikazan primjer
poziva rekurzivne funkcije, a ostale parametre dodajte po potrebi.
*/
Kandidat *rekNajboljaOcjena(Kandidat *k, int brk, const char *nazivPr, Kandidat *t, int i=0,int j=0,int z=0,int k2=0,int k3=0) {
	if (i == brk)
		return t;
	if (t == nullptr) {
		k2 = j, k3 = z;
		t = &k[i];
	}
	if (k[i]._uspjeh[j] == nullptr) {
		j++;
		if (j == 4) {
			j = 0;
			i++;
		}
		return rekNajboljaOcjena(k, brk, nazivPr, t, i, j, z, k2, k3);
	}
	if (k[i]._uspjeh[j] != nullptr && strcmp(k[i]._uspjeh[j]->_predmeti[z]._naziv, nazivPr) == 0) {

		if (k[i]._uspjeh[j]->_predmeti[z]._ocjena > t->_uspjeh[k2]->_predmeti[k3]._ocjena) {
			t = &k[i];
			k2 = j, k3 = z;
		}
		if (k[i]._uspjeh[j]->_predmeti[z]._ocjena == t->_uspjeh[k2]->_predmeti[k3]._ocjena && compareDate(*k[i]._uspjeh[j]->_predmeti[z]._datumUnosa, *t->_uspjeh[k2]->_predmeti[k3]._datumUnosa) == -1)
		{
			t = &k[i];
			k2 = j, k3 = z;
		}
	} z++;
		if (z == k[i]._uspjeh[j]->_brojPredmeta) {
			z = 0;
			j++;
		}
		if (j == 4) {
			j = 0;
			i++;
		}
	return rekNajboljaOcjena(k, brk, nazivPr, t, i, j, z, k2, k3);
}
void main()
{
	DatumVrijeme datum19062017_1015, datum20062017_1115, datum30062017_1215, datum05072017_1231;
	datum19062017_1015.Unos(19, 6, 2017, 10, 15);
	datum20062017_1115.Unos(20, 6, 2017, 11, 15);
	datum30062017_1215.Unos(30, 6, 2017, 12, 15);
	datum05072017_1231.Unos(5, 7, 2017, 12, 31);

	cout << datum19062017_1015.GetDatumKaoNizKaraktera() << endl;//9.6.2017 10:15

	Predmet Matematika, Fizika, Hemija, Engleski;
	//2 - ocjena na predmetu; datum - datum evidentiranja uspjeha na predmetu jer postoji krajnji rok za evidentiranje
	Matematika.Unos("Matematika", 2, datum19062017_1015);
	Fizika.Unos("Fizika", 5, datum20062017_1115);
	Hemija.Unos("Hemija", 2, datum20062017_1115);
	Engleski.Unos("Engleski", 5, datum05072017_1231);

	int brojKandidata = 2;

	Kandidat * prijave2017 = new Kandidat[brojKandidata];
	prijave2017[0].Unos(DL, "Jasmin Azemovic");
	prijave2017[1].Unos(REDOVAN, "Indira Hamulic");

	if (prijave2017[0].DodajPredmet(DRUGI, Engleski))//ne bi trebao dodati jer je prosao postavljeni rok za dodavanje predmeta
		cout << "Predmet 1 uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(DRUGI, Matematika))
		cout << "Predmet 2 uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(PRVI, Fizika))
		cout << "Predmet 3 uspjesno dodan!" << crt;
	if (prijave2017[0].DodajPredmet(PRVI, Hemija))
		cout << "Predmet 4 uspjesno dodan!" << crt;

	Matematika._ocjena = 5;
	Hemija._ocjena = 3;

	if (prijave2017[1].DodajPredmet(PRVI, Matematika))
		cout << "Predmet 5 uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(PRVI, Matematika))//ne bi trebalo ponovo dodati Matematiku!
		cout << "Predmet 6 uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(TRECI, Hemija))
		cout << "Predmet 7 uspjesno dodan!" << crt;
	if (prijave2017[1].DodajPredmet(DRUGI, Engleski))
		cout << "Predmet 8 uspjesno dodan!" << crt;

	/*
	koristeci Lambda izraz kreirati funkciju koja ce vratiti uspjeh kandidata koji je ostvario najveci prosjek
	(na nivou razreda, a ne ukupni prosjek). ukoliko vise kandidata ima isti prosjek funkcija vraca uspjeh (najboljeg razreda)
	prvog pronadjenog kandidata
	*/
	auto najboljiUspjeh = [prijave2017,brojKandidata]() {
		float **usp = new float*[brojKandidata];
		for (int i = 0; i < brojKandidata; i++)
			usp[i] = new float[4];
		for(int i=0;i<brojKandidata;i++)
			for (int j = 0; j < 4; j++) {
				if (prijave2017[i]._uspjeh[j] != nullptr) {
					int sum = 0;
					for (int l = 0; l < prijave2017[i]._uspjeh[j]->_brojPredmeta; l++)
						sum += prijave2017[i]._uspjeh[j]->_predmeti[l]._ocjena;
					usp[i][j] = (float)sum / prijave2017[i]._uspjeh[j]->_brojPredmeta;
				}
				else usp[i][j] = 0;
			}
		int naj = usp[0][0], trazenik = 0, trazeniu = 0;
		for(int i=0;i<brojKandidata;i++)
			for(int j=1;j<4;j++)
				if (naj < usp[i][j]) {
					naj = usp[i][j];
					trazeniu = j;
					trazenik = i;
				}
		return prijave2017[trazenik]._uspjeh[trazeniu];
	};
	shared_ptr<Uspjeh> najbolji = najboljiUspjeh();
	if (najbolji != nullptr)
		najbolji->Ispis();

	
	Kandidat * kandidatSaNajboljomOcjenom = rekNajboljaOcjena(prijave2017, brojKandidata, "Matematika",nullptr);
	cout <<"Kandidat sa najboljom ocjenom iz Matematike je "<< kandidatSaNajboljomOcjenom->_imePrezime << endl;

	for (size_t i = 0; i < brojKandidata; i++)
	{
		prijave2017[i].Ispis();
		prijave2017[i].Dealociraj();
	}
	delete[] prijave2017;
	prijave2017 = nullptr;

	system("pause>0");
}