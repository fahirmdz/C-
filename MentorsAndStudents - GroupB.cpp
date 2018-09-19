#include<iostream>
#include<memory>
#include<cstring>
#include<tuple>
using namespace std;



const char * not_set = "NEMA_VRIJEDNOST";
const char * crt = "\n-------------------------------------------\n";
const int min_polgavlja = 3;
const int max_zavrsnih = 30;
const char * zabranjeneRijeci[] = { "ubiti","mrziti", "NC", "RE" };

bool zabranjenSadrzaj(char *sadrzaj) {
	if (sadrzaj == nullptr)
		return true;
	for (int i = 0; i < 4; i++)
	{
		char *p = strstr(sadrzaj, zabranjeneRijeci[i]);
		if (p != NULL)
			return true;
	}
	return false;
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
	char _brojIndeksa[15];
	char * _tema;
	shared_ptr<Poglavlje> * _poglavljaRada;
	int _trenutnoPoglavlja;
	char * _datumOdbrane;
	float _konacnaOcjena; //konacna ocjena zavrsnog rada predstavlja prosjek ocjena svih poglavlja u zavrsnom radu koja se izracunava u momentu zakazivanja odbrane

	void Unos(const char * brojIndeksa, const char * nazivTeme = nullptr) {
		if (brojIndeksa == nullptr || nazivTeme == nullptr)
			return;
		_konacnaOcjena = 0;
		strcpy_s(_brojIndeksa,15, brojIndeksa);
		_tema = AlocirajNizKaraktera(nazivTeme);
		_trenutnoPoglavlja = 0;
		_datumOdbrane = AlocirajNizKaraktera(not_set);
		_poglavljaRada = nullptr;
	}
	void Dealociraj() {
		if (_tema != nullptr)
			delete[] _tema;
		_tema = nullptr;
		if (_datumOdbrane != nullptr)
			delete[] _datumOdbrane;
		_datumOdbrane = nullptr;
		if (_trenutnoPoglavlja > 0 && _poglavljaRada != nullptr) {
			for (int i = 0; i < _trenutnoPoglavlja; i++)
				_poglavljaRada[i]->Dealociraj();
			delete[] _poglavljaRada;
		}
		_poglavljaRada = nullptr;
	}
	void Ispis() {
		if (_tema == nullptr) 
			return;
		cout << "Tema rada: " << _tema << endl;
		cout << "Sadrzaj: " << endl;
		if (_trenutnoPoglavlja >= 0 && _poglavljaRada != nullptr)
			for (int i = 0; i < _trenutnoPoglavlja; i++)
				_poglavljaRada[i]->Ispis();
		else cout << "Nema dostupnih poglavlja..\n";
		if (_datumOdbrane != nullptr)
			cout << "Datum odbrane rada: " << _datumOdbrane << endl;
		else cout << "Datum odbrane rada: NIJE JOS ZAKAZAN" << endl;
			if(_konacnaOcjena!=0)
			cout<<"Ocjena: " << _konacnaOcjena << endl;
			else cout << "Ocjena: NEOCIJENJEN\n";
	}
	void DodajPoglavlje(const char *tema = nullptr, const char *sadrzaj = nullptr) {
		if (tema == nullptr || sadrzaj == nullptr)
			return;
		int t = -1;
		if (_trenutnoPoglavlja > 0 && _poglavljaRada != nullptr) {
			for (int i = 0; i < _trenutnoPoglavlja; i++)
				if (strcmp(_poglavljaRada[i]->_naslov, tema) == 0) {
					t = i;
					break;
				}
			if (t >= 0) {
				if (_poglavljaRada[t]->_sadrzaj == nullptr) {
					_poglavljaRada[t]->_sadrzaj = AlocirajNizKaraktera(sadrzaj);
					return;
				}
				int v = strlen(_poglavljaRada[t]->_sadrzaj) + 1;
				char *temp = new char[v];
				strcpy_s(temp, v, _poglavljaRada[t]->_sadrzaj);
				delete[] _poglavljaRada[t]->_sadrzaj;
				v += strlen(sadrzaj) + 1;
				_poglavljaRada[t]->_sadrzaj = new char[v];
				strcpy_s(_poglavljaRada[t]->_sadrzaj, v, temp);
				delete[] temp; temp = nullptr;
				strcat_s(_poglavljaRada[t]->_sadrzaj, v, " ");
				strcat_s(_poglavljaRada[t]->_sadrzaj, v, sadrzaj);
				return;
			}
			else {
				Poglavlje *temp = new Poglavlje[_trenutnoPoglavlja];
				for (int i = 0; i < _trenutnoPoglavlja; i++)
					swap(*_poglavljaRada[i], temp[i]);
				delete[] _poglavljaRada;
				_poglavljaRada = new shared_ptr<Poglavlje>[_trenutnoPoglavlja + 1];
				for (int i = 0; i < _trenutnoPoglavlja; i++) {
					_poglavljaRada[i] = make_shared<Poglavlje>(*(new Poglavlje));
					swap(*_poglavljaRada[i], temp[i]);
				}
				delete[] temp; temp = nullptr;
			}
		}
		else if (_poglavljaRada == nullptr)
			_poglavljaRada = new shared_ptr<Poglavlje>[_trenutnoPoglavlja + 1];
		_poglavljaRada[_trenutnoPoglavlja] = make_shared<Poglavlje>(*(new Poglavlje));
		_poglavljaRada[_trenutnoPoglavlja++]->Unos(tema, sadrzaj);
	} 

	void OcijeniPoglavlje(const char *naziv, int ocj) {
		if (naziv == nullptr || ocj < 5 || ocj>10)
			return;
		if (_trenutnoPoglavlja <= 0 || _poglavljaRada==nullptr)
			return;
		int t = -1;
		for (int i = 0; i < _trenutnoPoglavlja; i++) 
			if (strcmp(_poglavljaRada[i]->_naslov, naziv) == 0)
				t = i;
		if (t == -1) {
			cout << "Ne postoji poglavlje sa unijetim nazivom..\n";
			return;
		}
		_poglavljaRada[t]->OcijeniPoglavlje(ocj);
		
	}
};
struct Nastavnik {
	char * _imePrezime;
	shared_ptr<ZavrsniRad> _teme[max_zavrsnih] = { nullptr };

	void Unos(const char * imePrezime) {
		if (imePrezime == nullptr)
			return;
		_imePrezime = AlocirajNizKaraktera(imePrezime);
	}
	void Dealociraj() {
		if (_imePrezime != nullptr)
			delete[] _imePrezime;
		_imePrezime = nullptr;
		for (size_t i = 0; i < max_zavrsnih; i++)
			if (_teme[i] != nullptr) {
				_teme[i]->Dealociraj();
				_teme[i] = nullptr;
			}
	}
	void Ispis() {
		if (_imePrezime == nullptr)
			return;
		cout << crt << _imePrezime << crt;
		cout << "--LISTA RADOVA ZA ODBRANU--\n";
		for (size_t i = 0; i < max_zavrsnih; i++)
			if (_teme[i] != nullptr)
				_teme[i]->Ispis();
	}
	/*funkcija DodajZavrsniRad ima zadatak da odredjenom nastavniku dodijeli mentorstvo na zavrsnom radu. sprijeciti dodavanje zavrsnih radova 
	sa istom temom kao i mogucnost da jedan student kod istog nastavnika posjeduje vise zavrsnih radova*/

	bool DodajZavrsniRad(ZavrsniRad zr) {
		int sl = -1, x = 0;
		if (_teme[x] == nullptr)
			sl = x;
		else
			while (_teme[x] != nullptr && x < max_zavrsnih) {
				if (_teme[x + 1] == nullptr)
					sl = x + 1;
				x++;
			}

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
		_teme[sl] = make_shared<ZavrsniRad>(zr);
		return true;
	}
	/*funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto podrazumijeva
	izracunavanje konacne ocjene i definisanje datuma odbrane. odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio.
	Za zakazivanje odbrane rada moraju biti zadovoljeni sljedeci uslovi:
	1. zavrsni rad mora imati broj poglavlja veci od minimalnog
	2. sadrzaj bilo kojeg poglavlja u zavrsnom radu ne smije sadrzavati neku od zabranjenih rijeci (definisanih u nizu zabranjeneRijeci)
	3. svako poglavlje mora biti prihvaceno/odobreno
	ukoliko su zadovoljeni prethodni kriteriji, izracunava se konacna ocjena rada (prosjek ocjena svih poglavlja), postavlja datum odbrane rada i vraca pokazivac na rad kome je zakazana odbrana.
	u slucaju da student sa primljenim brojem indeksa nije prijavio zavrsni rad ili neki od postavljenih kriterija nije zadovoljen, funkcija vraca nullptr.
	*/
	//paramteri: brojIndeksa, datumOdbrane
	ZavrsniRad *ZakaziOdbranuRada(const char *brojIndeksa, const char *datum) {
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

		if (_teme[t]->_trenutnoPoglavlja < min_polgavlja || _teme[t]->_trenutnoPoglavlja <= 0)
			return nullptr;
		for (int i = 0; i < _teme[t]->_trenutnoPoglavlja; i++)
			if (zabranjenSadrzaj(_teme[t]->_poglavljaRada[i]->_sadrzaj) || !_teme[t]->_poglavljaRada[i]->_prihvaceno)
				return nullptr;
		float ocj = 0;
		for (int i = 0; i < _teme[t]->_trenutnoPoglavlja; i++)
			ocj += _teme[t]->_poglavljaRada[i]->_ocjena;
		ocj /= (float)_teme[t]->_trenutnoPoglavlja;
		if (ocj - (int)ocj >= 0.5)
			_teme[t]->_konacnaOcjena = ocj + 1;
		else
			_teme[t]->_konacnaOcjena = ocj;
		if (_teme[t]->_datumOdbrane != nullptr)
			delete[] _teme[t]->_datumOdbrane;
		_teme[t]->_datumOdbrane = new char[strlen(datum) + 1];
		strcpy_s(_teme[t]->_datumOdbrane, strlen(datum) + 1, datum);
		ZavrsniRad *g = new ZavrsniRad(*_teme[t]);
		return g;
	}
};
/*funkcija PronadjiNajStudenta ima zadatak da pronadje studenta koji kod nastavnika/mentora sa najvecom prosjecnom ocjenom radova
(sumaSvihOcjenaZavrsnihRadova/brojZavrsnihRadova) ima najvise neodobrenih (negativno ocijenjenih) poglavlja*/
tuple<char*, int, float> PronadjiStudenta(Nastavnik *niz, int max) {
	if (max <= 0)
		return make_tuple(nullptr, NULL, NULL);
	int brojStudenata = 0;
	float *prosjeci = new float[max] {0};
	int *brojTema = new int[max] {0};
	for (int i = 0; i < max; i++)
		for (int j = 0; j < max_zavrsnih; j++)
			if (niz[i]._teme[j] != nullptr) {
				brojStudenata++;
				if (niz[i]._teme[j]->_konacnaOcjena >= 5 && niz[i]._teme[j]->_konacnaOcjena <= 10) {
					prosjeci[i] += niz[i]._teme[j]->_konacnaOcjena;
					brojTema[i]++;
				}
			}
	for (int i = 0; i < max; i++)
		if(prosjeci[i]>0)
		prosjeci[i] /= (float)brojTema[i];

	delete[] brojTema; brojTema = nullptr;

	int najvMent = 0;
	for (int i = 1; i < max; i++)
		if (prosjeci[najvMent] < prosjeci[i])
			najvMent = i;
	int *brojacNeoc = new int[brojStudenata] {0};
	for (int i = 0; i < max_zavrsnih; i++) {
		if(niz[najvMent]._teme[i]!=nullptr)
			if(niz[najvMent]._teme[i]->_trenutnoPoglavlja>0 && niz[najvMent]._teme[i]->_poglavljaRada!=nullptr)
			for (int z = 0; z < niz[najvMent]._teme[i]->_trenutnoPoglavlja; z++) {
				if (niz[najvMent]._teme[i]->_poglavljaRada[z]->_ocjena == 5)
					brojacNeoc[i]++;
				
			}
	}
	int najviseNeocijenjenih = 0;
	for (int i = 1; i < brojStudenata; i++)
		if (brojacNeoc[i] > brojacNeoc[najviseNeocijenjenih])
			najviseNeocijenjenih = i;
	int temp = brojacNeoc[najviseNeocijenjenih];
	float tempProsjek = prosjeci[najvMent];
	delete[] prosjeci; prosjeci = nullptr;
	delete[] brojacNeoc; brojacNeoc = nullptr;
	return make_tuple(niz[najvMent]._teme[najviseNeocijenjenih]->_brojIndeksa, temp, tempProsjek);
	
}
int main() {
	const int max = 2;
	Nastavnik * nastavnici = new Nastavnik[max];

	nastavnici[0].Unos("Denis Music");
	nastavnici[1].Unos("Emina Junuz");

	ZavrsniRad multimedijalni;
	//parametri: brojIndeksa, tema
	multimedijalni.Unos("IB120021", "Multimedijalni informacijski sistem za visoko - obrazovnu ustanovu");
	ZavrsniRad podrsa_operaterima;
	podrsa_operaterima.Unos("IB130031", "Sistem za podršku rada kablovskog operatera");
	ZavrsniRad analiza_sigurnosti;
	analiza_sigurnosti.Unos("IB140041", "Prakticna analiza sigurnosti bežiènih raèunarskih mreža");
	ZavrsniRad kriptografija;
	kriptografija.Unos("IB120021", "Primjena teorije informacija u procesu generisanja kriptografskih kljuèeva");

	/*u zavrsni rad dodaje novo poglavlje i njegov sadrzaj. ukoliko poglavlje vec postoji u zavrsnom radu, funkcija tom poglavlju treba dodati novi sadrzaj i pri tome zadrzi postojeci (izmedju postojeceg i novog sadrzaja se dodaje prazan prostor). u slucaju da poglavlje ne postoji, ono se dodaje zajedno sa sadrzajem*/
	//parametri: nazivPoglavlja, sadrzajPoglavlja

	multimedijalni.DodajPoglavlje("Uvod", "U ovom poglavlju ce biti rijeci");
	multimedijalni.DodajPoglavlje("Uvod", "o multimedijalnim sistemima koji se danas koriste");
	multimedijalni.DodajPoglavlje("Uvod", "u savremenom poslovanju");
	multimedijalni.DodajPoglavlje("Vrste multimedijalnih sistema", "Danas se moze govoriti o nekoliko vrsta multimedijalnih sistema, a neke od najznacajnijih su ...");
	multimedijalni.DodajPoglavlje("Teorija multimedije", "Sadrzaj koji bi trebao stajati na pocetku treceg poglavlja zavrsnog rada o multimediji studenta IB130011");

	//parametri: nazivPoglavlja, ocjena
	multimedijalni.OcijeniPoglavlje("Uvod", 8);
	multimedijalni.OcijeniPoglavlje("Vrste multimedijalnih sistema", 6);
	multimedijalni.OcijeniPoglavlje("Teorija multimedije", 9);
	

	/*funkcija DodajZavrsniRad ima zadatak da odredjenom nastavniku dodijeli mentorstvo na zavrsnom radu. sprijeciti dodavanje zavrsnih radova sa istom temom kao i mogucnost da jedan student kod istog nastavnika posjeduje vise zavrsnih radova*/
	if (nastavnici[0].DodajZavrsniRad(multimedijalni))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (nastavnici[0].DodajZavrsniRad(podrsa_operaterima))
		cout << "Zavrsni rad uspjesno dodat!" << endl;
	if (!nastavnici[0].DodajZavrsniRad(podrsa_operaterima))//dupliranje rada, onemoguciti dodavanje
		cout << "Zavrsni rad nije dodat!" << endl;
	if (!nastavnici[0].DodajZavrsniRad(kriptografija)) //studentu vec dodijeljen rad, onemoguciti dodavanje
		cout << "Zavrsni rad nije dodat!" << endl;
	if (nastavnici[1].DodajZavrsniRad(analiza_sigurnosti))
		cout << "Zavrsni rad uspjesno dodat!" << endl;

	
	/*funkcija ZakaziOdbranuRada ima zadatak da studentu sa proslijedjenim brojem indeksa zakaze odbranu zavrsnog rada sto podrazumijeva
	izracunavanje konacne ocjene i definisanje datuma odbrane. odbrana rada se moze zakazati samo studentu koji je rad prethodno prijavio.
	Za zakazivanje odbrane rada moraju biti zadovoljeni sljedeci uslovi:
	1. zavrsni rad mora imati broj poglavlja veci od minimalnog
	2. sadrzaj bilo kojeg poglavlja u zavrsnom radu ne smije sadrzavati neku od zabranjenih rijeci (definisanih u nizu zabranjeneRijeci)
	3. svako poglavlje mora biti prihvaceno/odobreno
	ukoliko su zadovoljeni prethodni kriteriji, izracunava se konacna ocjena rada (prosjek ocjena svih poglavlja), postavlja datum odbrane rada i vraca pokazivac na rad kome je zakazana odbrana.
	u slucaju da student sa primljenim brojem indeksa nije prijavio zavrsni rad ili neki od postavljenih kriterija nije zadovoljen, funkcija vraca nullptr.
	*/
	//paramteri: brojIndeksa, datumOdbrane
	
	ZavrsniRad * zr1 = nastavnici[0].ZakaziOdbranuRada("IB120021", "25.09.2018");
	if (zr1 != nullptr)
		zr1->Ispis();

	zr1 = nastavnici[0].ZakaziOdbranuRada("IB180081", "25.09.2018");//student sa brojem indeksa IB180081 jos uvijek nije prijavio rad
	if (zr1 == nullptr)
		cout << "Odbrana ne moze biti zakazana!" << endl;


		nastavnici[0].Ispis();
	nastavnici[1].Ispis();


	
	int brojPoglavlja = -1;
	float prosjekMentora = 0;
	char * indeksStudenta;
	//parametri PronadjiStudenta: nastavnici, brojNastavnika
	tie(indeksStudenta, brojPoglavlja, prosjekMentora) = PronadjiStudenta(nastavnici, max);
	if (indeksStudenta != nullptr)
		cout << crt << indeksStudenta << " " << prosjekMentora << " " << brojPoglavlja << crt;

	for (int i = 0; i < max; i++)
		nastavnici[i].Dealociraj();
	delete[]nastavnici;
	nastavnici = nullptr;

	system("PAUSE");
	return 0;

}