#include<iostream>
#include<memory>
#include<cstring>
#include <functional>
using namespace std;
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

enum VrstaObaveze { Seminarski, Parcijalni1, Parcijalni2, Integralni, Prakticni };
char *vrstaObaveze[] = { "Seminarski","Parcijalni 1","Parcijalni 2","Integralni","Prakticni" };
struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() {
		cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl;
	}
	void Dealociraj() { if(_dan!=nullptr)delete _dan; 
	if(_mjesec!=nullptr)delete _mjesec; 
	if(_godina!=nullptr)delete _godina;
	_dan = _mjesec = _godina = nullptr;
	}
	int dani() { return *_dan + *_mjesec * 30 + *_godina * 365; }
};
struct ObavezeNaPredmetu {
	shared_ptr<VrstaObaveze> _vrstaObaveze;
	Datum _datumIzvrsenja;
	char * _napomena;
	int _ocjena; // 5 - 10 
	void Unos(VrstaObaveze vrsta, Datum datum, int ocjena, char * napomena) {
		if (ocjena < 5 || ocjena>10)
		{
			cout << "Neispravan format ocjene..\n";
			return;
		}
		if (napomena == nullptr) {
			cout << "Morate unijeti napomenu..\n";
			return;
		}
		_vrstaObaveze = make_shared<VrstaObaveze>(vrsta);
		_datumIzvrsenja.Unos(*datum._dan, *datum._mjesec, *datum._godina);
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
		_datumIzvrsenja.Dealociraj();
		if (_napomena != nullptr)
			delete[] _napomena;
		_napomena = nullptr;
	}
};

char * _kljucneRijeci[15] = { NULL }; /*da bi se odredjena obaveza na predmetu smatrala uspjesno izvrsenom, u napomeni (objekta ObavezeNaPredmetu)
									  ne smije stajati niti jedna od rijeci koja se nalazi u nizu _kljucneRijeci*/
bool zabranaNapomena(const char *napomena=nullptr) {
	if (napomena == nullptr)
		return false;
	for(int i=0;i<15;i++)
		if (_kljucneRijeci[i] != NULL) {
			char *p = strstr((char*)napomena, _kljucneRijeci[i]);
			if (p != NULL)
				return true;
		}
	return false;
}
struct PolozeniPredmet {
	Datum _datumPolaganja;//datum koji ce se evidentirati kao datum kada je predmet polozen tj. kada je formirana konacna ocjena
	char * _nazivPredmeta;
	ObavezeNaPredmetu * _listaIzvrsenihObaveza;
	int _trenutnoIzvrsenihObaveza;
	int _minimalanBrojDana; //odnosi se na minimalan broj dana koji mora proci od izvrsenja pojedinih obaveza na predmetu (npr. 7 dana)
	int _konacnaOcjena; //formira se na osnovu ocjena izvrsenih obaveza

	void Unos(char * naziv, int minimalanBrojDana) {
		if (naziv == nullptr || minimalanBrojDana<0)
			return;
		_datumPolaganja = { 0,0,0 };
		_nazivPredmeta = new char[strlen(naziv) + 1];
		strcpy_s(_nazivPredmeta, strlen(naziv) + 1, naziv);
		_listaIzvrsenihObaveza = nullptr;
		_trenutnoIzvrsenihObaveza = 0;
		_minimalanBrojDana = minimalanBrojDana;
		_konacnaOcjena = 0;
	}
	void Dealociraj() {
			_datumPolaganja.Dealociraj();
		if (_nazivPredmeta != nullptr)
			delete[] _nazivPredmeta;
		_nazivPredmeta = nullptr;
		if (_trenutnoIzvrsenihObaveza > 0 && _listaIzvrsenihObaveza!=nullptr)
			for (size_t i = 0; i < _trenutnoIzvrsenihObaveza; i++)
				_listaIzvrsenihObaveza[i].Dealociraj();
		if (_listaIzvrsenihObaveza != nullptr)
			delete[]_listaIzvrsenihObaveza;
		_listaIzvrsenihObaveza = nullptr;
	}
	/*Na osnovu vrijednosti primljenih parametara osigurati dodavanje novoizvrsene obaveze na predmetu. Potrebno je onemoguciti dodavanje 
	identicnih obaveza, a izmedju izvrsenja pojedinih obaveza mora proci najmanje onoliko dana koliko je definisano vrijednoscu atributa 
	_minimalanBrojDana. Identicna vrsta obaveze se moze dodati samo u slucaju da je prethodno dodana obaveza (identicne vrste) imala ocjenu 5
	ili je u napomeni sadrzavala neku od zabranjenih rijeci (definisanih nizom _kljucneRijeci).*/


	bool DodajIzvrsenuObavezu(VrstaObaveze v, Datum d, int ocj, const char *napomena) {
		if (ocj < 5 || ocj>10) {
			cout << "Neispravan format ocjene..\n";
			return false;
		}
		if (_trenutnoIzvrsenihObaveza > 0 && _listaIzvrsenihObaveza != nullptr) {
			for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
				if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == v && _listaIzvrsenihObaveza[i]._ocjena == ocj && (_listaIzvrsenihObaveza[i]._datumIzvrsenja.dani() - d.dani()) == 0)
					return false;
			for (int i = _trenutnoIzvrsenihObaveza - 1; i >= 0; i--)
				if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == v && _listaIzvrsenihObaveza[i]._ocjena > 5 && *_listaIzvrsenihObaveza[i]._vrstaObaveze!=Seminarski)
					return false;
				else if(*_listaIzvrsenihObaveza[i]._vrstaObaveze == v && *_listaIzvrsenihObaveza[i]._vrstaObaveze != Seminarski){
					if (!zabranaNapomena(_listaIzvrsenihObaveza[i]._napomena))
						return false;
				}
				if ((d.dani() - _listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza - 1]._datumIzvrsenja.dani()) < _minimalanBrojDana)
					return false;
				ObavezeNaPredmetu *temp = new ObavezeNaPredmetu[_trenutnoIzvrsenihObaveza];
				for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
					swap(_listaIzvrsenihObaveza[i], temp[i]);
				delete[] _listaIzvrsenihObaveza;
				_listaIzvrsenihObaveza = new ObavezeNaPredmetu[_trenutnoIzvrsenihObaveza + 1];
				for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
					swap(_listaIzvrsenihObaveza[i], temp[i]);
				delete[] temp; temp = nullptr;
		}
		else if (_listaIzvrsenihObaveza == nullptr) 
			_listaIzvrsenihObaveza = new ObavezeNaPredmetu[_trenutnoIzvrsenihObaveza + 1];
		
		_listaIzvrsenihObaveza[_trenutnoIzvrsenihObaveza++].Unos(v, d, ocj, (char*)napomena);
		return true;
	}
	/*Konacna ocjene predstavlja prosjecnu ocjenu na predmetu (prosjecnu ocjenu izvrsenih obaveza koje ispunjavaju uslove za uspjesno polaganje
	predmeta),a za njeno formiranje student mora posjedovati polozen integralni ili dva parijcalna ispita. Ukoliko je ispit polozen putem 
	parcijalnih ispita, student takodjer mora imati pozitivno (ocjenom vecom od 5) ocijenjena najmanje dva seminarska i jedan prakticni rad. 
	Takodjer, datum polaganja drugog parcijalnog ispita ne smije biti raniji (manji) od datuma polaganja prvog parcijalnog ispita. 
	Konacna ocjena ne smije biti formirana u slucaju da napomena bilo koje odbaveze (koja je ucestvovala u formiranju konacne ocjene)
	sadrzi neku od zabranjenih rijeci (niz _kljucneRijeci). U slucaju da neki od navedenih uslova nije zadovoljen konacna ocjena treba
	biti postavljena na vrijednost 5. Ukoliko su ispunjeni svi uslovi, osim formiranja konacne ocjene, datum polaganja je potrebno postaviti
	na vrijednost datuma posljednje izvrsene obaveze sa najvecom ocjenom.
	*/
	int FormirajKonacnuOcjenu() {
		if (_trenutnoIzvrsenihObaveza <= 0 || _listaIzvrsenihObaveza == nullptr)
			return 5;
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
			if (_listaIzvrsenihObaveza[i]._ocjena > 5) {
				if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Integralni) {
					if (zabranaNapomena(_listaIzvrsenihObaveza[i]._napomena))
						return 5;
					else
						return _listaIzvrsenihObaveza[i]._ocjena;
				}
			}
		ObavezeNaPredmetu *zahtjevaneObaveze[5]{ NULL };
		for (int i = 0; i < _trenutnoIzvrsenihObaveza; i++)
			if (_listaIzvrsenihObaveza[i]._ocjena > 5) {
				if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Parcijalni1)
					zahtjevaneObaveze[0] = &_listaIzvrsenihObaveza[i];
				else if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Parcijalni2)
					zahtjevaneObaveze[1] = &_listaIzvrsenihObaveza[i];
				else if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Seminarski)
					if (zahtjevaneObaveze[2] == NULL)
						zahtjevaneObaveze[2] = &_listaIzvrsenihObaveza[i];
					else
						zahtjevaneObaveze[3] = &_listaIzvrsenihObaveza[i];
				else if (*_listaIzvrsenihObaveza[i]._vrstaObaveze == Prakticni)
					zahtjevaneObaveze[4] = &_listaIzvrsenihObaveza[i];
			}
		for (int i = 0; i < 5; i++)
			if (zahtjevaneObaveze[i] == NULL)
				return 5;
			else if (zabranaNapomena(zahtjevaneObaveze[i]->_napomena))
				return 5;

		if ((zahtjevaneObaveze[1]->_datumIzvrsenja.dani() - zahtjevaneObaveze[0]->_datumIzvrsenja.dani()) <= 0)
			return 5;

		int naj = 0;
		for (int i = 1; i < _trenutnoIzvrsenihObaveza; i++)
			if (_listaIzvrsenihObaveza[i]._ocjena >= _listaIzvrsenihObaveza[naj]._ocjena)
				naj = i;
		_datumPolaganja.Unos(*_listaIzvrsenihObaveza[naj]._datumIzvrsenja._dan, *_listaIzvrsenihObaveza[naj]._datumIzvrsenja._mjesec, *_listaIzvrsenihObaveza[naj]._datumIzvrsenja._godina);
		float prosjOcj = 0;
		for (int i = 0; i < 5; i++)
			prosjOcj += zahtjevaneObaveze[i]->_ocjena;
		prosjOcj /= 5;
		if (prosjOcj - (int)prosjOcj > 0)
			return (int)prosjOcj + 1;
		else
			return (int)prosjOcj;
	}
};
/*Rekurzivna funkcija treba vratiti prosjecnu ocjenu uspjesno izvrsenih obaveze na predmetu odredjene vrste u definisanom periodu.
Izbjegavati koristenje globalnih varijabli*/

float PretragaRekurzivno(PolozeniPredmet pp,VrstaObaveze v,Datum OD, Datum DO,int i,int j,int brojac=0) {
	if (pp._trenutnoIzvrsenihObaveza <= 0 || pp._listaIzvrsenihObaveza == nullptr)
		return 0;
	if (i == pp._trenutnoIzvrsenihObaveza) 
		return j / brojac;
	
	int uslov1 = pp._listaIzvrsenihObaveza[i]._datumIzvrsenja.dani() - OD.dani(), uslov2 = DO.dani() - pp._listaIzvrsenihObaveza[i]._datumIzvrsenja.dani();
	if (uslov1 >= 0 && uslov2 >= 0)
		if (pp._listaIzvrsenihObaveza[i]._ocjena > 5)
			return PretragaRekurzivno(pp, v, OD, DO, i +1, j += pp._listaIzvrsenihObaveza[i]._ocjena,brojac+1);
		else return PretragaRekurzivno(pp, v, OD, DO, i+1,j,brojac);
	else return PretragaRekurzivno(pp, v, OD, DO, i + 1, j, brojac);
}
/*Koristeci lambda-u, unutar funkcije GetBrojObavezaIznadProsjeka pronaci broj izvrsenih obaveza koje imaju istu ili vecu ocjenu od prosjecne*/

int GetBrojObavezaIznadProsjeka(PolozeniPredmet pp, float prosjek) {
	if (prosjek <= 0 && pp._trenutnoIzvrsenihObaveza>=0)
		return pp._trenutnoIzvrsenihObaveza;
	if (pp._trenutnoIzvrsenihObaveza <= 0 || pp._listaIzvrsenihObaveza == nullptr)
		return 0;
	auto f = [=]() {
		int broj = 0;
		for (int i = 0; i < pp._trenutnoIzvrsenihObaveza; i++)
			if (pp._listaIzvrsenihObaveza[i]._ocjena >= prosjek)
				broj++;
		return broj;
	};
	return f();
}
void main() {
	_kljucneRijeci[0] = "prepisivao";
	_kljucneRijeci[1] = "ometao";
	_kljucneRijeci[2] = "nije dostavio";

	Datum datumSeminarski1, datumSeminarski2, datumParcijalni1, datumParcijalni2, datumIntegralni, datumPrakticni;
	datumSeminarski1.Unos(17, 7, 2017);
	datumSeminarski2.Unos(27, 7, 2017);
	datumParcijalni1.Unos(22, 6, 2017);
	datumParcijalni2.Unos(30, 6, 2017);
	datumPrakticni.Unos(7, 8, 2017);

	PolozeniPredmet prII;
	//naziv predmeta, minimalan broj dana
	prII.Unos("Programiranje II", 7);

	if (prII.DodajIzvrsenuObavezu(Parcijalni1, datumParcijalni1, 6, "uslovno polozen ispit"))
		cout << "Parcijalni 1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Parcijalni2, datumParcijalni2, 6, "previse gresaka, a tokom ispita  druge studente"))
		cout << "Parcijalni 2...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski1, 8, "implementirani svi algoritmi"))
		cout << "Seminarski1...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Seminarski, datumSeminarski2, 7, "rad slican kao kod studenta IB150388"))
		cout << "Seminarski2...dodan" << endl;
	if (prII.DodajIzvrsenuObavezu(Prakticni, datumPrakticni, 6, "fino odradjen prakticni"))
		cout << "Prakticni...dodan\n";

	cout << "Konacna ocjena iz predmeta PRII je: " << prII.FormirajKonacnuOcjenu() << endl;
	Datum OD, DO; OD.Unos(1, 1, 2017); DO.Unos(1, 7, 2017);
	

	
	float prosjek = PretragaRekurzivno(prII, Seminarski, OD, DO, 0, 0);
	cout << "Prosjecna ocjena izvrsenih obaveza je: " << prosjek << endl;

	int iznadProsjeka = GetBrojObavezaIznadProsjeka(prII, prosjek);
	cout << "Broj izvrsenih obaveza iznad prosjeka je: " << iznadProsjeka << endl;

	prII.Dealociraj();
	system("PAUSE");
}
