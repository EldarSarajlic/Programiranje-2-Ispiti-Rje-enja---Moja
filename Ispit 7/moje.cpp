#include <iostream>
#include <regex>
#include <thread>
#include <mutex>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
using namespace std;

const char* crt = "\n-------------------------------------------\n";
enum Pojas { BIJELI, ZUTI, NARANDZASTI, ZELENI, PLAVI, SMEDJI, CRNI };
ostream& operator<<(ostream& COUT, const Pojas& indeks) {
	const char* niz[] = { "BIJELI", "ZUTI", "NARANDZASTI", "ZELENI", "PLAVI", "SMEDJI", "CRNI" };
	COUT << niz[indeks];
	return COUT;
}
const int brojTehnika = 6;
const char* NIJE_VALIDNA = "<VRIJEDNOST_NIJE_VALIDNA>";
char* GetNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}
/*
	za autentifikaciju svaki korisnik mora posjedovati lozinku koja sadrzi:
	- najmanje 7 znakova
	- velika i mala slova
	- najmanje jedan broj
   - najmanje jedan specijalni znak
	za provjeru validnosti lozinke koristiti globalnu funkciju
   ValidirajLozinku, a unutar nje regex metode.
	validacija lozinke se vrsi unutar konstruktora klase Korisnik, a u
   slucaju da nije validna
	postaviti je na podrazumijevanu vrijednost: <VRIJEDNOST_NIJE_VALIDNA>*/
bool ValidirajLozinku(string lozinka) {
	if (lozinka.size() < 7)
		return false;
	regex maloSlovo{ "[a-z]" };
	regex velikoSlovo{ "[A-Z]" };
	regex broj{ "\\d" };
	regex specZnak{ "\\W" };

	return regex_search(lozinka, maloSlovo)
		&& regex_search(lozinka, velikoSlovo)
		&& regex_search(lozinka, broj)
		&& regex_search(lozinka, specZnak);
}

template<class T1, class T2, int max = 10>
class Kolekcija {
	T1* _elementi1[max] = { nullptr };
	T2* _elementi2[max] = { nullptr };
	int* _trenutno;
public:
	Kolekcija() {
		_trenutno = new int(0);
	}
	Kolekcija(const Kolekcija& obj) {
		_trenutno = new int(*obj._trenutno);
		for (size_t i = 0; i < getTrenutno(); i++)
		{
			_elementi1[i] = new T1(obj.getElement1(i));
			_elementi2[i] = new T2(obj.getElement2(i));
		}
	}
	Kolekcija& operator=(const Kolekcija& obj) {
		if (this != &obj) {
			for (size_t i = 0; i < *_trenutno; i++) {
				delete _elementi1[i]; _elementi1[i] = nullptr;
				delete _elementi2[i]; _elementi2[i] = nullptr;
			}
			delete _trenutno; _trenutno = nullptr;
			_trenutno = new int(*obj._trenutno);
			for (size_t i = 0; i < getTrenutno(); i++)
			{
				_elementi1[i] = new T1(obj.getElement1(i));
				_elementi2[i] = new T2(obj.getElement2(i));
			}
		}
		return *this;
	}
	~Kolekcija() {
		for (size_t i = 0; i < *_trenutno; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
		delete _trenutno; _trenutno = nullptr;
	}
	T1& getElement1(int lokacija)const { return *_elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return *_elementi2[lokacija]; }
	int getTrenutno()const { return *_trenutno; }

	friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < *obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}

	void AddElement(T1 el1, T2 el2, int lokacija = 0) {
		if (lokacija == 0) {
			if (getTrenutno() == max)
				throw exception("Kolekcija popunjena!\n");
			_elementi1[getTrenutno()] = new T1(el1);
			_elementi2[getTrenutno()] = new T2(el2);
			(*_trenutno)++;
		}
		else {
			if (getTrenutno() == max)
				throw exception("Kolekcija popunjena!\n");
			for (size_t i = getTrenutno(); i > lokacija; i--)
			{
				_elementi1[i] = _elementi1[i - 1];
				_elementi2[i] = _elementi2[i - 1];
			}
			_elementi1[lokacija] = new T1(el1);
			_elementi2[lokacija] = new T2(el2);
			(*_trenutno)++;
		}
	}

	void RemoveAt(int lokacija) {
		delete _elementi1[lokacija]; _elementi1[lokacija] = nullptr;
		delete _elementi2[lokacija]; _elementi2[lokacija] = nullptr;
		for (size_t i = lokacija; i < getTrenutno(); i++)
		{
			_elementi1[i] = _elementi1[i + 1];
			_elementi2[i] = _elementi2[i + 1];
		}
		_elementi1[getTrenutno() - 1] = nullptr;
		_elementi2[getTrenutno() - 1] = nullptr;
		(*_trenutno)--;
	}

	T1& operator[](const T1& el1) {
		for (size_t i = 0; i < getTrenutno(); i++)
		{
			if (getElement1(i) == el1)
				return getElement2(i);
		}
	}
};
class Datum {
	int* _dan, * _mjesec, * _godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	Datum(const Datum& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
	}
	Datum& operator=(const Datum& obj) {
		if (this != &obj) {
			delete _dan; _dan = nullptr;
			delete _mjesec; _mjesec = nullptr;
			delete _godina; _godina = nullptr;
			_dan = new int(*obj._dan);
			_mjesec = new int(*obj._mjesec);
			_godina = new int(*obj._godina);
		}
		return *this;
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream& operator<< (ostream& COUT, const Datum& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina;
		return COUT;
	}
	int getGodina()const { return *_godina; }
	int getMjesec()const { return *_mjesec; }
	int getDan()const { return *_dan; }

	int toInt()const {
		return getGodina() * 365 + getMjesec() * 30 + getDan();
	}

	int operator-(const Datum& datum) {
		return abs((toInt()) - (datum.toInt()));
	}

	bool operator>(const Datum& datum) {
		return (toInt()) > (datum.toInt());
	}

	bool operator==(const Datum& datum) {
		return getGodina() == datum.getGodina()
			&& getMjesec() == datum.getMjesec()
			&& getDan() == datum.getDan();
	}
	bool operator!=(const Datum& datum) {
		return !(*this == datum);
	}
};
class Tehnika {
	char* _naziv;
	//int se odnosi na ocjenu u opsegu od 1 – 5, a Datum na datum kada je ocijenjena odredjena tehnika
	Kolekcija<int, Datum*, brojTehnika> _ocjene;
public:
	Tehnika(const char* naziv) {
		_naziv = GetNizKaraktera(naziv);
	}
	Tehnika(const Tehnika& obj) {
		_naziv = GetNizKaraktera(obj._naziv);
		for (size_t i = 0; i < obj._ocjene.getTrenutno(); i++)
		{
			_ocjene.AddElement(obj._ocjene.getElement1(i), new Datum(*obj._ocjene.getElement2(i)));
		}
	}
	Tehnika& operator=(const Tehnika& obj) {
		if (this != &obj) {
			delete[] _naziv; _naziv = nullptr;
			for (size_t i = 0; i < _ocjene.getTrenutno(); i++)
			{
				delete _ocjene.getElement2(i);
				_ocjene.getElement2(i) = nullptr;
			}
			_naziv = GetNizKaraktera(obj._naziv);
			for (size_t i = 0; i < obj._ocjene.getTrenutno(); i++)
			{
				_ocjene.AddElement(obj._ocjene.getElement1(i), new Datum(*obj._ocjene.getElement2(i)));
			}
		}
		return *this;
	}
	~Tehnika() {
		delete[] _naziv; _naziv = nullptr;
		for (size_t i = 0; i < _ocjene.getTrenutno(); i++)
		{
			delete _ocjene.getElement2(i);
			_ocjene.getElement2(i) = nullptr;
		}
	}
	char* GetNaziv() const { return _naziv; }
	Kolekcija<int, Datum*, brojTehnika>& GetOcjene() { return _ocjene; }

	/*ispisuje: naziv tehnike, ocjene (zajedno sa datumom) i prosjecnu ocjenu za tu tehniku
		ukoliko tehnika nema niti jednu ocjenu prosjecna treba biti 0*/
	friend ostream& operator<<(ostream& COUT, const Tehnika& obj) {
		COUT << "Naziv: " << obj._naziv << endl;
		cout << "Ocjene: "<<endl;
		for (size_t i = 0; i < obj._ocjene.getTrenutno(); i++)
		{
			COUT << obj._ocjene.getElement1(i) << " (" << *obj._ocjene.getElement2(i) << ")"<<endl;
		}
		COUT << "Prosjecna ocjena tehnike: " << obj.prosjecnaOcjenaTehnike() << endl;
		return COUT;
	}
	
	float prosjecnaOcjenaTehnike() const {
		float suma = 0.0f;
		if (_ocjene.getTrenutno() == 0)
			return suma;
		for (size_t i = 0; i < _ocjene.getTrenutno(); i++)
		{
			suma += _ocjene.getElement1(i);
		}
		return suma / _ocjene.getTrenutno();
	}

	/*svaka tehnika moze imati vise ocjena tj. moze se polagati u vise
   navrata.
	- razmak izmedju polaganja dvije tehnike mora biti najmanje 3 dana
	- nije dozvoljeno dodati ocjenu sa ranijim datumom u odnosu na vec
   evidentirane (bez obzira sto je stariji od 3 dana)*/
	bool AddOcjena(int ocjena, const Datum& datum) {
		for (size_t i = 0; i < _ocjene.getTrenutno(); i++)
		{
			if ((*_ocjene.getElement2(i) - datum) < 3)
				return false;
			if (*_ocjene.getElement2(i) > datum)
				return false;
		}
		_ocjene.AddElement(ocjena, new Datum(datum));
		return true;
	}
	/*
	char* _naziv;
	//int se odnosi na ocjenu u opsegu od 1 – 5, a Datum na datum kada je ocijenjena odredjena tehnika
	Kolekcija<int, Datum*, brojTehnika> _ocjene;
	*/
	bool operator==(const Tehnika& tehnika) {
		if (strcmp(_naziv, tehnika._naziv) != 0)
			return false;
		if (_ocjene.getTrenutno() != tehnika._ocjene.getTrenutno())
			return false;
		for (size_t i = 0; i < _ocjene.getTrenutno(); i++)
		{
			if (_ocjene.getElement1(i) != tehnika._ocjene.getElement1(i))
				return false;
			if (*_ocjene.getElement2(i) != *tehnika._ocjene.getElement2(i))
				return false;
		}
		return true;
	}
};
 
class Polaganje {
	Pojas _pojas;
	vector<Tehnika*> _polozeneTehnike;
public:
	Polaganje(Pojas pojas = BIJELI) {
		_pojas = pojas;
	}
	Polaganje(const Polaganje& obj) {
		_pojas = obj._pojas;
		for (size_t i = 0; i < obj._polozeneTehnike.size(); i++)
		{
			_polozeneTehnike.push_back(new Tehnika(*obj._polozeneTehnike[i]));
		}
	}
	Polaganje(const Pojas& pojas, const Tehnika& tehnika) {
		_pojas = pojas;
		_polozeneTehnike.push_back(new Tehnika(tehnika));
	}
	Polaganje& operator=(const Polaganje& obj) {
		if (this != &obj) {
			for (size_t i = 0; i < _polozeneTehnike.size(); i++) {
				delete _polozeneTehnike[i];
				_polozeneTehnike[i] = nullptr;
			}
			_pojas = obj._pojas;
			for (size_t i = 0; i < obj._polozeneTehnike.size(); i++)
			{
				_polozeneTehnike.push_back(new Tehnika(*obj._polozeneTehnike[i]));
			}
		}
		return *this;
	}
	~Polaganje() {
		for (size_t i = 0; i < _polozeneTehnike.size(); i++) {
			delete _polozeneTehnike[i];
			_polozeneTehnike[i] = nullptr;
		}
	}
	vector<Tehnika*>& GetTehnike() { return _polozeneTehnike; }
	Pojas GetPojas() { return _pojas; }

	friend ostream& operator<< (ostream& COUT, const Polaganje& obj) {
		COUT << obj._pojas << endl;
		for (size_t i = 0; i < obj._polozeneTehnike.size(); i++)
			COUT << *obj._polozeneTehnike[i];
		return COUT;
	}

	float prosjecnaOcjenaSvihTehnika() const {
		float suma = 0.0f;
		if (_polozeneTehnike.size() == 0)
			return suma;
		for (size_t i = 0; i < _polozeneTehnike.size(); i++)
		{
			suma += _polozeneTehnike[i]->prosjecnaOcjenaTehnike();
		}
		return suma / _polozeneTehnike.size();
	}
};

class Korisnik {
	char* _imePrezime;
	string _emailAdresa;
	string _lozinka;
public:
	Korisnik(const char* imePrezime, string emailAdresa, string lozinka)
	{
		_imePrezime = GetNizKaraktera(imePrezime);
		_emailAdresa = emailAdresa;
		ValidirajLozinku(lozinka) ? _lozinka = lozinka : _lozinka = NIJE_VALIDNA;
	}
	Korisnik(const Korisnik& obj) {
		_imePrezime = GetNizKaraktera(obj._imePrezime);
		_emailAdresa = obj._emailAdresa;
		_lozinka = obj._lozinka;
	}
	Korisnik& operator=(const Korisnik& obj) {
		if (this != &obj) {
			delete[] _imePrezime; _imePrezime = nullptr;
			_imePrezime = GetNizKaraktera(obj._imePrezime);
			_emailAdresa = obj._emailAdresa;
			_lozinka = obj._lozinka;
		}
		return *this;
	}
	virtual ~Korisnik() { 
		delete[] _imePrezime; _imePrezime = nullptr; 
	}
	string GetEmail() { return _emailAdresa; }
	string GetLozinka() { return _lozinka; }
	char* GetImePrezime() { return _imePrezime; }
	virtual void info() = 0;
};

mutex m;
class KaratePolaznik : public Korisnik{
	vector<Polaganje*> _polozeniPojasevi;
	/*nakon evidentiranja tehnike na bilo kojem pojasu kandidatu se salje email sa porukom:
	FROM:info@karate.ba
	TO: emailKorisnika
	Postovani ime i prezime, evidentirana vam je thenika X za Y pojas.
   Dosadasnji uspjeh (prosjek ocjena) na pojasu Y iznosi F, a ukupni uspjeh (prosjek ocjena) na svim pojasevima iznosi Z.
   Fakultet informacijskih tehnologija
   Pismeni dio ispita
   Pozdrav.
	KARATE Team.
	slanje email poruka implemenitrati koristeci zasebne thread-ove.
	*/
	void posaljiEMail(const char* tehnika, const Pojas& pojas, float prosjekNaPojasu, float prosjekSvihPojasa) {
		m.lock();
		cout << "FROM:info@karate.ba" << endl;
		cout << "TO: " << GetEmail() << endl;
		cout << "Postovani " << GetImePrezime() << ", evidentirana vam je tehnika " << tehnika << " za " << pojas << " pojas." << endl;
		cout << "Dosadasnji uspjeh na pojasu " << pojas << " iznosi " << prosjekNaPojasu << ", a ukupni uspjeh na svim pojasevima iznosi " << prosjekSvihPojasa << endl;
		cout << "Fakultet informacijskih tehnologija" << endl;
		cout << "Pismeni dio ispita" << endl;
		cout << "Pozdrav." << endl;
		cout << "KARATE Team." << endl;
		m.unlock();
	}
public:
	KaratePolaznik(const char* imePrezime, string emailAdresa, string lozinka) : Korisnik(imePrezime,emailAdresa,lozinka){}
	KaratePolaznik(const KaratePolaznik& obj) : Korisnik(obj){
		for (size_t i = 0; i < obj._polozeniPojasevi.size(); i++)
		{
			_polozeniPojasevi.push_back(new Polaganje(*obj._polozeniPojasevi[i]));
		}
	}
	KaratePolaznik& operator=(const KaratePolaznik& obj) {
		if (this != &obj) {
			for (size_t i = 0; i < _polozeniPojasevi.size(); i++) {
				delete _polozeniPojasevi[i];
				_polozeniPojasevi[i] = nullptr;
			}
			for (size_t i = 0; i < obj._polozeniPojasevi.size(); i++)
			{
				_polozeniPojasevi.push_back(new Polaganje(*obj._polozeniPojasevi[i]));
			}
			Korisnik::operator=(obj);
		}
		return *this;
	}
	~KaratePolaznik() {
		cout << crt << "DESTRUKTOR -> KaratePolaznik" << crt;
		for (size_t i = 0; i < _polozeniPojasevi.size(); i++) {
			delete _polozeniPojasevi[i];
			_polozeniPojasevi[i] = nullptr;
		}
	}
	friend ostream& operator<< (ostream& COUT, KaratePolaznik& obj) {
		COUT << obj.GetImePrezime() << " " << obj.GetEmail() << " " << obj.GetLozinka() << endl;
		for (size_t i = 0; i < obj._polozeniPojasevi.size(); i++)
			COUT << *obj._polozeniPojasevi[i];
		return COUT;
	}
	vector<Polaganje*>& GetPolozeniPojasevi() { return _polozeniPojasevi; }

	void info() {
		cout << *this;
	}

	/*
	svi kandidati podrazumijevano imaju BIJELI pojas (za njega se ne dodaju tehnike)
	sve tehnike na nivou jednog pojasa (ZUTI, ZELENI ... ) se evidentiraju unutar istog objekta tipa Polaganje,
	tom prilikom onemoguciti:
	- dodavanje istih (moraju biti identicne vrijednosti svih atributa) tehnika na nivou jednog pojasa,
	- dodavanje tehnika za visi pojas ako prethodni pojas nema evidentirane najmanje 3 tehnike ili nema prosjecnu ocjenu svih tehnika vecu od 3.5
	(onemoguciti dodavanje tehnike za NARANDZASTI ako ne postoji najmanje 3 tehnike za ZUTI pojas i njihov prosjek je veci od 3.5)
	funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja
	*/
	
	bool AddTehniku(const Pojas& pojas, const Tehnika& tehnika) {
		for (size_t i = 0; i < _polozeniPojasevi.size(); i++)
		{
			if (_polozeniPojasevi[i]->GetPojas() == pojas) {
				for (size_t j = 0; j < _polozeniPojasevi[i]->GetTehnike().size(); j++)
				{
					if (*_polozeniPojasevi[i]->GetTehnike()[j] == tehnika)
						return false;
				}
				_polozeniPojasevi[i]->GetTehnike().push_back(new Tehnika(tehnika));
				thread t1(&KaratePolaznik::posaljiEMail, this, tehnika.GetNaziv(), pojas, _polozeniPojasevi[i]->prosjecnaOcjenaSvihTehnika(), prosjekSvihPojasa());
				t1.join();
				return true;
			}
		}
		if (_polozeniPojasevi.size() == 0) {
			if (pojas - Pojas::BIJELI == 1) {
				_polozeniPojasevi.push_back(new Polaganje(pojas, tehnika));
				thread t2(&KaratePolaznik::posaljiEMail, this, tehnika.GetNaziv(), pojas, _polozeniPojasevi.back()->prosjecnaOcjenaSvihTehnika(), prosjekSvihPojasa());
				t2.join();
				return true;
			}
			else
				return false;
		}
		else {
			if (pojas - _polozeniPojasevi.back()->GetPojas() == 1) {
				if (_polozeniPojasevi.back()->GetTehnike().size() < 3)
					return false;
				if (_polozeniPojasevi.back()->prosjecnaOcjenaSvihTehnika() < 3.5)
					return false;
				_polozeniPojasevi.push_back(new Polaganje(pojas, tehnika));
				thread t3(&KaratePolaznik::posaljiEMail, this, tehnika.GetNaziv(), pojas, _polozeniPojasevi.back()->prosjecnaOcjenaSvihTehnika(), prosjekSvihPojasa());
				t3.join();
				return true;
			}
			else
				return false;
		}
		
	}

	float prosjekSvihPojasa() {
		float suma = 0.0f;
		if (_polozeniPojasevi.size() == 0)
			return false;
		for (size_t i = 0; i < _polozeniPojasevi.size(); i++)
		{
			suma += _polozeniPojasevi[i]->prosjecnaOcjenaSvihTehnika();
		}
		return suma / _polozeniPojasevi.size();
	}
};
const char* GetOdgovorNaPrvoPitanje() {
	cout << "Pitanje -> Pojasnite ulogu operatora const_cast?\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
const char* GetOdgovorNaDrugoPitanje() {
	cout << "Pitanje -> Ukratko opisite redoslijed kreiranja objekta bazne klase u slucaju visestrukog nasljedjivanja(prilikom instanciranja objekta najizvedenije klase), te koja su moguca rjesenja najznacajnijih problema ubtom kontekstu ? \n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
void main() {
	/*cout << PORUKA;
	cin.get();
	cout << GetOdgovorNaPrvoPitanje() << endl;
	cin.get();
	cout << GetOdgovorNaDrugoPitanje() << endl;
	cin.get();*/

	Datum datum19062020(19, 6, 2020),
		datum20062020(20, 6, 2020),
		datum30062020(30, 6, 2020),
		datum05072020(5, 7, 2020);
	int kolekcijaTestSize = 10;
	Kolekcija<int, int> kolekcija1;
	for (int i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, i);
	cout << kolekcija1 << endl;
	try {
		/*metoda AddElement baca izuzetak u slucaju da se pokusa
		dodati vise od maksimalnog broja elemenata*/
		kolekcija1.AddElement(11, 11);
	}
	catch (exception& err) {
		cout << crt << "Greska -> " << err.what() << crt;
	}
	cout << kolekcija1 << crt;
	kolekcija1.RemoveAt(2);
	/*uklanja par (T1 i T2) iz kolekcije koji se nalazi na lokaciji sa
   proslijedjenim indeksom.
	nakon uklanjanja vrijednosti onemoguciti pojavu praznog prostora unutar
   kolekcije tj.
	pomjeriti sve elemente koji se nalaze nakon proslijedjene lokacije za
   jedno mjesto unazad
	npr. ako unutar kolekcije postoje elementi
	0 0
	1 1
	2 2
	3 3
	nakon uklanjanja vrijednosti na lokaciji 1, sadrzaj kolekcije ce biti
   sljedeci
	0 0
	2 2
	3 3
	*/
	cout << kolekcija1 << crt;
	kolekcija1.AddElement(9, 9, 2);
	/*funkciji AddElement se, kao treci parametar, moze proslijediti i
   lokacija na koju se dodaju
	nove vrijednosti pri cemu treba zadrzati postojece vrijednosti pomjerene
   za jedno mjesto unaprijed
	u odnosu na definisanu lokaciju npr. ako unutar kolekcije postoje
   elementi
	0 0
	1 1
	2 2
	3 3
	nakon dodavanja vrijednosti 9 i 9 na lokaciju 1, sadrzaj kolekcije ce
   biti sljedeci
	0 0
	9 9
	1 1
	2 2
   3 3
	*/
	cout << kolekcija1 << crt;
	Kolekcija<int, int> kolekcija2 = kolekcija1;
	cout << kolekcija1 << crt;
	//na osnovu vrijednosti T2 mijenja vrijednost T1.
	kolekcija1[9] = 2;
	/* npr.ako unutar kolekcije postoje elementi:
	0 0
	9 9
	1 1
	2 2
	3 3
	nakon promjene vrijednosti sadrzaj kolekcije ce biti sljedeci
	0 0
	2 9
	1 1
	2 2
	3 3
	*/

	Tehnika choku_zuki("choku_zuki"),
		gyaku_zuki("gyaku_zuki"),
		kizami_zuki("kizami_zuki"),
		oi_zuki("oi_zuki");

	if (choku_zuki.AddOcjena(1, datum19062020))
		cout << "Ocjena evidentirana!" << endl;
	if (!choku_zuki.AddOcjena(5, datum20062020))
		cout << "Ocjena NIJE evidentirana!" << endl;
	if (choku_zuki.AddOcjena(5, datum30062020))
		cout << "Ocjena evidentirana!" << endl;

	cout << choku_zuki << endl;

	if (ValidirajLozinku("john4Do*e"))
		cout << "OK" << crt;
	if (!ValidirajLozinku("john4Doe"))
		cout << "Specijalni znak?" << crt;
	if (!ValidirajLozinku("jo*4Da"))
		cout << "7 znakova?" << crt;
	if (!ValidirajLozinku("4jo-hnoe"))
		cout << "Veliko slovo?" << crt;
	if (ValidirajLozinku("@john2Doe"))
		cout << "OK" << crt;

	Korisnik* jasmin = new KaratePolaznik("Jasmin Azemovic","jasmin@karate.ba", "j@sm1N*");
	Korisnik* adel = new KaratePolaznik("Adel Handzic", "adel@edu.karate.ba","4Ade1*H");
	Korisnik* lozinkaNijeValidna = new KaratePolaznik("John Doe","john.doe@google.com", "johndoe");

	//doraditi klase da nacin da omoguce izvrsenje naredne linije koda
	KaratePolaznik* jasminPolaznik = dynamic_cast<KaratePolaznik*>(jasmin);

	if (jasminPolaznik != nullptr) {
		if (jasminPolaznik->AddTehniku(ZUTI, gyaku_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;
		//ne treba dodati kizami_zuki jer ne postoje evidentirane 3 tehnike za ZUTI pojas
		if (!jasminPolaznik->AddTehniku(NARANDZASTI, kizami_zuki))
			cout << "Tehnika NIJE uspjesno dodana!" << crt;
		if (jasminPolaznik->AddTehniku(ZUTI, kizami_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;
		if (jasminPolaznik->AddTehniku(ZUTI, oi_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;
		if (jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
			cout << "Tehnika uspjesno dodan!" << crt;
		//ne treba dodati choku_zuki jer je vec dodana za zuti pojas
		if (!jasminPolaznik->AddTehniku(ZUTI, choku_zuki))
			cout << "Tehnika NIJE uspjesno dodana!" << crt;
		//ispisuje sve dostupne podatke o karate polazniku
		cout << *jasminPolaznik << crt;
	}
	
	//osigurati da se u narednim linijama poziva i destruktor klase KaratePolaznik
	delete jasmin;
	delete adel;
	delete lozinkaNijeValidna;
	cin.get();
	system("pause>0");
}