#include<iostream>
#include <memory>
#include <functional>
#include <cstring>
#include <tuple>
using namespace std;

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
	void Dealociraj() { if(_dan!=nullptr)delete _dan;
	if(_mjesec!=nullptr)delete _mjesec;
	if(_godina!=nullptr)delete _godina;
	_dan = _godina = _mjesec = nullptr;
	}
	int dani() { return *_dan + *_mjesec * 30 + *_godina * 365; }
};
int compareDate(Datum d1, Datum d2) {
	if (*d1._godina > *d2._godina)
		return 1;
	else if (*d1._godina < *d2._godina)
		return -1;
	else if (*d1._mjesec > *d2._mjesec)
		return 1;
	else if (*d1._mjesec < *d2._mjesec)
		return -1;
	else if (*d1._dan > *d2._dan)
		return 1;
	else if (*d1._dan < *d2._dan)
		return -1;
	else return 0;
}
struct Aktivnost {
	unique_ptr<VrstaAktivnosti> _vrsta;
	Datum _datumIzvrsenja;
	char * _nazivOpis;
	int _ocjena; // 1 - 10 
	void Unos(VrstaAktivnosti vrsta, Datum * datum, int ocjena, const char * nazivOpis) {
		if (ocjena == NULL || nazivOpis == nullptr || datum==nullptr)
			return;
		_vrsta = make_unique<VrstaAktivnosti>(vrsta);
		_datumIzvrsenja.Unos(*datum->_dan, *datum->_mjesec, *datum->_godina);
		_ocjena = ocjena;
		int size = strlen(nazivOpis) + 1;
		_nazivOpis = new char[size];
		strcpy_s(_nazivOpis, size, nazivOpis);
	}
	void Ispis() {
		if (_ocjena == NULL || _nazivOpis == nullptr)
			return;
		cout << vrstaAktivnosti[*_vrsta] << " " << _ocjena << " " << _nazivOpis;
		_datumIzvrsenja.Ispis();
		cout << endl;
	}
	void Dealociraj() {
		_datumIzvrsenja.Dealociraj();
		if (_nazivOpis != nullptr)
			delete[] _nazivOpis;
		_nazivOpis = nullptr;
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
		if (_datumPolaganja != nullptr)
			_datumPolaganja->Dealociraj();
		_datumPolaganja = nullptr;
		if (_trenutnoIzvrsenihAktivnosti > 0)
			for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
				_listaIzvrsenihAktivnosti[i].Dealociraj();
		if (_listaIzvrsenihAktivnosti != nullptr)
			delete[] _listaIzvrsenihAktivnosti;
		_listaIzvrsenihAktivnosti = nullptr;
	}
	void Ispis() {
		_datumPolaganja->Ispis();
		cout << pojas[_pojas] << endl;
		if (_trenutnoIzvrsenihAktivnosti > 0)
			for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
				_listaIzvrsenihAktivnosti[i].Ispis();
		else
			cout << "Nema izvrsenih aktivmnosti..\n";
	}
	/*Na osnovu vrijednosti primljenog parametra osigurati dodavanje novoizvrsene aktivnosti za potrebe stjecanja odredjenog pojasa. 
	Broj aktivnosti nije ogranicen.
	Identicna aktivnost se moze dodati jedino u slucaju kada je prethodna (identivna aktivnost po vrsti i datumu izvrsenja) imala ocjenu manju od 6.
	Uspjesnom aktivnoscu se smatraju one aktivnosti koje imaju ocjenu vecu od 5, a svaka naredna identicna aktivnost, bez obzira da li je uspjesna ili
	ne,
	moze biti dodana jedino ako je proslo najmanje 15 dana od izvrsenja prethodne. Onemoguciti dodavanje aktivnosti uspjesno polozenom pojasu.*/
	bool DodajIzvrsenuAktivnost(Aktivnost *a) {
		if (_datumPolaganja != nullptr) {
			cout << "Ne mozete dodavati aktivnosti uspjesno polozenom pojasu!\n";
			return false;
		}
		if (_trenutnoIzvrsenihAktivnosti < 0)
			return false;
		if (_trenutnoIzvrsenihAktivnosti > 0) {
			for (int i = _trenutnoIzvrsenihAktivnosti - 1; i >= 0; i--) {
				if (*_listaIzvrsenihAktivnosti[i]._vrsta == *a->_vrsta)
					if (_listaIzvrsenihAktivnosti[i]._ocjena > 5)
						return false;
					else if ((a->_datumIzvrsenja.dani() - _listaIzvrsenihAktivnosti[i]._datumIzvrsenja.dani()) < 15)
						return false;
					else
						break;
			}
			Aktivnost *temp = new Aktivnost[_trenutnoIzvrsenihAktivnosti];
			for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
				swap(_listaIzvrsenihAktivnosti[i], temp[i]);
			delete[] _listaIzvrsenihAktivnosti;
			_listaIzvrsenihAktivnosti = new Aktivnost[_trenutnoIzvrsenihAktivnosti + 1];
			for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++)
				swap(_listaIzvrsenihAktivnosti[i], temp[i]);
			delete[] temp; temp = nullptr;
		}
		else if (_listaIzvrsenihAktivnosti == nullptr)
			_listaIzvrsenihAktivnosti = new Aktivnost[_trenutnoIzvrsenihAktivnosti + 1];
		_listaIzvrsenihAktivnosti[_trenutnoIzvrsenihAktivnosti++].Unos(*a->_vrsta, &a->_datumIzvrsenja, a->_ocjena, a->_nazivOpis);
		return true;
	}
	void Sortiraj() {
		if (_trenutnoIzvrsenihAktivnosti <= 0 || _listaIzvrsenihAktivnosti == nullptr)
			return;
		int iz, n = _trenutnoIzvrsenihAktivnosti;
		do {
			iz = 1;
			for (int i = 0; i < n - 1; i++) {
				if(_listaIzvrsenihAktivnosti[i]._ocjena!=NULL && _listaIzvrsenihAktivnosti[i+1]._ocjena!=NULL)
					if (_listaIzvrsenihAktivnosti[i]._ocjena > _listaIzvrsenihAktivnosti[i + 1]._ocjena) {
						swap(_listaIzvrsenihAktivnosti[i], _listaIzvrsenihAktivnosti[i + 1]);
						iz = 0;
					}
			}n -= 1;
		} while (n > 1 && iz == 0);
	}
	/* Karate pojas zahtijeva uspjesnu realizaciju svih planiranih aktivnosti, a one se za jedan povecavaju sa svakim novim pojasom, npr.
	zuti pojas: 1 x tehnika, 1 x kata, 1 x borba;	narandzasti pojas: 2 x tehnika, 2 x kata, 2 x borba; i td...
	Funkcija vraca false u slucaju da: su kandidatu u listu aktivnosti evidentirane tri negativno ocijenjene identicne vrste aktivnosti 
	(npr. tri negativne ocjene iz borbi), onda se taj pojas ne moze smatrati uspjesno stecenim
	i, te ukoliko nedostaje bilo koja od aktivnosti zahtijevanih u okviru tog pojasa. Ukoliko je kandidat uspjesno realizovao sve aktivnost, 
	datum polaganja se postavlja na datum posljednje uspjesno realizovane aktivnosti*/

	bool DaLiJePolozen() {
		if (_trenutnoIzvrsenihAktivnosti <= 0 || _listaIzvrsenihAktivnosti==nullptr)
			return false;
		int brojacUspjesnih[3] = { 0 }, brojacNeg[3] = { 0 };
		for (int i = 0; i < _trenutnoIzvrsenihAktivnosti; i++) {
			if (_listaIzvrsenihAktivnosti[i]._ocjena > 5)
				brojacUspjesnih[*_listaIzvrsenihAktivnosti[i]._vrsta]++;
			else
				brojacNeg[*_listaIzvrsenihAktivnosti[i]._vrsta]++;
		}
		if (brojacNeg[0] >= 3 || brojacNeg[1] >= 3 || brojacNeg[2] >= 3)
			return false;
		for (int i = 0; i < 3; i++)
			if (brojacUspjesnih[i] < ((int)_pojas + 1))
				return false;
		int poslPol;
		for(int i=_trenutnoIzvrsenihAktivnosti-1;i>=0;i--)
			if (_listaIzvrsenihAktivnosti[i]._ocjena > 5) {
				poslPol = i;
				break;
			}
		_datumPolaganja = new Datum;
		_datumPolaganja->Unos(*_listaIzvrsenihAktivnosti[poslPol]._datumIzvrsenja._dan, *_listaIzvrsenihAktivnosti[poslPol]._datumIzvrsenja._mjesec, *_listaIzvrsenihAktivnosti[poslPol]._datumIzvrsenja._godina);
		return true;
} 
	/*Funkcija vraca prosjecnu ocjenu svih uspjesno realizovanih aktivnosti koja u nazivu ili opisu sadrze vrijednost primljenog parametra.
	Ukoliko smatrate da je potrebno, mozete dodati i druge parametre za potrebe implementacije ove funkcije*/

	float PretragaRekrzivno(const char *parametar=nullptr,int i=0) {
		if (parametar == nullptr || _datumPolaganja == nullptr || i>=_trenutnoIzvrsenihAktivnosti || _trenutnoIzvrsenihAktivnosti<=0)
			return 0;
		
		if (_listaIzvrsenihAktivnosti[i]._ocjena > 5)
			return (float)(_listaIzvrsenihAktivnosti[i]._ocjena / (((int)_pojas + 1)*3)) + PretragaRekrzivno(parametar, i + 1);
		else return PretragaRekrzivno(parametar, i + 1);
	}
};

struct Kandidat {
	char * _imePrezime;
	shared_ptr<KaratePojas> _pojasevi[6];
	void Unos(const char * imePrezime = nullptr) {
		if (imePrezime == nullptr)
			return;
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		for (size_t i = 0; i < 6; i++)
			_pojasevi[i] = nullptr;
	}
	void Dealociraj() {
		if (_imePrezime != nullptr)
			delete[] _imePrezime;
		_imePrezime = nullptr;
		for (size_t i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr) {
				_pojasevi[i]->Dealociraj();
				_pojasevi[i].reset();
				_pojasevi[i] = nullptr;
			}
	}
	void Ispis() {
		if (_imePrezime != nullptr)
			cout << _imePrezime << endl;
		else
			cout << " PRAZNO\n";
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
		auto f = [=]() {
			int potrebno = (int)kp._pojas;
			int *uslov = new int[potrebno] { 0 };
			for (int i = 0; i < 6; i++)
				if (_pojasevi[i] != nullptr && _pojasevi[i]->_pojas < potrebno)
					uslov[_pojasevi[i]->_pojas]++;
			for (int i = 0; i < potrebno; i++)
				if (uslov[i] <= 0)
					return false;
			return true;
		};
		int slobodan = 0;
		if (_pojasevi[0] != nullptr) {
			while (_pojasevi[slobodan] != nullptr && slobodan < 6) {
				if (_pojasevi[slobodan + 1] == nullptr) {
					slobodan += 1;
					break;
				}
				slobodan++;
			}
		}
		if (slobodan > 0)
			if (!f())
				return false;
		if (slobodan >= 6)
		{
			cout << "Nema mjesta za novi pojas..\n";
			return false;
		}
		_pojasevi[slobodan] = make_shared<KaratePojas>();
		_pojasevi[slobodan]->Unos(kp._pojas);
		if (kp._trenutnoIzvrsenihAktivnosti > 0)
			for (int i = 0; i < kp._trenutnoIzvrsenihAktivnosti; i++)
				_pojasevi[slobodan]->DodajIzvrsenuAktivnost(&kp._listaIzvrsenihAktivnosti[i]);
		_pojasevi[slobodan]->DaLiJePolozen();
		return true;
	}
	//Funkcija GetNajbolji vraca par koji sadrzi oznaku i prosjecnu ocjenu (uspjesno okoncanih aktivnosti) pojasa sa najvecim prosjekom


	pair<Pojas, float>GetNajbolji() {
		int x = 0, ima = 0;
		for (int i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr)
				ima++;


		if (ima <= 0)
			return make_pair(Zuti, (float)0.0);
		float *prosjeci = new float[ima] {0};
		for (int i = 0; i < 6; i++)
			if (_pojasevi[i] != nullptr)
				prosjeci[i] = _pojasevi[i]->PretragaRekrzivno(" ");
		int naj = 0;
		for (int i = 1; i < ima; i++)
			if (prosjeci[i] > prosjeci[naj])
				naj = i;
		float temp = prosjeci[naj];
		delete[] prosjeci; prosjeci = nullptr;
		Pojas p = _pojasevi[naj]->_pojas;
		return make_pair(p, temp);
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
	Pojas pojass;
	tie(pojass, prosjek) = jasmin.GetNajbolji();
	cout << "Najbolji rezultat od " << prosjek << " je ostvaren tokom stjecanja pojasa " << pojas[pojass] << endl;

	datumPolaganja1.Dealociraj(), datumPolaganja2.Dealociraj(), datumPolaganja3.Dealociraj(), datumPolaganja4.Dealociraj();
	aktivnost1.Dealociraj(), aktivnost2.Dealociraj(), aktivnost3.Dealociraj(), aktivnost4.Dealociraj(), aktivnost5.Dealociraj();
	pojasZuti.Dealociraj(), pojasNarandzasti.Dealociraj(), pojasZeleni.Dealociraj();
	jasmin.Dealociraj();
	system("pause");
}
