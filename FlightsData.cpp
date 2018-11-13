#include<iostream>
#include <cstring>
using namespace std;
enum Kompanija { Pegasus, TurkishAirlines, AustrianAirlines, FlyEmirates };
const char *kompanije[] = { "Pegasus","Turkish Airlines","Austrian Airlines","Fly Emirates" };
int ID = 1;
struct Putnik {
	int _putnikID;
	char * _imePrezime;
	float _brojPredjenihMilja;
	void Unos(char * imePrezime) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_brojPredjenihMilja = 0;
		_putnikID = ID++;
	}
	void Info() {
		cout << "[" << _putnikID << "] " << _imePrezime <<
			" (" << _brojPredjenihMilja << " milja)" << endl;
	}
	void Dealociraj() { delete[] _imePrezime; _imePrezime = nullptr; }
	void DodajPredjeneMilje(int brojMilja) { _brojPredjenihMilja += brojMilja; }
};
struct Rezervacija {
	Putnik * _putnik;
	int _oznakaSjedista;
	float _cijena;
	void Unos(Putnik putnik, int oznaka, float cijena) {
		_putnik = new Putnik;
		_putnik->Unos(putnik._imePrezime);
		_oznakaSjedista = oznaka;
		_cijena = cijena;
	}
	void Info() {
		_putnik->Info();
		cout << "Sjediste: " << _oznakaSjedista << " Cijena: "
			<< _cijena << endl;
	}
	void Dealociraj() { _putnik->Dealociraj(); }
};
struct Let {
	Kompanija * _kompanija;
	char * _pocetak; //pocetna lokacija
	char * _destinacija;
	Rezervacija * _rezervacije;
	int _brojMjesta; //maksimalan broj mjesta na letu
	float _brojMilja; //odnosi se na duzinu leta – broj predjenih milja
	float _cijenaKarte;
	void Unos(Kompanija kompanija, char * pocetak, char * destinacija, int brojMjesta, float brojMilja, float cijena) {
		_kompanija = new Kompanija(kompanija);
		int size = strlen(pocetak) + 1;
		_pocetak = new char[size];
		strcpy_s(_pocetak, size, pocetak);
		size = strlen(destinacija) + 1;
		_destinacija = new char[size];
		strcpy_s(_destinacija, size, destinacija);
		_brojMjesta = brojMjesta;
		_rezervacije = new Rezervacija[_brojMjesta];
		Putnik * temp;
		temp = new Putnik;
		temp->Unos("<SLOBODNO MJESTO>");
		for (int i = 0; i < _brojMjesta; i++)
			_rezervacije[i].Unos(*temp, i, 0);
		_brojMilja = brojMilja;
		_cijenaKarte = cijena;
	}
	void Dealociraj() {
		delete[] _pocetak; _pocetak = nullptr;
		delete[] _destinacija; _destinacija = nullptr;
		for (int i = 0; i < _brojMjesta; i++)
			_rezervacije[i].Dealociraj();
		delete[] _rezervacije; _rezervacije = nullptr;
	}
	bool AddRezervaciju(Putnik * p) {
		/*na samom pocetku, funkcija treba ispisati listu svih SLOBODNIH sjedista na letu, te omoguciti odabir nekog do njih.
		prilikom formiranja cijene karte voditi racuna o broju predjenih milja; ako je putnik presao od 10000 do 50000 milja onda ostvaruje
		/*funkcija uklanja rezervaciju na sjedistu cija je oznaka proslijedjena kao parametar funkcije, te vraca vrijednost. U slucaju da
		oznaceno mjesto nije rezervisano ili ne postoji na tom letu, funkcija vraca vrijednost false */
		cout << "\n-----> Rezervacija sjedista <-------\n----------------------------------------------------------\n";
		int br = 0;
		for (int i = 0; i < _brojMjesta; i++)
			if (strcmp(_rezervacije[i]._putnik->_imePrezime, "<SLOBODNO MJESTO>") == 0)br++;
		if (br == 0) {
			cout << "Zao nam je.. Sva mjesta su popunjena.\n";
			return false;
		}
		cout << "Broj slobodnih mjesta: " << br << endl;

		for (int i = 0; i < _brojMjesta; i++) {
			if (strcmp(_rezervacije[i]._putnik->_imePrezime, "<SLOBODNO MJESTO>") == 0)
				cout << _rezervacije[i]._oznakaSjedista+1 << " ";
			if ((i+1) % 10 == 0)cout << endl;
		}
		int oz;
		do {
			cout << "---------------------------------------------\nUnesite zeljenu oznaku sjedista: ";
			cin >> oz;
			if (oz<1 || oz>_brojMjesta)cout << "Unijeli ste nepostojecu oznaku sjedista.. Ponovite unos!\n";
		} while (oz<1 || oz>_brojMjesta);
		cin.ignore();
		float c;
		oz -= 1;
		if (p->_brojPredjenihMilja >= 10000 && p->_brojPredjenihMilja <= 50000)c = _cijenaKarte - (_cijenaKarte*0.15);
		else c = _cijenaKarte;
		int t;
		for (int i = 0; i < _brojMjesta; i++)
			if (_rezervacije[i]._oznakaSjedista == oz)
				t = i;
		_rezervacije[t].Dealociraj();
		_rezervacije[t].Unos(*p, oz, c);
		return true;
	}
	/*funkcija uklanja rezervaciju na sjedistu cija je oznaka proslijedjena kao parametar funkcije, te vraca vrijednost. U slucaju da
	oznaceno mjesto nije rezervisano ili ne postoji na tom letu, funkcija vraca vrijednost false */
	bool UkloniRezervaciju(int oz) {
		int t = -1;
		for (int i = 0; i < _brojMjesta; i++)
			if (_rezervacije[i]._oznakaSjedista == oz) {
				t = i;
				break;
			}
		if (t == -1)return false;
		_rezervacije[t].Dealociraj();
		Putnik *temp;
		temp = new Putnik;
		temp->Unos("<SLOBODNO MJESTO>");
		_rezervacije[t].Unos(*temp, t, 0);
		return true;
	}

};
/*funkcija vraca broj milja koje je putnik p ostvario putujuci sa kompanijom k*/
float GetMiljeByKompanija(Let * letovi, int maxLetova, Kompanija k, Putnik p) {
	float uk = 0;
	for (int i = 0; i < maxLetova; i++)
		if (*letovi[i]._kompanija == k)
			for (int j = 0; j < letovi[i]._brojMjesta; j++)
				if (strcmp(letovi[i]._rezervacije[j]._putnik->_imePrezime,p._imePrezime)==0)
					uk += letovi[i]._brojMilja;
	return uk;

}
void main() {
	/*provjeriti validnost izvrsenja svih dostupnih funkcionalnosti. vrijednosti koje koristite prilikom testiranja mogu biti predefinisane tj.
	ne morate omogucavati korisniku da ih inicijalizuje.*/
	Let *letovi = new Let[4];
	int maxLetova = 4;
	letovi[0].Unos(Pegasus, "Sarajevo", "Dubai", 30, 8000, 450);
	letovi[1].Unos(TurkishAirlines, "Sarajevo", "Istanbul", 30, 2300, 270);
	letovi[2].Unos(Pegasus, "Sarajevo", "Doha", 30, 6500, 400);
	letovi[3].Unos(AustrianAirlines, "Sarajevo", "Bec", 30, 1700, 190);
	Putnik fahir, dzemil, uros, kerim, ali, ifo;
	fahir.Unos("Fahir ");
	dzemil.Unos("Dzemil");
	uros.Unos("Uros");
	kerim.Unos("Abdul ");
	ali.Unos("Ali ");
	ifo.Unos("Nazif");
	if (letovi[0].AddRezervaciju(&fahir))cout << "Putnik uspjesno dodan..\n";
	if (letovi[0].AddRezervaciju(&dzemil))cout << "Putnik uspjesno dodan..\n";
	if (letovi[0].AddRezervaciju(&uros))cout << "Putnik uspjesno dodan..\n";
	if (letovi[0].AddRezervaciju(&kerim))cout << "Putnik uspjesno dodan..\n";
	if (letovi[0].AddRezervaciju(&ali))cout << "Putnik uspjesno dodan..\n";
	if (letovi[0].AddRezervaciju(&ifo))cout << "Putnik uspjesno dodan..\n";

	if (letovi[1].AddRezervaciju(&fahir))cout << "Putnik uspjesno dodan..\n";
	if (letovi[1].AddRezervaciju(&uros))cout << "Putnik uspjesno dodan..\n";
	if (letovi[1].AddRezervaciju(&dzemil))cout << "Putnik uspjesno dodan..\n";


	if (letovi[2].AddRezervaciju(&fahir))cout << "Putnik uspjesno dodan..\n";
	if (letovi[2].AddRezervaciju(&uros))cout << "Putnik uspjesno dodan..\n";
	if (letovi[2].AddRezervaciju(&ifo))cout << "Putnik uspjesno dodan..\n";

	if (letovi[3].AddRezervaciju(&fahir))cout << "Putnik uspjesno dodan..\n";
	if (letovi[3].AddRezervaciju(&ali))cout << "Putnik uspjesno dodan..\n";
	if (letovi[3].AddRezervaciju(&dzemil))cout << "Putnik uspjesno dodan..\n";


	if (letovi[0].UkloniRezervaciju(5))cout << "Uspjesno uklonjena rezervacija..\n";
	for (int i = 0; i < maxLetova; i++)cout << kompanije[*letovi[i]._kompanija] << endl;
	cout << GetMiljeByKompanija(letovi, maxLetova, Pegasus, fahir) << endl;

	for (int i = 0; i < maxLetova; i++)
		letovi[i].Dealociraj();
}
