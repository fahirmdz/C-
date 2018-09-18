#include<iostream>
#include<cstring>
#include<memory>
#include<functional>
#include<tuple>
using namespace std;


const char * not_set = "NEMA_VRIJEDNOST";
const char * crt = "\n-------------------------------------------\n";
const int min_polgavlja = 3;
const int min_karaktera_po_poglavlju = 30;
const int max_zavrsnih = 30;
template<class T>
int firstFree(T *niz[]) {
	int x = 0;
	if (niz[0] == nullptr)
		return 0;
	while (niz[x] != nullptr && x<max_zavrsnih) {
		if (niz[x + 1] == nullptr)
			return x + 1;
		x++;
	}
	return -1;
}
char * AlocirajNizKaraktera(const char * sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char * temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}

struct Poglavlje {
	char * _naslov;
	char * _sadrzaj;
	bool _prihvaceno;
	int _ocjena;//da bi se poglavlje smatralo prihvacenim ocjena mora biti u opsegu od 6 - 10
	void Unos(const char * naslov = nullptr, const char * sadrzaj = nullptr) {
		if (naslov == nullptr || sadrzaj == nullptr)
			return;
		_ocjena = 0;
		_prihvaceno = false;
		_naslov = AlocirajNizKaraktera(naslov);
		_sadrzaj = AlocirajNizKaraktera(sadrzaj);
	}
	void Dealociraj() {
		if (_naslov != nullptr)
			delete[] _naslov;
		_naslov = nullptr;
		if (_sadrzaj != nullptr)
			delete[] _sadrzaj;
		_sadrzaj = nullptr;
	}
	void Ispis() {
		if (_naslov == nullptr || _sadrzaj == nullptr)
			return;
		cout << endl << "Poglavlje -> " << _naslov << endl << _sadrzaj << endl;
		if (_prihvaceno)
			cout << "Ocjena: " << _ocjena << endl;;
	}
	void OcijeniPoglavlje(int ocjena) {
		if (ocjena < 5 || ocjena>10)
			return;
		_ocjena = ocjena;
		if (_ocjena > 5 && ocjena <= 10)
			_prihvaceno = true;
	}
};

struct ZavrsniRad {
	char * _brojIndeksa;
	char * _tema;
	Poglavlje * _poglavljaRada;
	int _trenutnoPoglavlja;
	char * _datumOdbrane;
	float _konacnaOcjena; //konacna ocjena zavrsnog rada predstavlja prosjek ocjena svih poglavlja u zavrsnom radu koja se izracunava u momentu zakazivanja odbrane

	void Unos(const char * brojIndeksa, const char * nazivTeme = nullptr) {
		if (brojIndeksa == nullptr || nazivTeme == nullptr)
			return;
		_konacnaOcjena = 0;
		_poglavljaRada = nullptr;
		_trenutnoPoglavlja = 0;
		_brojIndeksa = AlocirajNizKaraktera(brojIndeksa);
		_tema = AlocirajNizKaraktera(nazivTeme);
		_datumOdbrane = AlocirajNizKaraktera(not_set);
	}
	void Dealociraj() {
		if (_tema != nullptr)
			delete[] _tema;
		_tema = nullptr;
		delete[] _datumOdbrane; _datumOdbrane = nullptr;
		if (_brojIndeksa != nullptr)
			delete[] _brojIndeksa;
		_brojIndeksa = nullptr;
		if (_trenutnoPoglavlja > 0 && _poglavljaRada != nullptr) {
			for (int i = 0; i < _trenutnoPoglavlja; i++)
				_poglavljaRada[i].Dealociraj();
			delete[] _poglavljaRada; _poglavljaRada = nullptr;
		}
	}
	void Ispis() {
		if (_tema == nullptr)return;
		cout << "Tema rada: " << _tema << endl;
		cout << "Sadrzaj: " << endl;
		if (_trenutnoPoglavlja > 0 && _poglavljaRada != nullptr)
			for (int i = 0; i < _trenutnoPoglavlja; i++)
				_poglavljaRada[i].Ispis();
		else
			cout << "Nema poglavlja..\n";
		cout << crt << "--Informacije o odbrani rada--\n";
		if (strcmp(_datumOdbrane, not_set) != 0)
			cout << "Datum odbrane rada: " << _datumOdbrane;
		else cout << "Datum odbrane rada: NIJE ZAKAZANA ODBRANA";
		if (_konacnaOcjena > 4 && _konacnaOcjena <= 10)
			cout << endl << "Ocjena: " << _konacnaOcjena << endl;
		else cout << endl << "Ocjena: NEOCENJEN\n";
		cout << endl << endl;
	}
	/*u zavrsni rad dodaje novo poglavlje i njegov sadrzaj. ukoliko poglavlje vec postoji u zavrsnom radu, funkcija tom poglavlju treba 
	dodati novi sadrzaj i pri tome zadrzi postojeci (izmedju postojeceg i novog sadrzaja se dodaje prazan prostor). u slucaju da poglavlje
	ne postoji, ono se dodaje zajedno sa sadrzajem*/
	//parametri: nazivPoglavlja, sadrzajPoglavlja

	void DodajPoglavlje(const char *tema=nullptr,const char *sadrzaj=nullptr) {
		if (tema == nullptr || sadrzaj == nullptr)
			return;
		int t = -1;
		if (_trenutnoPoglavlja > 0 && _poglavljaRada!=nullptr) {
			for (int i = 0; i < _trenutnoPoglavlja; i++)
				if (strcmp(_poglavljaRada[i]._naslov, tema) == 0) {
					t = i;
					break;
				}
			if (t >= 0) {
				if (_poglavljaRada[t]._sadrzaj == nullptr) {
					_poglavljaRada[t]._sadrzaj = AlocirajNizKaraktera(sadrzaj);
					return;
				}
				int v = strlen(_poglavljaRada[t]._sadrzaj) + 1;
				char *temp = new char[v];
				strcpy_s(temp, v, _poglavljaRada[t]._sadrzaj);
				delete[] _poglavljaRada[t]._sadrzaj;
				v += strlen(sadrzaj) + 1;
				_poglavljaRada[t]._sadrzaj = new char[v];
				strcpy_s(_poglavljaRada[t]._sadrzaj, v, temp);
				delete[] temp; temp = nullptr;
				strcat_s(_poglavljaRada[t]._sadrzaj, v, " ");
				strcat_s(_poglavljaRada[t]._sadrzaj, v, sadrzaj);
				return;
			}
			else {
				Poglavlje *temp = new Poglavlje[_trenutnoPoglavlja];
				for (int i = 0; i < _trenutnoPoglavlja; i++)
					swap(_poglavljaRada[i], temp[i]);
				delete[] _poglavljaRada;
				_poglavljaRada = new Poglavlje[_trenutnoPoglavlja + 1];
				for (int i = 0; i < _trenutnoPoglavlja; i++)
					swap(_poglavljaRada[i], temp[i]);
				delete[] temp; temp = nullptr;
			}
		}
		else if (_poglavljaRada == nullptr)
			_poglavljaRada = new Poglavlje[_trenutnoPoglavlja + 1];
		_poglavljaRada[_trenutnoPoglavlja++].Unos(tema, sadrzaj);
	}

	void OcijeniPoglavlje(const char *naziv, int ocjena) {
		if (naziv == nullptr || ocjena < 5 || ocjena>10)
			return;
		if (_trenutnoPoglavlja <= 0 || _poglavljaRada == nullptr)
			return;
		int t = -1;
		for (int i = 0; i < _trenutnoPoglavlja; i++)
			if (strcmp(_poglavljaRada[i]._naslov, naziv) == 0)
				t = i;
		if (t == -1) {
			cout << "Nismo pronasli poglavlje sa unijetim nazivom..\n";
			return;
		}
		_poglavljaRada[t].OcijeniPoglavlje(ocjena);
	}
};
struct Nastavnik {
	char * _imePrezime;
	ZavrsniRad * _teme[max_zavrsnih] = { nullptr };

	void Unos(const char * imePrezime=nullptr) {
		if (imePrezime == nullptr)
			return;
		_imePrezime = AlocirajNizKaraktera(imePrezime);
	}
	void Dealociraj() {
		if (_imePrezime != nullptr)
			delete[] _imePrezime;
		_imePrezime = nullptr;
		for (size_t i = 0; i < max_zavrsnih; i++)
		{
			if (_teme[i] != nullptr) {
				_teme[i]->Dealociraj();
				delete _teme[i];
			}
		}
	}
	void Ispis() {
		if (_imePrezime == nullptr)
			return;
		cout << crt << _imePrezime << crt;
		cout << "--LISTA TEMA ZA ODBRANU--\n";
		for (size_t i = 0; i < max_zavrsnih; i++)
			if (_teme[i] != nullptr)
				_teme[i]->Ispis();
	}
	/*funkcija DodajZavrsniRad ima zadatak da odredjenom nastavniku dodijeli mentorstvo na zavrsnom radu. sprijeciti dodavanje zavrsnih radova
	sa istom temom kao i mogucnost da jedan student kod istog nastavnika posjeduje vise zavrsnih radova*/

	bool DodajZavrsniRad(ZavrsniRad zr) {
		int sl = firstFree(_teme);
		if (sl == -1) {
			cout << "Sva mjesta su popunjena! Zao nam je..\n";
			return false;
		}
		for (int i = 0; i < sl; i++) {
			if (_teme[i]->_tema != nullptr && zr._tema != nullptr)
				if (strcmp(_teme[i]->_tema, zr._tema) == 0) {
					cout << "Izabrana tema vec postoji..\n";
					return false;
				}
			if (_teme[i]->_brojIndeksa != nullptr && zr._brojIndeksa != nullptr)
				if (strcmp(_teme[i]->_brojIndeksa, zr._brojIndeksa) == 0) {
					cout << "Student vec prijavljen..\n";
					return false;
				}
		}
		_teme[sl] = new ZavrsniRad;
		_teme[sl]->Unos(zr._brojIndeksa, zr._tema);
		if (zr._trenutnoPoglavlja > 0 && zr._poglavljaRada!=nullptr)
			for (int i = 0; i < zr._trenutnoPoglavlja; i++) {
				_teme[sl]->DodajPoglavlje(zr._poglavljaRada[i]._naslov, zr._poglavljaRada[i]._sadrzaj);
				if (zr._poglavljaRada[i]._ocjena > 0)
					_teme[sl]->_poglavljaRada[i].OcijeniPoglavlje(zr._poglavljaRada[i]._ocjena);
			}
		return true;
	}
	/*funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto podrazumijeva
	izracunavanje konacne ocjene i definisanje datuma odbrane. odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio.
	Za zakazivanje odbrane rada moraju biti zadovoljeni sljedeci uslovi:
	1. zavrsni rad mora imati broj poglavlja veci od minimalnog
	2. svako poglavlje mora imati broj karaktera veci od minimalnog
	3. svako poglavlje mora biti prihvaceno/odobreno
	ukoliko su zadovoljeni prethodni kriteriji, izracunava se konacna ocjena rada (prosjek ocjena svih poglavlja), postavlja datum odbrane rada
	i vraca pokazivac na rad kome je zakazana odbrana.
	u slucaju da student sa primljenim brojem indeksa nije prijavio zavrsni rad ili neki od postavljenih kriterija nije zadovoljen, funkcija vraca 
	nullptr.
	*/
	ZavrsniRad *ZakaziOdbranuRada(const char *brojIndeksa,const char *datum) {
		if (brojIndeksa == nullptr || datum == nullptr)
			return nullptr;

		int x = 0, t = -1;
		while (_teme[x] != nullptr) {
			if (strcmp(_teme[x]->_brojIndeksa, brojIndeksa) == 0) {
				t = x;
				break;
			}
			x++;
		}
		if (t == -1) {
			cout << "Student sa unijetim broj indeksa nema prijavljen rad..\n";
			return nullptr;
		}
		
		if (_teme[t]->_trenutnoPoglavlja < min_polgavlja || _teme[t]->_trenutnoPoglavlja<=0)
			return nullptr;
		for (int i = 0; i < _teme[t]->_trenutnoPoglavlja; i++)
			if (strlen(_teme[t]->_poglavljaRada[i]._sadrzaj) < min_karaktera_po_poglavlju || !_teme[t]->_poglavljaRada[i]._prihvaceno)
				return nullptr;
		float ocj = 0;
		for (int i = 0; i < _teme[t]->_trenutnoPoglavlja; i++)
			ocj += _teme[t]->_poglavljaRada[i]._ocjena;
		ocj /= (float)_teme[t]->_trenutnoPoglavlja;
		if (ocj - (int)ocj >= 0.5)
			_teme[t]->_konacnaOcjena = (int)ocj + 1;
		else
			_teme[t]->_konacnaOcjena = (int)ocj;
		if (_teme[t]->_datumOdbrane != nullptr)
			delete[] _teme[t]->_datumOdbrane;
		_teme[t]->_datumOdbrane = new char[strlen(datum) + 1];
		strcpy_s(_teme[t]->_datumOdbrane, strlen(datum) + 1, datum);
		return _teme[t];
	}
};

/*funkcija PronadjiNajStudenta ima zadatak da pronadje prvog studenta koji je zavrsni rad odbranio kod
nastavnika/mentora koji ima najnizu prosjecnu ocjenu radova (sumaSvihOcjenaZavrsnihRadova/brojZavrsnihRadova), te
tom prilikom ostvario (odnosi se na studenta) ocjenu vecu od proslijedjene (npr. 8.2)*/

tuple<char*, float, float>PronadjiNajStudenta(Nastavnik *niz[], int max, float prosjek) {
	if (max <= 0)
		return make_tuple(nullptr, NULL, NULL);
	float *prosjeci = new float[max] {0};
	int *brojTema = new int[max] {0};
	for (int i = 0; i < max; i++)
		for (int j = 0; j < max_zavrsnih; j++)
			if (niz[i]->_teme[j] != nullptr)
				if (niz[i]->_teme[j]->_konacnaOcjena >= 5 && niz[i]->_teme[j]->_konacnaOcjena <= 10) {
					prosjeci[i] += niz[i]->_teme[j]->_konacnaOcjena;
					brojTema[i]++;
				}
	for (int i = 0; i < max; i++)
		prosjeci[i] /= (float)brojTema[i];

	delete[] brojTema; brojTema = nullptr;
	int najmMent = 0;
	for (int i = 1; i < max; i++)
		if (prosjeci[najmMent] > prosjeci[i])
			najmMent = i;

	int student = -1;
	for (int i = 0; i < max_zavrsnih; i++)
		if(niz[najmMent]->_teme[i]!=nullptr)
			if (niz[najmMent]->_teme[i]->_konacnaOcjena > prosjek) {
				student = i;
				break;
			}
	if (student == -1)
		return make_tuple(nullptr, NULL, NULL);
	return make_tuple(niz[najmMent]->_teme[student]->_brojIndeksa, prosjeci[najmMent], niz[najmMent]->_teme[student]->_konacnaOcjena);
	
}
int main() {
	const int max = 2;
	Nastavnik * nastavnici[max];

	nastavnici[0] = new Nastavnik;    
	nastavnici[0]->Unos("Denis Music");
	nastavnici[1] = new Nastavnik;     
	nastavnici[1]->Unos("Emina Junuz");

	ZavrsniRad multimedijalni;
	//parametri: brojIndeksa, tema
	multimedijalni.Unos("IB120021", "Multimedijalni informacijski sistem za visoko - obrazovnu ustanovu");
	ZavrsniRad podrsa_operaterima;
	podrsa_operaterima.Unos("IB130031", "Sistem za podršku rada kablovskog operatera");
	ZavrsniRad analiza_sigurnosti;
	analiza_sigurnosti.Unos("IB140041", "Prakticna analiza sigurnosti bežiènih raèunarskih mreža");
	ZavrsniRad kriptografija;
	kriptografija.Unos("IB120021", "Primjena teorije informacija u procesu generisanja kriptografskih kljuèeva");

	
	multimedijalni.DodajPoglavlje("Uvod", "U ovom poglavlju ce biti rijeci");
	multimedijalni.DodajPoglavlje("Uvod", "o multimedijalnim sistemima koji se danas koriste");
	multimedijalni.DodajPoglavlje("Uvod", "u savremenom poslovanju");
	multimedijalni.DodajPoglavlje("Vrste multimedijalnih sistema", "Danas se moze govoriti o nekoliko vrsta multimedijalnih sistema, a neke od najznacajnijih su ...");
	multimedijalni.DodajPoglavlje("Teorija multimedije", "Sadrzaj koji bi trebao stajati na pocetku treceg poglavlja zavrsnog rada o multimediji studenta IB130011");

	//nazivPoglavlja, ocjena
	multimedijalni.OcijeniPoglavlje("Uvod", 8);
	multimedijalni.OcijeniPoglavlje("Vrste multimedijalnih sistema", 6);
	multimedijalni.OcijeniPoglavlje("Teorija multimedije", 10);

	

	 //brojIndeksa, zavrsniRad
		if (nastavnici[0]->DodajZavrsniRad(multimedijalni))
			cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (nastavnici[0]->DodajZavrsniRad(podrsa_operaterima))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (!nastavnici[0]->DodajZavrsniRad(podrsa_operaterima))//dupliranje rada, onemoguciti dodavanje
		cout << "Zavrsni rad nije dodat!" << endl;
	if (!nastavnici[0]->DodajZavrsniRad(kriptografija)) //studentu vec dodijeljen rad, onemoguciti dodavanje
		cout << "Zavrsni rad nije dodat!" << endl;
	if (nastavnici[1]->DodajZavrsniRad(analiza_sigurnosti))
		cout << "Zavrsni rad uspjesno dodat!" << endl;

	
	//paramteri: brojIndeksa, datumOdbrane
	ZavrsniRad * zr1 = nastavnici[0]->ZakaziOdbranuRada("IB120021", "25.09.2018");
	if (zr1 != nullptr)
		zr1->Ispis();
	zr1 = nastavnici[0]->ZakaziOdbranuRada("IB180081", "25.09.2018");//student sa brojem indeksa IB180081 jos uvijek nije prijavio rad
	if (zr1 == nullptr)
		cout << "Odbrana ne moze biti zakazana!" << endl;
	//ispisuje sve podatke o nastavniku i njegovim mentorstvima
	nastavnici[0]->Ispis();
	nastavnici[1]->Ispis();

	
	float prosjekStudenta = 0, prosjekMentora = 0;
	char * indeksStudenta;
	//parametri PronadjiNajStudenta: nastavnici, brojNastavnika, minimalnaKonacnaOcjena
	tie(indeksStudenta, prosjekMentora, prosjekStudenta) = PronadjiNajStudenta(nastavnici, max, 7.9);
	if (indeksStudenta != nullptr)
		cout << crt << indeksStudenta << " " << prosjekMentora << " " << prosjekStudenta << crt;

	for (int i = 0; i < max; i++) {
		nastavnici[i]->Dealociraj();
		delete nastavnici[i];
		nastavnici[i] = nullptr;
	}
	system("PAUSE");
	return 0;
}