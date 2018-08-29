#include<iostream>
#include <memory>
#include <functional>
#include <cstring>
#include <tuple>
using namespace std;
/*
1. BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA. U SUPROTNOM SE RAD NECE BODOVATI
2. STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
3. KREIRAJTE .DOC FAJL SA VAŠIM BROJEM INDEKSA ( NPR. IB160061.DOC BEZ IMENA I PREZIMENA), TE NA KRAJU ISPITA U NJEGA KOPIRAJTE RJEŠENJA VAŠIH ZADATAKA. NE PREDAVATI .TXT ILI .CPP FAJLOVE
4. TOKOM IZRADE ISPITA NIJE DOZVOLJENO KORIŠTENJE HELP-A
5. TOKOM IZRADE ISPITA MOGU BITI POKRENUTA SAMO TRI PROGRAMA: PDF READER (ISPITNI ZADACI), MS VISUAL STUDIO, MS WORD (U KOJI CETE KOPIRATI VAŠA RJEŠENJA)
6. BEZ OBZIRA NA TO DA LI SU ISPITNI ZADACI URAÐENI, SVI STUDENTI KOJI SU PRISTUPILI ISPITU MORAJU PREDATI SVOJ RAD
*/
//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)


enum VrstaAktivnosti { Tehnike, Kata, Borba };
enum Pojas { Zuti, Narandzasti, Zeleni, Plavi, Smedji, Crni };
const char *vrstaAktivnosti[] = { "Tehnike","Kata","Borba" };
const char *pojas[] = { "Zuti","Narandzasti","Zeleni","Plavi","Smedji","Crni" };
struct Datum {
	int * _dan, *_mjesec, *_godina;
	void Unos(int d, int m, int g) {
		_dan = new int(d);
		_mjesec = new int(m);
		_godina = new int(g);
	}
	void Ispis() { cout << *_dan << "/" << *_mjesec << "/" << *_godina << endl; }
	void Dealociraj() { delete _dan; delete _mjesec; delete _godina; }
	int dani() { return (*_godina * 365 + *_mjesec * 30 + *_dan); };
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
struct Aktivnost {
	unique_ptr<VrstaAktivnosti> _vrsta;
	Datum _datumIzvrsenja;
	char * _nazivOpis;
	int _ocjena; // 1 - 10 
	void Unos(VrstaAktivnosti vrsta, Datum * datum, int ocjena, const char * nazivOpis) {
		_vrsta = make_unique<VrstaAktivnosti>(vrsta);
		_datumIzvrsenja.Unos(*datum->_dan, *datum->_mjesec, *datum->_godina);
		_ocjena = ocjena;
		int size = strlen(nazivOpis) + 1;
		_nazivOpis = new char[size];
		strcpy_s(_nazivOpis, size, nazivOpis);
	}
	void Ispis() {
		cout << "Vrsta aktivnosti: " << vrstaAktivnosti[*_vrsta] << "\nOcjena: " << _ocjena << "\nNaziv: " << _nazivOpis << "\nDatum izvrsenja: ";
		_datumIzvrsenja.Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_vrsta.reset();
		_vrsta = nullptr;
		_datumIzvrsenja.Dealociraj();
		delete[] _nazivOpis; _nazivOpis = nullptr;
	}
};
struct KaratePojas {
	Datum * _datumPolaganja;//datum koji ce se evidentirati kao datum polaganja pojasa tj. kada su ispunjene sve aktivnosti/obaveze
	Pojas _pojas;
	Aktivnost * _listaIzvrsenihAktivnosti;
	int _trenutnoIzvrsenihAktivnosti;
	void Unos(Pojas pojas) {
		_listaIzvrsenihAktivnosti = nullptr;
		_trenutnoIzvrsenihAktivnosti = 0;
		_datumPolaganja = nullptr;
		_pojas = pojas;
	}
	void Dealociraj() {
		if (_datumPolaganja != nullptr) {
			_datumPolaganja->Dealociraj();
			delete _datumPolaganja;
			_datumPolaganja = nullptr;
		}
		if (_trenutnoIzvrsenihAktivnosti > 0) {
			for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
				_listaIzvrsenihAktivnosti[i].Dealociraj();
			delete[] _listaIzvrsenihAktivnosti;
			_listaIzvrsenihAktivnosti = nullptr;
		}
	}
	void Ispis() {
		cout << "Datum polaganja: ";
		if (_datumPolaganja != nullptr)
			_datumPolaganja->Ispis();
		else cout << " *POJAS NIJE POLOZEN*\n";
		cout << "\nPojas: " << pojas[_pojas] << endl;
		if (_trenutnoIzvrsenihAktivnosti > 0)
			for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
				_listaIzvrsenihAktivnosti[i].Ispis();
	}/*Na osnovu vrijednosti primljenog parametra osigurati dodavanje novoizvrsene aktivnosti za potrebe stjecanja odredjenog pojasa. Broj aktivnosti
	 nije ogranicen.
	 Identicna aktivnost se moze dodati jedino u slucaju kada je prethodna (identivna aktivnost po vrsti i datumu izvrsenja) imala ocjenu manju od 6.
	 Uspjesnom aktivnoscu se smatraju one aktivnosti koje imaju ocjenu vecu od 5, a svaka naredna identicna aktivnost, bez obzira da li je uspjesna ili
	 ne,
	 moze biti dodana jedino ako je proslo najmanje 15 dana od izvrsenja prethodne. Onemoguciti dodavanje aktivnosti uspjesno polozenom pojasu.*/

	bool DodajIzvrsenuAktivnost(Aktivnost *a) {
		if (_datumPolaganja != nullptr)return false;
		if (_trenutnoIzvrsenihAktivnosti > 0) {
			for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
				if (compareDate(a->_datumIzvrsenja, _listaIzvrsenihAktivnosti[i]._datumIzvrsenja) == 0 && a->_vrsta == _listaIzvrsenihAktivnosti[i]._vrsta)
					if (a->_ocjena <= _listaIzvrsenihAktivnosti[i]._ocjena)return false;
			if ((a->_datumIzvrsenja.dani() - _listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti - 1]._datumIzvrsenja.dani()) < 15)return false;

			_listaIzvrsenihAktivnosti = expandArray(_listaIzvrsenihAktivnosti, _trenutnoIzvrsenihAktivnosti);
		}
		else _listaIzvrsenihAktivnosti = new Aktivnost[_trenutnoIzvrsenihAktivnosti + 1];
		_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti++].Unos(*a->_vrsta, &a->_datumIzvrsenja, a->_ocjena, a->_nazivOpis);
		return true;
	}
	void Sortiraj() {
		int x = _trenutnoIzvrsenihAktivnosti, iz;
		do {
			iz = 1;
			for (int i = 0; i < x - 1; i++) {
				if (_listaIzvrsenihAktivnosti[i]._ocjena > _listaIzvrsenihAktivnosti[i]._ocjena) {
					swap(_listaIzvrsenihAktivnosti[i], _listaIzvrsenihAktivnosti[i + 1]);
					iz = 0;
				}
			}
			x -= 1;
		} while (x > 1 && iz == 0);
	}
	/* Karate pojas zahtijeva uspjesnu realizaciju svih planiranih aktivnosti, a one se za jedan povecavaju sa svakim novim pojasom, npr.
	zuti pojas: 1 x tehnika, 1 x kata, 1 x borba;	narandzasti pojas: 2 x tehnika, 2 x kata, 2 x borba; i td...
	Funkcija vraca false u slucaju da: su kandidatu u listu aktivnosti evidentirane tri negativno ocijenjene identicne vrste aktivnosti
	(npr. tri negativne ocjene iz borbi), onda se taj pojas ne moze smatrati uspjesno stecenim
	i, te ukoliko nedostaje bilo koja od aktivnosti zahtijevanih u okviru tog pojasa. Ukoliko je kandidat uspjesno realizovao sve aktivnost,
	datum polaganja se postavlja na datum posljednje uspjesno realizovane aktivnosti*/
	bool DaLiJePolozen() {
		if (_trenutnoIzvrsenihAktivnosti == 0)return false;
		int brojac[3] = { 0 }, neg[3] = { 0 };
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
			if (_listaIzvrsenihAktivnosti[i]._ocjena > 5)
				brojac[*_listaIzvrsenihAktivnosti[i]._vrsta]++;
			else neg[*_listaIzvrsenihAktivnosti[i]._vrsta]++;

			int x = (int)_pojas + 1;

			for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
				if (brojac[i] < x)return false;
				if (neg[i] > 2)return false;
			}
			_datumPolaganja = new Datum;
			int posl;
			for (int i = _trenutnoIzvrsenihAktivnosti - 2; i >= 0; i--)
				if (_listaIzvrsenihAktivnosti[i]._ocjena > 5) {
					posl = i;
					break;
				}
			_datumPolaganja->Unos(*_listaIzvrsenihAktivnosti[posl]._datumIzvrsenja._dan, *_listaIzvrsenihAktivnosti[posl]._datumIzvrsenja._mjesec, *_listaIzvrsenihAktivnosti[posl]._datumIzvrsenja._godina);
			return true;
	}
	/*Funkcija vraca prosjecnu ocjenu svih uspjesno realizovanih aktivnosti koja u nazivu ili opisu sadrze vrijednost primljenog parametra. 
	Ukoliko smatrate da je potrebno, mozete dodati i druge parametre za potrebe implementacije ove funkcije*/

	int PretragaRekrzivno(const char *p,int z=0,int i=0,int j=0) {
		if (z == _trenutnoIzvrsenihAktivnosti)return i /= j;
		if (_listaIzvrsenihAktivnosti[z]._ocjena > 5) PretragaRekrzivno(p,z+1, i + _listaIzvrsenihAktivnosti[z]._ocjena, j + 1);
		else PretragaRekrzivno(p, z + 1, i, j);
	}
};

struct Kandidat {
	char * _imePrezime;
	shared_ptr<KaratePojas> _pojasevi[6];
	void Unos(const char * imePrezime) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		for (size_t i = 0; i < 6; i++)
			_pojasevi[i] = nullptr;
	}
	void Dealociraj() {
		delete[] _imePrezime; _imePrezime = nullptr;

		for (size_t i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr) {
				_pojasevi[i]->Dealociraj();
				_pojasevi[i].reset();
				_pojasevi[i] = nullptr;
			}
	}
	void Ispis() {
		cout << "Ime i prezime: " << _imePrezime << endl;
		cout << "---> Lista izvrsenih aktivnosti <----\n";
		for (size_t i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr)
				_pojasevi[i]->Ispis();
	}
	/*
	Karate pojasevi se moraju dodavati po redoslijedu tj. ne smije se dozvoliti dodavanje zelenog pojasa ukoliko prethodno nije dodan zuti i
	narandzasti. Za provjeru lokacije (unutar funkcije DodajPojas) na koju ce se dodati novi karate pojas, te da li su nizi pojasevi prethodno
	dodani koristiti lambda funkciju.
	*/
	bool DodajPojas(KaratePojas kp) {
		if (kp._pojas == 0) {
			_pojasevi[0] = make_shared<KaratePojas>();
			_pojasevi[0]->Unos(kp._pojas);
			for (int i = 0; i < kp._trenutnoIzvrsenihAktivnosti; i++)
				_pojasevi[0]->DodajIzvrsenuAktivnost(&kp._listaIzvrsenihAktivnosti[i]);
			_pojasevi[0]->DaLiJePolozen();
			return true;
		}
		int noReq = kp._pojas;
		noReq -= 1;
		for (int i = 5; i >= 0; i--)
			if (_pojasevi[i]!=nullptr )
				if ((int)_pojasevi[i]->_pojas == noReq)noReq -= 1;

		if (noReq > -1)return false;
		_pojasevi[kp._pojas]=make_shared<KaratePojas>();
		_pojasevi[kp._pojas]->Unos(kp._pojas);
		for (int i = 0; i < kp._trenutnoIzvrsenihAktivnosti; i++)
			_pojasevi[0]->DodajIzvrsenuAktivnost(&kp._listaIzvrsenihAktivnosti[i]);
		_pojasevi[0]->DaLiJePolozen();
		return true;
	}
	//Funkcija GetNajbolji vraca par koji sadrzi oznaku i prosjecnu ocjenu (uspjesno okoncanih aktivnosti) pojasa sa najvecim prosjekom
	pair<Pojas, float>GetNajbolji() {
		int brp = 0;
		for (int i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr)brp++;
		float *prosjeci = new float[brp];
		for (int i = 0; i < brp; i++)prosjeci[i] = 0;
		for (int i = 0; i < brp; i++) {
			for (int j = 0; j < _pojasevi[i]->_trenutnoIzvrsenihAktivnosti; i++)prosjeci[i] += _pojasevi[i]->_listaIzvrsenihAktivnosti[j]._ocjena;
			(float)prosjeci[i] /= _pojasevi[i]->_trenutnoIzvrsenihAktivnosti;
		}
		int naj = 0;
		for (int i = 1; i < brp; i++)if (prosjeci[i] > prosjeci[naj])naj = i;
		return make_pair(_pojasevi[naj]->_pojas, prosjeci[naj]);
	}
};

void main() {

	//BROJ I VRSTA PARAMETARA MORAJU BITI IDENTICNI KAO U PRIMJERIMA
	//STAVITE KOMENTAR NA DIJELOVE CODE-A KOJE NE BUDETE IMPLEMENTIRALI
	Datum datumPolaganja1, datumPolaganja2, datumPolaganja3, datumPolaganja4;
	datumPolaganja1.Unos(10, 6, 2018);
	datumPolaganja2.Unos(18, 6, 2018);
	datumPolaganja3.Unos(22, 3, 2018);
	datumPolaganja4.Unos(22, 7, 2018);

	Aktivnost aktivnost1, aktivnost2, aktivnost3, aktivnost4, aktivnost5;
	aktivnost1.Unos(Tehnike, &datumPolaganja1, 6, "Tehnike za zuti pojas");
	aktivnost2.Unos(Kata, &datumPolaganja1, 8, "Taiki joko shodan - zuti pojas");
	aktivnost3.Unos(Borba, &datumPolaganja1, 2, "Jednostavne borbene tehnike sa partnerom");
	aktivnost4.Unos(Borba, &datumPolaganja1, 6, "Jednostavne borbene tehnike sa partnerom");
	aktivnost5.Unos(Borba, &datumPolaganja4, 6, "Jednostavne borbene tehnike sa partnerom");

	KaratePojas pojasZuti, pojasNarandzasti, pojasZeleni;
	pojasZuti.Unos(Zuti);
	pojasZeleni.Unos(Zeleni);
	pojasNarandzasti.Unos(Narandzasti);

	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost1))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost2))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost3))
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost4))//15 dana...
		cout << "Aktivnost uspjesno dodana!" << endl;
	if (pojasZuti.DodajIzvrsenuAktivnost(&aktivnost5))
		cout << "Aktivnost uspjesno dodana!" << endl;

	//Koristeci neki od obradjenih algoritama, po ocjeni sortirati aktivnosti u okviru odredjenog pojasa
	pojasZuti.Sortiraj();



	if (pojasZuti.DaLiJePolozen())
		pojasZuti.Ispis();

	cout << "Prosjecna ocjena za zuti pojas -> " << pojasZuti.PretragaRekrzivno("pojas") << endl;

	//ispisuje sve dostupne podatke o pojasu
	pojasZuti.Ispis();

	Kandidat jasmin;
	jasmin.Unos("Jasmin Azemovic");

	if (jasmin.DodajPojas(pojasZuti))
		cout << "Pojas uspjesno dodan!" << endl;
	if (jasmin.DodajPojas(pojasZeleni))//prethodno je trebao biti dodan narandzasti pojas
		cout << "Pojas uspjesno dodan!" << endl;
	if (jasmin.DodajPojas(pojasNarandzasti))
		cout << "Pojas uspjesno dodan!" << endl;

	float prosjek = 0;
	Pojas ppojas;
	//Funkcija GetNajbolji vraca par koji sadrzi oznaku i prosjecnu ocjenu (uspjesno okoncanih aktivnosti) pojasa sa najvecim prosjekom
	tie(ppojas, prosjek) = jasmin.GetNajbolji();
	cout << "Najbolji rezultat od " << prosjek << " je ostvaren tokom stjecanja pojasa " << pojas[ppojas] << endl;


	datumPolaganja1.Dealociraj(), datumPolaganja2.Dealociraj(), datumPolaganja3.Dealociraj(), datumPolaganja4.Dealociraj();
	aktivnost1.Dealociraj(), aktivnost2.Dealociraj(), aktivnost3.Dealociraj(), aktivnost4.Dealociraj(), aktivnost5.Dealociraj();
	pojasZuti.Dealociraj(), pojasNarandzasti.Dealociraj(), pojasZeleni.Dealociraj();
	jasmin.Dealociraj();

	system("pause");
}