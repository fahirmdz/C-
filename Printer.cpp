#include<iostream>
#include <memory>
#include <cstring>
#include<cctype>
#include <functional>
using namespace std;

//narednu liniju code-a ignorisite, osim u slucaju da vam bude predstavljala smetnje u radu
#pragma warning(disable:4996)

const char *crt = "\n-------------------------------------------\n";

enum vrstaDokumenta { PDF, DOC, TXT, HTML };
const char * vrstaDokumentaChar[] = { "PDF", "DOC", "TXT", "HTML" };

enum Prioritet { VISOK, SREDNJI, NIZAK };
const char * PrioritetChar[] = { "VISOK", "SREDNJI", "NIZAK" };

char * AlocirajNizKaraktera(const char * sadrzaj) {
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

const int BROJ_ZNAKOVA_PO_STRANICI = 30;

struct DatumVrijeme {
	int *_dan, *_mjesec, *_godina, *_sati, *_minuti;
	void Unos(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 1, int minuti = 1) {
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
};
int compareDate(DatumVrijeme d1, DatumVrijeme d2) {
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
	else if (*d1._sati > *d2._sati)
		return 1;
	else if (*d1._sati < *d2._sati)
		return -1;
	else if (*d1._minuti > *d2._minuti)
		return 1;
	else if (*d1._minuti < *d2._minuti)
		return -1;
	else return 0;
}
struct Filter {
	char * _sadrzaj;
	Prioritet _prioritet;
	void Unos(const char * sadrzaj, Prioritet prioritet) {
		if (sadrzaj != nullptr)
			_sadrzaj = AlocirajNizKaraktera(sadrzaj);
		else return;
		_prioritet = prioritet;
	}
	void Dealociraj() {
		if (_sadrzaj != nullptr) {
			delete[] _sadrzaj; _sadrzaj = nullptr;
		}
	}
	void Ispis() {
		if (_sadrzaj != nullptr)
			cout << _sadrzaj << " (" << PrioritetChar[_prioritet] << ")" << endl;
		else
			cout << "Nema sadrzaja..\n";
	}
};

struct Dokument {
	vrstaDokumenta _vrsta;
	char * _naziv;
	char * _sadrzaj;
	unique_ptr<DatumVrijeme> _kreiran;
	int _brojStranica;

	void Unos(vrstaDokumenta vrsta, const char * naziv, DatumVrijeme kreiran) {
		if (naziv != nullptr)
			_naziv = AlocirajNizKaraktera(naziv);
		else return;
		_vrsta = vrsta;
		_kreiran = make_unique<DatumVrijeme>();
		_kreiran->Unos(*kreiran._dan, *kreiran._mjesec, *kreiran._godina, *kreiran._sati, *kreiran._minuti);
		_brojStranica = 0;//broj stranica se odredjuje prilikom svakog dodavanja novog sadrzaja dokumentu
		_sadrzaj = nullptr;
	}
	void Dealociraj() {
		if (_naziv != nullptr) {
			delete[] _naziv; _naziv = nullptr;
		}
		if (_sadrzaj != nullptr) {
			delete[] _sadrzaj; _sadrzaj = nullptr;
		}
		_kreiran->Dealociraj();
		_kreiran.reset();
		_kreiran = nullptr;
	}
	void Ispis() {
		if (_naziv != nullptr)
			cout << crt << _naziv;
		else
			cout << "--Nema naziva--";
		cout << " " << vrstaDokumentaChar[_vrsta] << " kreiran ";
		_kreiran->Ispis();
		if (_sadrzaj != nullptr)
			cout << crt << _sadrzaj << crt << " br.stranica (" << _brojStranica << ")" << crt;
		else cout << "Nema sadrzaja..\n";
	}
	void DodajSadrzaj(const char *sadrzaj) {
		if (sadrzaj == nullptr) {
			cout << "Morate unijeti parametar sadrzaj..\n";
			return;
		}
		if (_sadrzaj != nullptr) {
			char *temp = new char[strlen(_sadrzaj) + 1];
			strcpy_s(temp, strlen(_sadrzaj) + 1, _sadrzaj);
			delete[] _sadrzaj;
			int v = strlen(temp) + strlen(sadrzaj) + 2;
			_sadrzaj = new char[v];
			strcpy_s(_sadrzaj, v, temp);
			delete[] temp; temp = nullptr;
			strcat_s(_sadrzaj, v, sadrzaj);
		}
		else
			_sadrzaj = AlocirajNizKaraktera(sadrzaj);
		float brs = (float)strlen(_sadrzaj) / BROJ_ZNAKOVA_PO_STRANICI;
		if (brs - (int)brs > 0)
			_brojStranica = (int)brs + 1;
		else
			_brojStranica = (int)brs;
	}
};
struct Printer {
	char * _model;
	Dokument * _dokumenti;
	int _trenutnoDokumenata;
	Filter * _filteri;
	int _trenutnoFiltera;

	void Unos(const char * model) {
		if (model != nullptr)
			_model = AlocirajNizKaraktera(model);
		else return;
		_filteri = nullptr;
		_dokumenti = nullptr;
		_trenutnoDokumenata = 0;
		_trenutnoFiltera = 0;
	}
	void Dealociraj() {
		if (_model != nullptr) {
			delete[] _model; _model = nullptr;
		}
		if (_trenutnoDokumenata > 0)
			for (size_t i = 0; i < _trenutnoDokumenata; i++)
				_dokumenti[i].Dealociraj();
		if (_dokumenti != nullptr) {
			delete[]_dokumenti; _dokumenti = nullptr;
		}
		if (_trenutnoFiltera > 0)
			for (size_t i = 0; i < _trenutnoFiltera; i++)
				_filteri[i].Dealociraj();
		if (_filteri != nullptr) {
			delete[]_filteri; _filteri = nullptr;
		}
	}
	void Ispis() {
		if (_model != nullptr)
			cout << _model << crt;
		if (_trenutnoDokumenata > 0)
			for (size_t i = 0; i < _trenutnoDokumenata; i++) {
				if (uslovPrikaza(_dokumenti[i])) {
					_dokumenti[i].Ispis();
				}
			}
		else cout << "Nema sacuvanih dokumenata u printeru..\n";
	}
	bool DodajFilter(const char *sadrzaj,Prioritet p) {
		if (sadrzaj == nullptr) {
			cout << "Morate unijeti parametar sadrzaj..\n";
			return false;
		}
		if (_trenutnoFiltera > 0) {
			for (int i = 0; i < _trenutnoFiltera; i++)
				if (_filteri[i]._sadrzaj != nullptr)
					if (strcmp(_filteri[i]._sadrzaj, sadrzaj) == 0)
						return false;
			Filter *temp = new Filter[_trenutnoFiltera];
			for (int i = 0; i < _trenutnoFiltera; i++)
				swap(_filteri[i], temp[i]);
			delete[] _filteri;
			_filteri = new Filter[_trenutnoFiltera + 1];
			for(int i=0;i<_trenutnoFiltera;i++)
				swap(_filteri[i], temp[i]);
			delete[] temp; temp = nullptr;
		}
		else _filteri = new Filter[_trenutnoFiltera + 1];
		_filteri[_trenutnoFiltera++].Unos(sadrzaj, p);
		return true;
	}
	/*
	DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDECA PRAVILA:
	1. NAZIV DOKUMENTA MOZE SADRZAVATI SAMO SLOVA, A EKSTENZIJA MOZE BITI IDENTICNA ONOJ DEFINISANOJ VRIJEDNOSCU ATRIBUTA vrstaDokumenta
	2. SADRZAJ DOKUMENTA MOZE POSJEDOVATI RIJECI DEFINISANE FILTERIMA, ALI PREMA SLJEDECIM PRAVILIMA:
	- NITI JEDNU RIJEC OZNACENU FILTEROM PRIORITETA VISOK
	- NAJVISE 2 RIJECI OZNACENE FILTEROM PRIORITETA SREDNJI
	- NAJVISE 3 RIJECI OZNACENE FILTEROM PRIORITETA NIZAK
	UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA PRINTAJ DOKUMENT TREBA SACUVATI U NIZ _dokumenti, ALI TAJ DOKUMENT NE TREBA PRINTATI 
	I FUNKCIJA VRACA FALSE.
	UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI SADRZAJ DOKUMENTA KOJI SE PRINTA, A SA CRT LINIJOM ODVAJATI
	STRANICE DOKUMENTA
	*/
	bool uslovPrikaza(Dokument &d) {
		
		if (d._sadrzaj == nullptr) {
			cout << "Dokument nema sadrzaja..\n";
			return false;
		}
		if (d._naziv != nullptr) {
			int x = 0;
			while (d._naziv[x] != '.') {
				if (!isalpha(d._naziv[x])) {
					return false;
					break;
				}
				x++;
			}
		}
		char *eks = strstr(d._naziv, ".");
		if (eks != NULL) {
			eks += 1;
			char *temp = new char[strlen(eks) + 1];
			strcpy_s(temp, strlen(eks) + 1, eks);
			for (int i = 0; i < strlen(temp); i++)
				if (isalpha(temp[i]))
					temp[i] = toupper(temp[i]);
			bool ekstenzija = false;
			for (int i = 0; i < 4; i++)
				if (strcmp(temp, vrstaDokumentaChar[i]) == 0)
					ekstenzija = true;
			if (!ekstenzija)
				return false;
			if (temp != nullptr) {
				delete[] temp; temp = nullptr;
			}
		}
		else
			return false;

		
			eks = nullptr;
		int brojacFiltera[3] = { 0 };
		if (_trenutnoFiltera > 0) {
			char *trazi;
			for (int i = 0; i < _trenutnoFiltera; i++) {
				char *tempTekst = new char[strlen(d._sadrzaj) + 1];
				strcpy_s(tempTekst, strlen(d._sadrzaj) + 1, d._sadrzaj);
				for (int j = 0; j < strlen(tempTekst); j++)
					if (isalpha(tempTekst[j]))
						tempTekst[j] = toupper(tempTekst[j]);
				trazi = strstr(tempTekst, _filteri[i]._sadrzaj);
				if (trazi != NULL) {
					brojacFiltera[_filteri[i]._prioritet]++;
					trazi += 1;
					while (trazi != NULL) {
						trazi = strstr(trazi, _filteri[i]._sadrzaj);
						if (trazi != NULL) {
							brojacFiltera[_filteri[i]._prioritet]++;
							trazi += 1;
						}
					}
				}
				trazi = nullptr;
				delete[] tempTekst; tempTekst = nullptr;
			}
			trazi = nullptr;
		}
		if (brojacFiltera[0] > 0 || brojacFiltera[1] > 2 || brojacFiltera[2] > 3)
			return false;

	}
	bool Printaj(Dokument &d) {
		
		if (_trenutnoDokumenata > 0) {
			Dokument *temp = new Dokument[_trenutnoDokumenata];
			for (int i = 0; i < _trenutnoDokumenata; i++)
				swap(temp[i], _dokumenti[i]);
			delete[] _dokumenti;
			_dokumenti = new Dokument[_trenutnoDokumenata + 1];
			for (int i = 0; i < _trenutnoDokumenata; i++)
				swap(temp[i], _dokumenti[i]);
			delete[] temp; temp = nullptr;
		}
		else
			_dokumenti = new Dokument[_trenutnoDokumenata + 1];
		_dokumenti[_trenutnoDokumenata++].Unos(d._vrsta, d._naziv, *d._kreiran);
		_dokumenti[_trenutnoDokumenata - 1].DodajSadrzaj(d._sadrzaj);
		if (!uslovPrikaza(d))
			return false;

		if (d._naziv != nullptr)
			cout << crt << "Naziv: " << d._naziv << endl;
		else cout << "--Nema naziva--\n";
		cout << "Vrsta: " << vrstaDokumentaChar[d._vrsta] << endl;
		cout << "Datum kreiranja: ";
		if (d._kreiran != nullptr)
			d._kreiran->Ispis();
		cout << crt << crt;
		for (int i = 0; i < strlen(d._sadrzaj); i++) {
			cout << d._sadrzaj[i];
			if (i>0 && i % 15 == 0)cout << endl;
			if (i>0 && i%BROJ_ZNAKOVA_PO_STRANICI == 0)cout << crt;
		}cout << crt;
		cout << "Broj stranica: " << d._brojStranica << endl;
		return true;
	}
	int GetProsjecanBrojStranicaUPeriodu_Od_Do(DatumVrijeme d1, DatumVrijeme d2, int i = 0) {
		if (_trenutnoDokumenata <= 0)
			return 0;
		if (i == _trenutnoDokumenata)
			return 0;
		if (_dokumenti[i]._kreiran != nullptr) {
			int u1 = compareDate(d1, *_dokumenti[i]._kreiran), u2 = compareDate(d2, *_dokumenti[i]._kreiran);
			if ((u1 == 0 && u2 == 1) || (u1 == -1 && u2 == 0))
				return _dokumenti[i]._brojStranica / _trenutnoDokumenata + GetProsjecanBrojStranicaUPeriodu_Od_Do(d1, d2, i + 1);
			else return GetProsjecanBrojStranicaUPeriodu_Od_Do(d1, d2, i + 1);
		}
		else return GetProsjecanBrojStranicaUPeriodu_Od_Do(d1, d2, i + 1);
	}
	Filter *GetNakoristenijiFilter() {
		if (_trenutnoFiltera > 0) {
			int *brojacFiltera = new int[_trenutnoFiltera] {0};
			if (_trenutnoDokumenata > 0) {
				for (int z = 0; z < _trenutnoDokumenata; z++) {
					if (_dokumenti[z]._sadrzaj != nullptr) {
						char *trazi;
						for (int i = 0; i < _trenutnoFiltera; i++) {
							char *tempTekst = new char[strlen(_dokumenti[z]._sadrzaj) + 1];
							strcpy_s(tempTekst, strlen(_dokumenti[z]._sadrzaj) + 1, _dokumenti[z]._sadrzaj);
							for (int j = 0; j < strlen(tempTekst); j++)
								if (isalpha(tempTekst[j]))
									tempTekst[j] = toupper(tempTekst[j]);
							trazi = strstr(tempTekst, _filteri[i]._sadrzaj);
							if (trazi != NULL) {
								brojacFiltera[i]++;
								trazi += 1;
								while (trazi != NULL) {
									trazi = strstr(trazi, _filteri[i]._sadrzaj);
									if (trazi != NULL) {
										brojacFiltera[i]++;
										trazi += 1;
									}
								}
							}

							trazi = nullptr;
							tempTekst = nullptr;
						}
						trazi = nullptr;
					}
				}
			}
			int naj = 0;
			for (int i = 1; i < _trenutnoFiltera; i++)
				if (brojacFiltera[i] > brojacFiltera[naj])
					naj = i;
			delete[] brojacFiltera; brojacFiltera = nullptr;
			return &_filteri[naj];
		}
		else
			return nullptr;
	}
};


int main() {
	DatumVrijeme prije3Dana; prije3Dana.Unos(3, 2, 2018, 10, 15);
	DatumVrijeme danas1; danas1.Unos(6, 2, 2018, 10, 15);
	DatumVrijeme danas2; danas2.Unos(6, 2, 2018, 10, 16);
	DatumVrijeme za10Dana; za10Dana.Unos(16, 2, 2018, 10, 15);

	Dokument ispitPRII, ispitMAT, ispitUIT, ispitUITDrugiRok;
	ispitPRII.Unos(DOC, "ispitPRII.doc", prije3Dana);
	ispitMAT.Unos(DOC, "ispitMAT.doc", danas1);
	ispitUIT.Unos(DOC, "ispitUIT.doc", danas2);
	ispitUITDrugiRok.Unos(PDF, "ispitUITDrugiRok.pdf", za10Dana);

	/*BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREDJUJE PRILIKOM DODAVANJA SADRZAJA. ZA POTREBE ISPITA PRETPOSTAVLJAMO DA NA JEDNU
	STRANICU MOZE STATI BROJ_ZNAKOVA_PO_STRANICI ZNAKOVA UKLJUCUJUCI RAZMAKE I DRUGE VRSTE ZNAKOVA*/
	ispitPRII.DodajSadrzaj("Programiranje ili racunarsko programiranje (engl. programming) jeste vjestina pomocu koje ");
	ispitPRII.DodajSadrzaj("korisnik stvara i izvrsava algoritme koristeci odredjene programske jezike da bi ... ");
	cout << "Broj stranica -> " << ispitPRII._brojStranica << endl;

	ispitPRII.Ispis();//ISPISUJE SVE PODATKE O DOKUMENTU
	
	ispitMAT.DodajSadrzaj("Matematika se razvila iz potrebe da se obavljaju proracuni u trgovini, vrse mjerenja zemljista i predvidjaju ");
	ispitMAT.DodajSadrzaj("astronomski dogadjaji, i ove tri primjene se mogu dovesti u vezu sa grubom podjelom matematike ");

	Printer hp3200; hp3200.Unos("HP 3200");
	/*PRINTER NECE DOZVOLITI PRINTANJE DOKUMENATA U CIJEM SADRZAJU SE NALAZI NEKA OD ZABRANJENIH RIJECI DEFINISANIH FILTERIMA*/
	if (hp3200.DodajFilter("RE", NIZAK)) //VISOK :)
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("RAT", VISOK))
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("UBITI", VISOK))
		cout << "Filter dodan!" << endl;
	if (hp3200.DodajFilter("MRZITI", SREDNJI))
		cout << "Filter dodan!" << endl;
	if (!hp3200.DodajFilter("RE", SREDNJI))/* ONEMOGUCITI PONAVLJANJE FILTERA, BEZ OBZIRA STO SU RAZLICITOG PRIORITETA*/
		cout << "Filter nije dodan!" << endl;


	if (hp3200.Printaj(ispitPRII))
		cout << "Printan-> " << ispitPRII._naziv << endl << endl;
	if (hp3200.Printaj(ispitMAT))
		cout << "Printan -> " << ispitMAT._naziv << endl;
	if (hp3200.Printaj(ispitUIT))
		cout << "Printan -> " << ispitUIT._naziv << endl;
	if (hp3200.Printaj(ispitUITDrugiRok))
		cout << "Printan -> " << ispitUITDrugiRok._naziv << endl;

	//PROSJECAN BROJ STRANICA = UKUPAN BROJ STRANICA / BROJ PRINTANIH DOKUMENATA
	//REKURZIVNA FUNKCIJA VRACA PROSJECAN BROJ ISPRINTANIH STRANICA (KOJE SU ZADOVOLJILE POSTAVLJENE FILTERE) U PERIODU OD (npr.prije3Dana) - DO (npr.danas2), UKLJUCUJUCI I TAJ PERIOD
	cout << "Prosjecan broj printanih stranica je -> " << hp3200.GetProsjecanBrojStranicaUPeriodu_Od_Do(prije3Dana,danas1) << crt;
	Filter * f = hp3200.GetNakoristenijiFilter();
	if (f != nullptr && f->_sadrzaj != nullptr)
		cout << "Najcesce koristeni sadrzaj filtera je -> " << f->_sadrzaj << crt;
	//PORED VRIJEDNOSTI SVIH OSTALIH ATRIBUTA, ISPISUJE SAMO ONE DOKUMENTE KOJI ZADOVOLJAVAJU SVA PRETHODNO DEFINISANA PRAVILA
	hp3200.Ispis();
	/*NAPISATI LAMBDA FUNKCIJU ZamijeniIPrintaj KOJA U SADRZAJU DOKUMENTA MIJENJA PRVI POSLATI PARAMETAR SA VRIJEDNOSCU DRUGOG PARAMETRA, TE VRACA BROJ ZAMIJENJENIH ZNAKOVA*/
	auto ZamijeniIPrintaj = [&hp3200](char a, char b, DatumVrijeme d) {
		if (hp3200._trenutnoDokumenata <= 0)
			return 0;
		int t = -1, brojac = 0;
		for (int i = 0; i < hp3200._trenutnoDokumenata; i++)
			if(hp3200._dokumenti[i]._kreiran!=nullptr)
			if (compareDate(d, *hp3200._dokumenti[i]._kreiran) == 0) {
				t = i;
				break;
			}
		if (t == -1)
			return 0;
		char *p = strstr(hp3200._dokumenti[t]._sadrzaj, &a);
		if (p != NULL) {
			brojac++;
			p[0] = b;
			p += 1;
			while (p != NULL) {
				p = strstr(p, &a);
				if (p != NULL) {
					brojac++;
					p[0] = b;
					p += 1;
				}
			}
		}
		else 
			return 0;
		return brojac;
	};
	//RAZMAK MIJENJA ZNAKOM CRTICA U SADRZAJU DOKUMENATA KOJI SU PRINTANI danas1
	int zamijenjeno = ZamijeniIPrintaj(' ', '-', danas1);
	
	cout << "Zamijenjeno -> " << zamijenjeno << " znakova" << endl;
	hp3200.Ispis();

	prije3Dana.Dealociraj(); danas1.Dealociraj(); danas2.Dealociraj(); za10Dana.Dealociraj();
	ispitMAT.Dealociraj(); ispitPRII.Dealociraj(); ispitUIT.Dealociraj(); ispitUITDrugiRok.Dealociraj();

	hp3200.Dealociraj();
	
	system("PAUSE>1");
	return 0;
}
