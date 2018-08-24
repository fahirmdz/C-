#include<iostream>
#include <memory>
#include <cstring>
#include <iomanip>
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NECE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/

using namespace std;
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum VrstaObaveze { Seminarski, Parcijalni1, Parcijalni2, Integralni, Prakticni };
const char *vrstaObaveze[] = { "Seminarski","Parcijalni 1","Parcijalni 2","Integralni","Prakticni" };
struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() {cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl;}
	void Dealociraj() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
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
struct ObavezeNaPredmetu {
	shared_ptr<VrstaObaveze> _vrstaObaveze;
	Datum _datumIzvrsenja;
	char * _napomena;
	int _ocjena; // 5 - 10 
	void Unos(VrstaObaveze vrsta, Datum datum, int ocjena, char * napomena) {
		_vrstaObaveze = make_shared<VrstaObaveze>(vrsta);
		_datumIzvrsenja = datum;
		_ocjena = ocjena;
		_napomena = new char[strlen(napomena) + 1];
		strcpy_s(_napomena, strlen(napomena) + 1, napomena);
	}
	void Ispis() {
		cout << "Vrsta obaveze: " << vrstaObaveze[*_vrstaObaveze] << "\nOcjena: " << _ocjena << "\nNapomena: " << _napomena << "\nDatum izvrsenja: ";
		_datumIzvrsenja.Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_vrstaObaveze.reset();
		_vrstaObaveze = nullptr;
		_datumIzvrsenja.Dealociraj();
		delete[] _napomena; _napomena = nullptr;
	}
};

char * _kljucneRijeci[15] = { NULL }; /*da bi se odredjena obaveza na predmetu smatrala uspjesno izvrsenom, u napomeni 
									  (objekta ObavezeNaPredmetu) ne smije stajati niti jedna od rijeci koja se nalazi u nizu _kljucneRijeci*/

struct PolozeniPredmet {
	Datum _datumPolaganja;//datum koji ce se evidentirati kao datum kada je predmet polozen tj. kada je formirana konacna ocjena
	char * _nazivPredmeta;
	ObavezeNaPredmetu * _listaIzvrsenihObaveza;
	int _trenutnoIzvrsenihObaveza;
	int _minimalanBrojDana; //odnosi se na minimalan broj dana koji mora proci od izvrsenja pojedinih obaveza na predmetu (npr. 7 dana)
	int _konacnaOcjena; //formira se na osnovu ocjena izvrsenih obaveza

	void Unos(char * naziv, int minimalanBrojDana) {
		_nazivPredmeta = new char[strlen(naziv) + 1];
		strcpy_s(_nazivPredmeta, strlen(naziv) + 1, naziv);
		_listaIzvrsenihObaveza = nullptr;
		_trenutnoIzvrsenihObaveza = 0;
		_minimalanBrojDana = minimalanBrojDana;
		_konacnaOcjena = 0;
	}
	void Dealociraj() {
		_datumPolaganja.Dealociraj();
		delete[] _nazivPredmeta; _nazivPredmeta = nullptr;
		if (_trenutnoIzvrsenihObaveza > 0)
			for (size_t i = 0; i < _trenutnoIzvrsenihObaveza; i++)
				_listaIzvrsenihObaveza[i].Dealociraj();
		delete[]_listaIzvrsenihObaveza;
	}  	/*Na osnovu vrijednosti primljenih parametara osigurati dodavanje novoizvrsene obaveze na predmetu. 
		Potrebno je onemoguciti dodavanje identicnih obaveza, a izmedju izvrsenja pojedinih obaveza mora proci najmanje onoliko 
		dana koliko je definisano vrijednoscu atributa _minimalanBrojDana. Identicna vrsta obaveze se moze dodati samo u slucaju da 
		je prethodno dodana obaveza (identicne vrste) imala ocjenu 5 ili je u napomeni sadrzavala neku od zabranjenih rijeci 
		(definisanih nizom _kljucneRijeci).*/

	bool DodajIzvrsenuObavezu(VrstaObaveze v, Datum d, int ocj, const char *napom) {
		if (_trenutnoIzvrsenihObaveza > 0) {
			for (int i = _trenutnoIzvrsenihObaveza - 1; i >= 0; i--)
				if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == v && _listaIzvrsenihObaveza[i]._ocjena == ocj && compareDate(_listaIzvrsenihObaveza[i]._datumIzvrsenja, d) == 0 && strcmp(napom, _listaIzvrsenihObaveza[i]._napomena) == 0)
					return false;
			bool flag = true;
			for (int i = _trenutnoIzvrsenihObaveza - 1; i >= 0; i--)
				if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == v && *_listaIzvrsenihObaveza[i]._vrstaObaveze!=Seminarski) {
					if (_listaIzvrsenihObaveza[i]._ocjena != 5) {
						flag = false;
						break;
					}
					int x = 0, kr = 0;
					while (_kljucneRijeci[x] != NULL) {
						char *p = strstr(_listaIzvrsenihObaveza[i]._napomena, _kljucneRijeci[x]);
						if (p != NULL)kr = 1;
						x++;
					}
					if (kr != 1) {
						flag = false;
						break;
					}
				}
			if (!flag)return false;
			if ((d.dani() - _listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza - 1]._datumIzvrsenja.dani()) < _minimalanBrojDana)return false;
		}
		if (_trenutnoIzvrsenihObaveza == 0)_listaIzvrsenihObaveza = new ObavezeNaPredmetu[_trenutnoIzvrsenihObaveza + 1];
		else {
			ObavezeNaPredmetu *temp = new ObavezeNaPredmetu[_trenutnoIzvrsenihObaveza];
			for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
				swap(temp[i], _listaIzvrsenihObaveza[i]);
			delete[] _listaIzvrsenihObaveza;
			_listaIzvrsenihObaveza = nullptr;
			_listaIzvrsenihObaveza = new ObavezeNaPredmetu[_trenutnoIzvrsenihObaveza + 1];
			for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
				swap(temp[i], _listaIzvrsenihObaveza[i]);
			delete[] temp;
			temp = nullptr;
		}
		_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza++].Unos(v, d, ocj, (char*)napom);
		return true;
	}
	/*Konacna ocjene predstavlja prosjecnu ocjenu na predmetu (prosjecnu ocjenu izvrsenih obaveza koje ispunjavaju uslove za 
	uspjesno polaganje predmeta),a za njeno formiranje student mora posjedovati polozen integralni ili dva parijcalna ispita. 
	Ukoliko je ispit polozen putem parcijalnih ispita, student takodjer mora imati pozitivno (ocjenom vecom od 5) ocijenjena najmanje
	dva seminarska i jedan prakticni rad. Takodjer, datum polaganja drugog parcijalnog ispita ne smije biti raniji (manji) od datuma
	polaganja prvog parcijalnog ispita. Konacna ocjena ne smije biti formirana u slucaju da napomena bilo koje odbaveze
	(koja je ucestvovala u formiranju konacne ocjene) sadrzi neku od zabranjenih rijeci (niz _kljucneRijeci). 
	U slucaju da neki od navedenih uslova nije zadovoljen konacna ocjena treba biti postavljena na vrijednost 5. 
	Ukoliko su ispunjeni svi uslovi, osim formiranja konacne ocjene, datum polaganja je potrebno postaviti na vrijednost
	datuma posljednje izvrsene obaveze sa najvecom ocjenom.
	*/

	int FormirajKonacnuOcjenu() {

		if (_trenutnoIzvrsenihObaveza == 0)return 5;
		int l = _trenutnoIzvrsenihObaveza - 1;
		for (int i = _trenutnoIzvrsenihObaveza - 1; i >= 0; i--)
			if (_listaIzvrsenihObaveza[i]._ocjena > _listaIzvrsenihObaveza[l]._ocjena)
				l = i;
		_datumPolaganja.Unos(*_listaIzvrsenihObaveza[l]._datumIzvrsenja._dan, *_listaIzvrsenihObaveza[l]._datumIzvrsenja._mjesec, *_listaIzvrsenihObaveza[l]._datumIzvrsenja._godina);
		ObavezeNaPredmetu *p1 = nullptr, *p2 = nullptr, *s1 = nullptr, *s2 = nullptr, *prakt = nullptr;
		for (int i = _trenutnoIzvrsenihObaveza-1; i>=0; i--) {
			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Integralni) {
				int x = 0;
				while (_kljucneRijeci[x] != NULL) {
					char *k = strstr(_listaIzvrsenihObaveza[i]._napomena, _kljucneRijeci[x]);
					if (k != NULL)return 5;
					x++;
				}
				return _listaIzvrsenihObaveza[i]._ocjena;

			}
			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Parcijalni2 && p2==nullptr)	p2 = &_listaIzvrsenihObaveza[i];
			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Parcijalni1 && p1 == nullptr)p1 = &_listaIzvrsenihObaveza[i];
			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Prakticni && prakt == nullptr)prakt = &_listaIzvrsenihObaveza[i];
			if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Seminarski && s1 == nullptr)s1 = &_listaIzvrsenihObaveza[i];
			else if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Seminarski && s2 == nullptr)s2 = &_listaIzvrsenihObaveza[i];
			
		}
		if (p1 != nullptr && p2 != nullptr) {
			int condition = compareDate(p1->_datumIzvrsenja, p2->_datumIzvrsenja);
			if (condition == 0 || condition == 1)return false;
			if (s1 == nullptr || s2 == nullptr || prakt == nullptr)return false;
			int x = 0;
			if (p1->_ocjena == 5 || p2->_ocjena == 5 || s1->_ocjena == 5 || s2->_ocjena == 5 || prakt->_ocjena == 5)return 5;
			while (_kljucneRijeci[x] != NULL) {
				char *k = strstr(s1->_napomena, _kljucneRijeci[x]), *k1 = strstr(s2->_napomena, _kljucneRijeci[x]), *k2 = strstr(prakt->_napomena, _kljucneRijeci[x]);
				char *k4 = strstr(p1->_napomena, _kljucneRijeci[x]), *k5 = strstr(p2->_napomena, _kljucneRijeci[x]);
				if (k != NULL || k1 != NULL || k2 != NULL || k4 != NULL || k5 != NULL)return 5;
				x++;
			}

			float ocj = 0;
			int konacna;
			ocj += p1->_ocjena + p2->_ocjena + s1->_ocjena + s2->_ocjena + prakt->_ocjena;
			ocj /= 5;
			float ost = ocj - (int)ocj;
			if (ost >= 0.5) konacna = (int)ocj + 1;
			else konacna = (int)ocj;
			return konacna;
		}
		return 5;
	}
};
/*Rekurzivna funkcija treba vratiti prosjecnu ocjenu uspjesno izvrsenih obaveze na predmetu odredjene vrste u definisanom periodu.
Izbjegavati koristenje globalnih varijabli*/
float PretragaRekurzivno(PolozeniPredmet pp, VrstaObaveze v, Datum OD, Datum DO, int i, int j, float ocj = 0) {
	if (i == pp._trenutnoIzvrsenihObaveza)return (float)ocj / j;

	if (*pp._listaIzvrsenihObaveza[i]._vrstaObaveze == v) {
		int odd = compareDate(OD, pp._listaIzvrsenihObaveza[i]._datumIzvrsenja), doo = compareDate(DO, pp._listaIzvrsenihObaveza[i]._datumIzvrsenja);
		if ((odd==-1 && doo==1) || (odd==0 && doo==1) || (odd==-1 && doo==0)) {
			j++;
			ocj += pp._listaIzvrsenihObaveza[i]._ocjena;
		}
	}
	return PretragaRekurzivno(pp, v, OD, DO, i + 1, j, ocj);
}
void main() {
	_kljucneRijeci[0] = "prepisivao";
	_kljucneRijeci[1] = "ometao";
	_kljucneRijeci[2] = "nije dostavio";

	Datum datumSeminarski1, datumSeminarski2, datumParcijalni1, datumParcijalni2, datumIntegralni, datumPrakticni;
	datumSeminarski1.Unos(7, 7, 2017);
	datumSeminarski2.Unos(18, 7, 2017);
	datumParcijalni1.Unos(22, 6, 2017);
	datumParcijalni2.Unos(30, 6, 2017);
	datumPrakticni.Unos(25, 7, 2017);

	PolozeniPredmet prII;
	//naziv predmeta, minimalan broj dana
	prII.Unos("Programiranje II", 7);

	/*Na osnovu vrijednosti primljenih parametara osigurati dodavanje novoizvrsene obaveze na predmetu. Potrebno je onemoguciti dodavanje identicnih obaveza, a izmedju izvrsenja pojedinih obaveza mora proci najmanje onoliko dana koliko je definisano vrijednoscu atributa _minimalanBrojDana. Identicna vrsta obaveze se moze dodati samo u slucaju da je prethodno dodana obaveza (identicne vrste) imala ocjenu 5 ili je u napomeni sadrzavala neku od zabranjenih rijeci (definisanih nizom _kljucneRijeci).*/
	if (prII.DodajIzvrsenuObavezu(Parcijalni1, datumParcijalni1, 6, "uslovno polozen ispit"))
		cout << "Parcijalni 1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Parcijalni2, datumParcijalni2, 6, "previse gresaka, a tokom ispita druge studente"))
		cout << "Parcijalni 2...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski1, 8, "implementirani svi algoritmi"))
		cout << "Seminarski1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski2, 10, "rad dobar"))
		cout << "Seminarski2...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Prakticni, datumPrakticni, 7, "dobar momak"))
	cout << "Prakticni...dodan\n";
	/*Konacna ocjene predstavlja prosjecnu ocjenu na predmetu (prosjecnu ocjenu izvrsenih obaveza koje ispunjavaju uslove za uspjesno polaganje predmeta),a za njeno formiranje student mora posjedovati polozen integralni ili dva parijcalna ispita. Ukoliko je ispit polozen putem parcijalnih ispita, student takodjer mora imati pozitivno (ocjenom vecom od 5) ocijenjena najmanje dva seminarska i jedan prakticni rad. Takodjer, datum polaganja drugog parcijalnog ispita ne smije biti raniji (manji) od datuma polaganja prvog parcijalnog ispita. Konacna ocjena ne smije biti formirana u slucaju da napomena bilo koje odbaveze (koja je ucestvovala u formiranju konacne ocjene) sadrzi neku od zabranjenih rijeci (niz _kljucneRijeci). U slucaju da neki od navedenih uslova nije zadovoljen konacna ocjena treba biti postavljena na vrijednost 5. Ukoliko su ispunjeni svi uslovi, osim formiranja konacne ocjene, datum polaganja je potrebno postaviti na vrijednost datuma posljednje izvrsene obaveze sa najvecom ocjenom.
	*/
	cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;

	datumParcijalni2.Unos(22, 7, 2017);
	if (prII.DodajIzvrsenuObavezu(Parcijalni2, datumParcijalni2, 6, ""))
		cout << "Parcijalni 2...dodan" << endl;
	cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;
	Datum OD, DO; OD.Unos(1, 1, 2017); DO.Unos(1, 8, 2017);

	/*Rekurzivna funkcija treba vratiti prosjecnu ocjenu uspjesno izvrsenih obaveze na predmetu odredjene vrste u definisanom periodu. 
	Izbjegavati koristenje globalnih varijabli*/
	float prosjek = PretragaRekurzivno(prII, Seminarski, OD, DO, 0, 0);
	cout << "Prosjecna ocjena izvrsenih obaveza je: " << prosjek << endl;
	auto GetBrojObavezaIznadProsjeka = [](PolozeniPredmet pp,float pros) {
		int br = 0;
		for (int i = 0; i < pp._trenutnoIzvrsenihObaveza; i++)
			if (pp._listaIzvrsenihObaveza[i]._ocjena >= pros)br++;
		return br;
	};
	/*Koristeci lambda-u, unutar funkcije GetBrojObavezaIznadProsjeka pronaci broj izvrsenih obaveza koje imaju istu ili vecu ocjenu od prosjecne*/
	int iznadProsjeka = GetBrojObavezaIznadProsjeka(prII, prosjek);
	cout << "Broj izvrsenih obaveza iznad prosjeka je: " << iznadProsjeka << endl;

	
	prII.Dealociraj();
}