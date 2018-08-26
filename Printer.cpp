#include <iostream>
#include <memory>
#include <functional>
#include <cstring>
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
struct Filter {
	char * _sadrzaj;
	Prioritet _prioritet;
	void Unos(const char * sadrzaj, Prioritet prioritet) {
		_sadrzaj = AlocirajNizKaraktera(sadrzaj);
		_prioritet = prioritet;
	}
	void Dealociraj() {
		delete[] _sadrzaj; _sadrzaj = nullptr;
	}
	void Ispis() { cout << _sadrzaj << " (" << PrioritetChar[_prioritet] << ")" << endl; }
};

struct Dokument {
	vrstaDokumenta _vrsta;
	char * _naziv;
	char * _sadrzaj;
	unique_ptr<DatumVrijeme> _kreiran;
	int _brojStranica;

	void Unos(vrstaDokumenta vrsta, const char * naziv, DatumVrijeme kreiran) {
		_naziv = AlocirajNizKaraktera(naziv);
		_vrsta = vrsta;
		_kreiran = make_unique<DatumVrijeme>();
		_kreiran->Unos(*kreiran._dan, *kreiran._mjesec, *kreiran._godina,*kreiran._sati,*kreiran._minuti);
		_brojStranica = 0;//broj stranica se odredjuje prilikom svakog dodavanja novog sadrzaja dokumentu
		_sadrzaj = nullptr;
	}
	void Dealociraj() {
		delete[] _naziv; _naziv = nullptr;
		delete[] _sadrzaj; _sadrzaj = nullptr;
		_kreiran->Dealociraj();
		_kreiran.reset(); _kreiran = nullptr;
	}
	void Ispis() {
		cout << crt << _naziv << "\nvrsta dokumenta: " << vrstaDokumentaChar[_vrsta] << "\nDatum i vrijeme kreiranja ";
		_kreiran->Ispis();
		if (_sadrzaj != nullptr)
			cout << crt << _sadrzaj << crt << "Broj stranica -> (" << _brojStranica << ")" << crt;
		else cout << "\nEMPTY\n";
	}
	/*BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREDJUJE PRILIKOM DODAVANJA SADRZAJA. ZA POTREBE ISPITA PRETPOSTAVLJAMO DA NA JEDNU
	STRANICU MOZE STATI BROJ_ZNAKOVA_PO_STRANICI ZNAKOVA UKLJUCUJUCI RAZMAKE I DRUGE VRSTE ZNAKOVA*/
	void DodajSadrzaj(const char *sadrzaj) {
		if (sadrzaj != nullptr) {
			int v = strlen(sadrzaj);
			if (_sadrzaj == nullptr)
				_sadrzaj = AlocirajNizKaraktera(sadrzaj);
			else {
				char *temp = new char[strlen(_sadrzaj) + 2];
				strcpy_s(temp, strlen(_sadrzaj) + 2, _sadrzaj);
				delete[] _sadrzaj; _sadrzaj = nullptr;
				v = strlen(temp) + strlen(sadrzaj) + 3;
				_sadrzaj = new char[v];
				strcpy_s(_sadrzaj, v, temp);
				strcat_s(_sadrzaj, v, sadrzaj);
				delete[] temp; temp = nullptr;
			}
			float decStr = (float)(v - 1) / BROJ_ZNAKOVA_PO_STRANICI;
			if ((decStr - (int)decStr) > 0)_brojStranica = (int)decStr + 1;
			else _brojStranica = (int)decStr;
		}
	}
};
struct Printer {
	char * _model;
	Dokument * _dokumenti;
	int _trenutnoDokumenata;
	Filter * _filteri;
	int _trenutnoFiltera;

	void Unos(const char * model) {
		_model = AlocirajNizKaraktera(model);
		_filteri = nullptr;
		_dokumenti = nullptr;
		_trenutnoDokumenata = 0;
		_trenutnoFiltera = 0;
	}
	void Dealociraj() {
		delete[] _model; _model = nullptr;
		if (_trenutnoDokumenata > 0) {
			for (int i = 0; i < _trenutnoDokumenata; i++)
				_dokumenti[i].Dealociraj();
			delete[]_dokumenti; _dokumenti = nullptr;
		}
		if (_trenutnoFiltera > 0) {
			for (int i = 0; i < _trenutnoFiltera; i++)
				_filteri[i].Dealociraj();
			delete[]_filteri; _filteri = nullptr;
		}
	}
	void Ispis() {
		cout << "Model: " << _model << crt;
		cout << "-----> Dokumenti sacuvani u printeru <------" << crt;
		if (_trenutnoDokumenata > 0)
			for (size_t i = 0; i < _trenutnoDokumenata; i++)
				_dokumenti[i].Ispis();
	}
	/*PRINTER NECE DOZVOLITI PRINTANJE DOKUMENATA U CIJEM SADRZAJU SE NALAZI NEKA OD ZABRANJENIH RIJECI DEFINISANIH FILTERIMA*/
	bool DodajFilter(const char *sadrzaj, Prioritet p) {
		if (_trenutnoFiltera > 0) {
			for (int i = 0; i < _trenutnoFiltera; i++)
				if (strcmp(sadrzaj, _filteri[i]._sadrzaj) == 0)return false;
			_filteri=expandArray(_filteri, _trenutnoFiltera);
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
	UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA PRINTAJ DOKUMENT TREBA SACUVATI U NIZ _dokumenti, ALI TAJ DOKUMENT NE TREBA PRINTATI I FUNKCIJA VRACA FALSE.
	UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI SADRZAJ DOKUMENTA KOJI SE PRINTA, A SA CRT LINIJOM ODVAJATI STRANICE DOKUMENTA
	*/

	bool Printaj(Dokument &d) {
		if (_trenutnoDokumenata > 0) _dokumenti=expandArray(_dokumenti, _trenutnoDokumenata);
		else _dokumenti = new Dokument[_trenutnoDokumenata + 1];
		_dokumenti[_trenutnoDokumenata].Unos(d._vrsta, d._naziv, *d._kreiran);
		_dokumenti[_trenutnoDokumenata++].DodajSadrzaj(d._sadrzaj);

		bool uslovPrvi = true;
		int x = 0;
		while (d._naziv[x] != '.') {
			if ((int)d._naziv[x]<(int)'a' || (int)d._naziv[x]>(int)'z')uslovPrvi = false;
			x++;
		}
		char *ekst = strstr(d._naziv, ".");
		ekst += 1;
		for (int i = 0; i < strlen(ekst); i++)ekst[i] = toupper(ekst[i]);
		if (strcmp(vrstaDokumentaChar[d._vrsta], ekst) != 0)uslovPrvi = false;
		if (!uslovPrvi)return false;
		ekst = nullptr;
		int zabranjeneRijeci[3] = { 0 };
		for (int i = 0; i < _trenutnoFiltera; i++) {

			char *temp = new char[strlen(_filteri[i]._sadrzaj)+1];
			for (int z = 0; z < strlen(_filteri[i]._sadrzaj); z++)temp[z] = tolower(_filteri[i]._sadrzaj[z]);
			temp[strlen(_filteri[i]._sadrzaj)] = '\0';
			char *zabR = strstr(d._sadrzaj, temp);
			if (zabR != NULL) {
				zabranjeneRijeci[_filteri[i]._prioritet]++;
				while (zabR != NULL) {
					zabR += 1;
					zabR = strstr(zabR, temp);
					if (zabR != NULL)zabranjeneRijeci[_filteri[i]._prioritet]++;
				}
			}
			delete[] temp;
			temp = nullptr;
		}
		if (zabranjeneRijeci[0] > 0 || zabranjeneRijeci[1] > 2 || zabranjeneRijeci[2] > 3)return false;
		cout << "----> " << d._naziv << " <------" << crt;
		for (int i = 0; i < strlen(d._sadrzaj); i++) {
			cout << d._sadrzaj[i];
			if (i % BROJ_ZNAKOVA_PO_STRANICI == 0 && i>0)cout << crt;
		}
		cout << endl << endl;
		return true;
	}
	//REKURZIVNA FUNKCIJA VRACA PROSJECAN BROJ ISPRINTANIH STRANICA (KOJE SU ZADOVOLJILE POSTAVLJENE FILTERE) U PERIODU OD (npr.prije3Dana) - DO (npr.danas2), UKLJUCUJUCI I TAJ PERIOD

	int GetProsjecanBrojStranicaUPeriodu_Od_Do(DatumVrijeme d1, DatumVrijeme d2, int brd,int i = 0) {
		if (i == _trenutnoDokumenata)return 0;
		int u1 = compareDate(d1, *_dokumenti[i]._kreiran), u2 = compareDate(d2, *_dokumenti[i]._kreiran);
		if ((u1 == -1 && u2 == 1) || (u1 == 0 && u2 == 1) || (u1 == -1 && u2 == 0))return _dokumenti[i]._brojStranica / brd + GetProsjecanBrojStranicaUPeriodu_Od_Do(d1, d2, brd,i + 1);
		else return GetProsjecanBrojStranicaUPeriodu_Od_Do(d1, d2, brd,i + 1);
	}
	Filter *GetNakoristenijiFilter() {
		int *brojacK = new int[_trenutnoFiltera];
		for (int i = 0; i < _trenutnoFiltera; i++)brojacK[i] = 0;

		for (int i = 0; i < _trenutnoDokumenata; i++) {
			if (_dokumenti[i]._sadrzaj != nullptr) {
				for (int j = 0; j < _trenutnoFiltera; j++) {
					char *temp = new char[strlen(_filteri[j]._sadrzaj) + 1];
					for (int z = 0; z < strlen(_filteri[j]._sadrzaj); z++)temp[z] = tolower(_filteri[j]._sadrzaj[z]);
					temp[strlen(_filteri[j]._sadrzaj)] = '\0';
					char *p = strstr(_dokumenti[i]._sadrzaj, _filteri[j]._sadrzaj);
					if (p != NULL) {
						brojacK[j]++;
						while (p != NULL) {
							p += 1;
							p = strstr(_dokumenti[i]._sadrzaj, _filteri[j]._sadrzaj);
							if (p != NULL) brojacK[j]++;
						}
					}
					p = nullptr;
				}
			}
		}
		int naj = 0;
		for (int i = 1; i < _trenutnoFiltera; i++)
			if (brojacK[i] > brojacK[naj])naj = i;
		return &_filteri[naj];
	}

};



int main() {
	DatumVrijeme prije3Dana; prije3Dana.Unos(3, 2, 2018, 10, 15);
	DatumVrijeme danas1; danas1.Unos(6, 2, 2018, 10, 15);
	DatumVrijeme danas2; danas2.Unos(6, 2, 2018, 10, 16);
	DatumVrijeme za10Dana; za10Dana.Unos(16, 2, 2018, 10, 15);

	Dokument ispitPRII, ispitMAT, ispitUIT, ispitUITDrugiRok;
	ispitPRII.Unos(DOC, "ispitprii.doc", prije3Dana);
	ispitMAT.Unos(DOC, "ispitmat.doc", danas1);
	ispitUIT.Unos(DOC, "ispitUIT.doc", danas2);
	ispitUITDrugiRok.Unos(PDF, "ispitUITDrugiRok.pdf", za10Dana);

	ispitPRII.DodajSadrzaj("Programiranje ili racunarsko programiranje (engl. programming) jeste vjestina pomocu koje ");
	ispitPRII.DodajSadrzaj("korisnik stvara i izvrsava algoritme koristeci odredjene programske jezike da bi ... ");

	ispitPRII.Ispis();//ISPISUJE SVE PODATKE O DOKUMENTU

	ispitMAT.DodajSadrzaj("Matematika se razvila iz potrebe da se obavljaju proracuni u trgovini, vrse mjerenja zemljista i predvidjaju ");
	ispitMAT.DodajSadrzaj("astronomski dogadjaji, i ove tri primjene se mogu dovesti u vezu sa grubom podjelom matematike ");

	Printer hp3200; hp3200.Unos("HP 3200");

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
		cout << "Printam -> " << ispitPRII._naziv << endl;

	if (hp3200.Printaj(ispitMAT))
		cout << "Printam -> " << ispitMAT._naziv << endl;
	if (hp3200.Printaj(ispitUIT))
		cout << "Printam -> " << ispitUIT._naziv << endl;
	if (hp3200.Printaj(ispitUITDrugiRok))
		cout << "Printam -> " << ispitUITDrugiRok._naziv << endl;

	//PROSJECAN BROJ STRANICA = UKUPAN BROJ STRANICA / BROJ PRINTANIH DOKUMENATA
	//REKURZIVNA FUNKCIJA VRACA PROSJECAN BROJ ISPRINTANIH STRANICA (KOJE SU ZADOVOLJILE POSTAVLJENE FILTERE) U PERIODU OD (npr.prije3Dana) - DO (npr.danas2), UKLJUCUJUCI I TAJ PERIOD
	int brD = 0;
	for (int i = 0; i < hp3200._trenutnoDokumenata; i++) {
		int u1 = compareDate(prije3Dana, *hp3200._dokumenti[i]._kreiran), u2 = compareDate(danas2, *hp3200._dokumenti[i]._kreiran);
		if ((u1 == -1 && u2 == 1) || (u1 == 0 && u2 == 1) || (u1 == -1 && u2 == 0))
			brD++;
	}
	cout << "Prosjecan broj printanih stranica je -> " << hp3200.GetProsjecanBrojStranicaUPeriodu_Od_Do(prije3Dana,danas2,brD) << crt;
	//VRACA SADRZAJ FILTERA KOJI SE NAJCESCE KORISTIO U DOKUMENTIMA
	Filter * f = hp3200.GetNakoristenijiFilter();
	if (f != nullptr && f->_sadrzaj != nullptr)
		cout << "Najcesce koristeni sadrzaj filtera je -> " << f->_sadrzaj << crt;
	//PORED VRIJEDNOSTI SVIH OSTALIH ATRIBUTA, ISPISUJE SAMO ONE DOKUMENTE KOJI ZADOVOLJAVAJU SVA PRETHODNO DEFINISANA PRAVILA
	hp3200.Ispis();

	/*NAPISATI LAMBDA FUNKCIJU ZamijeniIPrintaj KOJA U SADRZAJU DOKUMENTA MIJENJA PRVI POSLATI PARAMETAR SA VRIJEDNOSCU DRUGOG PARAMETRA,
	TE VRACA BROJ ZAMIJENJENIH ZNAKOVA*/
	auto ZamijeniIPrintaj = [&](char a, char b, DatumVrijeme d) {
		int t = -1;
		for (int i = 0; i < hp3200._trenutnoDokumenata; i++)
			if (compareDate(d, *hp3200._dokumenti[i]._kreiran) == 0) {
				t = i;
				break;
			}
		if (t == -1)return 0;
		int br = 0;
		for (int i = 0; i < strlen(hp3200._dokumenti[t]._sadrzaj); i++) {
			if (hp3200._dokumenti[t]._sadrzaj[i] == a) {
				hp3200._dokumenti[t]._sadrzaj[i] = b;
				br++;
			}
		}
		return br;
	};
	//RAZMAK MIJENJA ZNAKOM CRTICA U SADRZAJU DOKUMENATA KOJI SU PRINTANI danas1
	int zamijenjeno = ZamijeniIPrintaj(' ', '-', danas1);
	cout << "Zamijenjeno -> " << zamijenjeno << " znakova" << endl;
	hp3200.Ispis();

	prije3Dana.Dealociraj(); danas1.Dealociraj(); danas2.Dealociraj(); za10Dana.Dealociraj();
	ispitMAT.Dealociraj(); ispitPRII.Dealociraj(); ispitUIT.Dealociraj(); ispitUITDrugiRok.Dealociraj();

	hp3200.Dealociraj();

	return 0;
}