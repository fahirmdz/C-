#include<iostream>
#include <iomanip>
#include <cstring>
using namespace std;

char * crt = "\n---------------------------\n";

struct Odgovor {
	bool tacan;
	char * tekstOdgovora;
};
struct Pitanje {
	char * tekstPitanja;
	Odgovor odgovori[5];
	int brojOdgovora;
	int brojBodova;
};
int PrikaziMenu() {
	int izbor;
	cout << crt << "\t::MENU::" << crt;
	cout << "1. Dodaj pitanje" << endl;
	cout << "2. Promijeni odgovor na pitanje" << endl;
	cout << "3. Pokreni kviz" << endl;
	cout << "4. Prikazi rezultat" << crt;
	cout << "5. Kraj" << crt;
	do {
		cout << "Vas izbor -> ";
		cin >> izbor;
	} while (izbor < 1 || izbor>5);
	system("cls");
	return izbor;
}
void DodajPitanje(Pitanje *ispitPRII[], int &tr) {
	cout << crt << "::DODAVANJE NOVOG PITANJA::" << crt;
	cout << "Tekst pitanja: ";
	ispitPRII[tr] = new Pitanje;
	ispitPRII[tr]->tekstPitanja = new char[100];
	cin.getline(ispitPRII[tr]->tekstPitanja, 100);
	cout << "\nUNOS ODGOVORA ZA PITANJE\n";
	cout << "Koliko ponudjenih odgovora ima pitanje: ";
	cin >> ispitPRII[tr]->brojOdgovora;
	cin.ignore();
	for (int i = 0; i < ispitPRII[tr]->brojOdgovora; i++) {
		char tn;
		cout << i + 1 << ". odgovor: ";
		ispitPRII[tr]->odgovori[i].tekstOdgovora = new char[50];
		cin.getline(ispitPRII[tr]->odgovori[i].tekstOdgovora, 50);
		do {
			cout << "Tacan ili netacan(T-N): ";
			cin >> tn;
			if (toupper(tn) != 'T' && toupper(tn) != 'N')
				cout << "GRESKA! Morate unijeti T ili N!\n";
			if (toupper(tn) == 'T')
				ispitPRII[tr]->odgovori[i].tacan = true;
			else
				ispitPRII[tr]->odgovori[i].tacan = false;
		} while (toupper(tn) != 'T' && toupper(tn) != 'N');
		cin.ignore();
	}
	cin.ignore();
	cout << "Koliko bodova nosi pitanje: ";
	cin >> ispitPRII[tr]->brojBodova;
	tr++;
}
void PromijeniOdgovor(Pitanje *pitanja[],int trp) {
	char srch[50];
	bool flag = false;
	cout << crt << "ODJEL ZA PROMJENU ODGOVORA" << crt;
	cout << "Unesite odgovor ili dio odgovora za pretragu -> ";
	cin.getline(srch, 50);
	for (int i = 0; i < trp; i++) {
		for (int j = 0; j < pitanja[i]->brojOdgovora; j++) {
			char *p = strstr(pitanja[i]->odgovori[j].tekstOdgovora, srch);
			if (p != NULL) {
				char x;
				cout << "Odgovor " << pitanja[i]->odgovori[j].tekstOdgovora << " pronadjen u pitanju: " << pitanja[i]->tekstPitanja << endl;
				do {
					cout << "Da li je ovo odgovor koji biste zeljeli promijeniti (d/n) -> ";
					cin >> x;
					if (toupper(x) != 'D' && toupper(x) != 'N')cout << "Morate unijeti D ili N.. Ponovite unos!\n";
				} while (toupper(x) != 'D' && toupper(x) != 'N');
				cin.ignore();
				if (toupper(x) == 'D') {
					cout << "Unesite novi odgovor -> ";
					char novi[50];
					cin.getline(novi, 50);
					delete[] pitanja[i]->odgovori[j].tekstOdgovora;
					pitanja[i]->odgovori[j].tekstOdgovora = nullptr;
					pitanja[i]->odgovori[j].tekstOdgovora = new char[50];
					strcpy_s(pitanja[i]->odgovori[j].tekstOdgovora, 50, novi);
					do {
						cout << "Da li zelite postaviti novi odgovor kao tacan (d/n) -> ";
						cin >> x;
						if (toupper(x) != 'D' && toupper(x) != 'N')cout << "Morate unijeti D ili N.. Ponovite unos!\n";
					} while (toupper(x) != 'D' && toupper(x) != 'N');
					if (toupper(x) == 'D')pitanja[i]->odgovori[j].tacan = true;
					else pitanja[i]->odgovori[j].tacan = false;
				}
				flag = true;
				break;
			}
		}
		if (flag)break;
	}
}

int *PokreniKviz(Pitanje *ispitPRII[], int maxPitanja) {
	int *odgs = new int[maxPitanja];
	for (int i = 0; i < maxPitanja; i++) {
		cout << crt << i + 1 << ". " << ispitPRII[i]->tekstPitanja << crt;
		for (int j = 0; j < ispitPRII[i]->brojOdgovora; j++)
			cout << j + 1 << ". " << ispitPRII[i]->odgovori[j].tekstOdgovora << endl;

		int odg;
		cout << crt;
		do {
			cout << "Vas odgovor -> ";
			cin >> odg;
			if (odg<1 || odg>ispitPRII[i]->brojOdgovora)cout << "Unijeli ste nepostojeci odgovor..\nPonovite unos!\n";
			else odgs[i] = odg;
		} while (odg<1 || odg>ispitPRII[i]->brojOdgovora);
	}
	return odgs;
}
void PrikaziRezultat(Pitanje *ispitPRII[], int *odgovoriNaPitanja, int maxPitanja) {
	int ukupno = 0;
	int *tacni = new int[maxPitanja];
	for (int i = 0; i < maxPitanja; i++)
		tacni[i] = 0;
	for (int i = 0; i < maxPitanja;i++) 
		for (int j = 0; j < ispitPRII[i]->brojOdgovora; j++)
			if (odgovoriNaPitanja[i] == j+1 && ispitPRII[i]->odgovori[j].tacan) {
				ukupno += ispitPRII[i]->brojBodova;
				tacni[i] = 1;
			}
	cout << crt << "REZULTATI" << crt;
	int max = 0;
	for (int i = 0; i < maxPitanja; i++) {
		max += ispitPRII[i]->brojBodova;
		cout << i + 1 << ". pitanje -> ";
		if (tacni[i] == 1)cout << "TACNO\n";
		else cout << "NETACNO\n";
	}
	cout << crt << "USPJEH: " << ukupno << "/" << max << " (" <<setprecision(4)<< ((float)ukupno/max) * 100 << " %)" << crt;
	delete[] tacni; tacni = nullptr;
}
void Dealociraj(Pitanje *ispitPRII[], int max) {
	for (int i = 0; i < max; i++) {
		for (int j = 0; j < ispitPRII[i]->brojOdgovora; j++) {
			delete[] ispitPRII[i]->odgovori[j].tekstOdgovora;
			ispitPRII[i]->odgovori[j].tekstOdgovora = nullptr;
		}
		delete[] ispitPRII[i]->tekstPitanja;
		ispitPRII[i]->tekstPitanja = nullptr;
		delete ispitPRII[i]; ispitPRII = nullptr;
	}
}
void main() {
	const int maxPitanja = 6;
	int trenutnoPitanja = 0, izbor = 0, radjen = 0;
	Pitanje * ispitPRII[maxPitanja];
	int *odgovoriNaPitanja = nullptr;
	do {
		izbor = PrikaziMenu();
		cin.ignore();
		if (izbor == 1) {
			if (trenutnoPitanja < maxPitanja)
				DodajPitanje(ispitPRII, trenutnoPitanja);
			else cout << "Ne mozete unijeti vise pitanja..\n";
		}
		else if (izbor == 2) {
			if (radjen == 0)
				PromijeniOdgovor(ispitPRII, trenutnoPitanja);
			else cout << "Ne mozete mijenjati pitanja ukoliko je kviz radjen!\n";
			cin.ignore();
		}
		else if (izbor == 3) {
			
			if (trenutnoPitanja > 3) {
				if (radjen == 0) {
					odgovoriNaPitanja = PokreniKviz(ispitPRII, trenutnoPitanja);
					radjen = 1;
				}
				else cout << "Kviz je vec radjen..\n";
			}
			else cout << "Kviz mora imati vise od 3 pitanja  da bi se pokrenuo..\n";
			
			cin.ignore();
		}
		else if (izbor == 4) {
			if (radjen == 1)
				PrikaziRezultat(ispitPRII, odgovoriNaPitanja, trenutnoPitanja);
			cin.ignore();
			
		}

	} while (izbor != 5);
	Dealociraj(ispitPRII, trenutnoPitanja);
	delete[] odgovoriNaPitanja; odgovoriNaPitanja = nullptr;
}
