#include <iostream>
#include <cstring>
using namespace std;
enum GodinaStudija { PRVA = 1, DRUGA, TRECA, CETVRTA };
enum NacinStudiranja { DL = 1, REDOVAN = 3 };
char *godinaStudija[] = { "PRVA","DRUGA","TRECA","CETVRTA" };
struct polozeniPredmet {
	char *naziv;
	int *ocjena;
	GodinaStudija *godina;
	bool Obavezni;
	char *Napomena;
	void unos(char *_naziv, int _ocjena, GodinaStudija _Godina, bool _obavezni) {
		int v = strlen(_naziv) + 1;
		naziv = new char[v];
		strcpy_s(naziv, v, _naziv);
		ocjena = new int(_ocjena);
		godina = new GodinaStudija(_Godina);
		Obavezni = _obavezni;
		Napomena = nullptr;
	}
	void setNapomena(char * _napomena) {
		int v = strlen(_napomena) + 1;
		Napomena = new char[v];
		strcpy_s(Napomena, v, _napomena);
	}
	void ispis() {
		cout << "Naziv predmeta: " << naziv << endl;
		cout << "Ocjena: " << *ocjena << endl;
		cout << "Godina studija: " << godinaStudija[*godina - 1] << endl;
		cout << "Obavezan: ";
		if (Obavezni)cout << "DA\n"; else cout << "NE\n";
		if (Napomena != nullptr)cout << "Napomena: " << Napomena << endl;
		cout << endl << endl;
	}
	void dealociraj() {
		delete[] naziv; naziv = nullptr;
		delete ocjena; ocjena = nullptr;
		delete godina; godina = nullptr;
		if (Napomena != nullptr) {
			delete[] Napomena;
			Napomena = nullptr;
		}
	}
};

struct student {
	int brojindeksa;
	char *imeprezime;
	NacinStudiranja nacin;
	GodinaStudija godina;
	polozeniPredmet *polozeni[40];
	int *brojpolozenih;
	void unos(int indeks, char *_imeprezime, NacinStudiranja *_nacin, GodinaStudija *_godina) {
		brojindeksa = indeks;
		int v = strlen(_imeprezime) + 1;
		imeprezime = new char[v];
		strcpy_s(imeprezime, v, _imeprezime);
		nacin = *_nacin;
		godina = *_godina;
		brojpolozenih = new int(0);
	}
	void Dealociraj() {
		delete[] imeprezime; imeprezime = nullptr;
		if (*brojpolozenih > 0)
			for (int i = 0; i < *brojpolozenih; i++) {
				polozeni[i]->dealociraj();
				delete polozeni[i]; polozeni[i] = nullptr;
			}
		delete brojpolozenih; brojpolozenih = nullptr;
	}
	void ispis() {
		cout << "Broj indeksa: " << brojindeksa << endl;
		cout << "Ime i prezime: " << imeprezime << endl;
		cout << "Nacin studiranja: ";
		if (nacin == 3)cout << "REDOVAN\n"; else cout << "DL\n";
		cout << "Godina: " << godinaStudija[godina - 1] << endl;
		cout << "Broj polozenih predmeta: " << *brojpolozenih << endl;
		if (*brojpolozenih > 0)
			for (int i = 0; i < *brojpolozenih; i++)
				polozeni[i]->ispis();
	}
	void ispis(GodinaStudija *godina) {
		if (*brojpolozenih > 0)
			for (int i = 0; i < *brojpolozenih; i++)
				if (*polozeni[i]->godina == *godina)polozeni[i]->ispis();
	}
	bool dodajpolozeni(polozeniPredmet *p) {
		if (*p->ocjena < 6)return false;
		if (*brojpolozenih > 0) {
			for (int i = 0; i < *brojpolozenih; i++)
				if (strcmp(polozeni[i]->naziv, p->naziv) == 0 && *p->ocjena > *polozeni[i]->ocjena) {
					delete polozeni[i];
					polozeni[i] = new polozeniPredmet;
					polozeni[i]->unos(p->naziv, *p->ocjena, *p->godina, p->Obavezni);
					return true;
				}
				else if (strcmp(polozeni[i]->naziv, p->naziv) == 0 && (*p->ocjena == *polozeni[i]->ocjena || *p->ocjena < *polozeni[i]->ocjena))return false;
		}
		if (*brojpolozenih == 40)return false;
		polozeni[*brojpolozenih] = new polozeniPredmet;
		polozeni[*brojpolozenih]->unos(p->naziv, *p->ocjena, *p->godina, p->Obavezni);
		if (p->Napomena != nullptr)
			polozeni[(*brojpolozenih)++]->setNapomena(p->Napomena);
		return true;
	}
	bool uklonipolozeni(char *naziv, GodinaStudija *godina) {
		if (*brojpolozenih > 0)
			for (int i = 0; i < *brojpolozenih; i++)
				if (strcmp(polozeni[i]->naziv, naziv) == 0 && *polozeni[i]->godina == *godina) {
					delete polozeni[i];
					polozeni[i] = nullptr;
					for (int j = i; j < *brojpolozenih - 1; j++)swap(polozeni[j], polozeni[j + 1]);
					(*brojpolozenih)--;
					return true;
				}
	}
	int getbrojpolozenih(bool obavezni) {
		int brp = 0;
		for (int i = 0; i < *brojpolozenih; i++)
			if (polozeni[i]->Obavezni == obavezni)brp++;
		return brp;
	}
};

polozeniPredmet *getPolozeniPredmeti(student *pok, int max,int &brs) {
	char pretraga[100];
	cout << "Unesite rijec ili tekst prema kojem zelite vrsiti pretragu po napomeni u polozenim predmetima" << endl;
	cin.getline(pretraga, 99);
	for (int i = 0; i<max; i++) 
		for (int j = 0; j < *pok[i].brojpolozenih; j++) 
			if (pok[i].polozeni[j]->Napomena != nullptr) {
				char *p = strstr(pok[i].polozeni[j]->Napomena, pretraga);
				if (pok != 0)brs++;

			}
	polozeniPredmet *niz = new polozeniPredmet[brs];
	int x = 0;
	for (int i = 0; i<max; i++)
		for (int j = 0; j < *pok[i].brojpolozenih; j++)
			if (pok[i].polozeni[j]->Napomena != nullptr) {
				char *p = strstr(pok[i].polozeni[j]->Napomena, pretraga);
				if (pok != 0) {
					niz[x++].unos(pok[i].polozeni[j]->naziv, *pok[i].polozeni[j]->ocjena, *pok[i].polozeni[j]->godina, pok[i].polozeni[j]->Obavezni);
					if (pok[i].polozeni[j]->Napomena != nullptr)niz[x-1].setNapomena(pok[i].polozeni[j]->Napomena);
				}

			}
	return niz;
}
int main() {
	int max;
	cout << "Unesite koliko zelite studenata inicijalizovati: ";
	cin >> max;
	cin.ignore();
	student *s = new student[max];
	
	polozeniPredmet p1, p2, p3, p4, p5, p6, p7, p8;
	p1.unos("Programinje II", 7, PRVA, false);
	p1.setNapomena("Potrebno usavrsavanje optimizacije koda");
	p2.unos("Arhitektura Racunara", 6, PRVA, false);
	p2.setNapomena("Odradjeno za prolaz");
	p3.unos("Inzinjerska Matematika", 7, PRVA, true);
	p3.setNapomena("Solidno odradjeno");
	p4.unos("Diskretna matematika", 6, PRVA, true);
	p5.unos("Baze podataka II", 8, DRUGA, false);
	p5.setNapomena("Fino odradjeno");
	p6.unos("Engleski jezik III",7, DRUGA, true);
	p7.unos("Baze podataka I",8, DRUGA, true);
	p8.unos("Sistemska i mrezna administracija",7, DRUGA, true);
	
	GodinaStudija gs1 = DRUGA;
	NacinStudiranja ns1 = REDOVAN;
	s[0].unos(160262, "Fahir Mumdzic",&ns1,&gs1 );
	s[1].unos(160126, "Hana Jusufovic",&ns1,&gs1 );
	if (s[0].dodajpolozeni(&p1))
		cout << "Predmet uspjesno dodan..\n";
	if (s[0].dodajpolozeni(&p2))
		cout << "Predmet uspjesno dodan..\n";
	if (s[0].dodajpolozeni(&p3))
		cout << "Predmet uspjesno dodan..\n";
	if (s[0].dodajpolozeni(&p4))
		cout << "Predmet uspjesno dodan..\n";
	if (s[0].dodajpolozeni(&p5))
		cout << "Predmet uspjesno dodan..\n";
	if (s[0].dodajpolozeni(&p6))
		cout << "Predmet uspjesno dodan..\n";

	if (s[1].dodajpolozeni(&p7))
		cout << "Predmet uspjesno dodan..\n";
	if (s[1].dodajpolozeni(&p8))
		cout << "Predmet uspjesno dodan..\n";
	if (s[1].dodajpolozeni(&p5))
		cout << "Predmet uspjesno dodan..\n";
	if (s[1].dodajpolozeni(&p6))
		cout << "Predmet uspjesno dodan..\n";


	if (s[0].uklonipolozeni("Engleski jezik III", &gs1))cout << "Predmet uspjesno uklonjen..\n";

	cout << "\n\nBroj uspjesno polozenih predmeta koji su obavezni je: " << s[0].getbrojpolozenih(true) << endl;
	int brs = 0;
	polozeniPredmet *ss = getPolozeniPredmeti(s, max,brs);
	if (brs > 0) {
		cout << "--> LISTA PREDMETA KOJI U NAPOMENI PSADRZE UNIJETU RIJEC <---\n";
		for (int i = 0; i < brs; i++)
			ss[i].ispis();
	}
	else cout << "\nNISMO PRONASLI NI JEDAN PREDMET KOJI U NAPOMENI SADRZI UNIJETU RIJEC\n";

	cout << "---- PODACI O STUDENTU -----\n";
	s[0].ispis();
	for (int i = 0; i < max; i++)s[i].Dealociraj();
	if (max > 0)delete[] s;
	s = nullptr;
	system("pause");
}