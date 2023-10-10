#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <algorithm>

using namespace std;

enum Starost {Dijete=1, Student, Odrasli, Penzioner};
enum PovratnaKarta {Jednosmjerna, Povratna};
enum Legenda {SvakiDan=7, Praznik=6, Raspust=5};

struct Datum{
	int dan;
	int mjesec;
	int godina;
	
	bool DatumUnos(Datum datum){
		if(datum.godina<1000 || datum.godina>10000) return false;
		if(datum.dan<1 || datum.dan>31) return false;
		switch (datum.mjesec){
			case 1: case 3: case 5: case 7: case 8: case 10: case 12:
				if(datum.dan>31 || datum.dan<1) return false;
				else return true;
				break;
			case 2:
				if(datum.godina%4!=0 || (datum.godina%4==0 && datum.godina%100==0 && datum.godina%400!=0)){
					if(datum.dan>28 || datum.dan<1) return false;
					else return true;
				}else{
					if(datum.dan>29 || datum.dan<1) return false;
					else return true;
				}
				break;
			case 4: case 6: case 9: case 11:
				if(datum.dan>30 || datum.dan<1) return false;
				else return true;
				break;
			default:
				if(datum.mjesec>12 || datum.mjesec<1) return false;
				break;
		}
	}
	bool DatumProvjera(Datum prvi, Datum drugi){
		if(drugi.godina<prvi.godina) return false;
		else if(drugi.godina>prvi.godina) return true;
		else{
			if(drugi.mjesec<prvi.mjesec) return false;
			else if(drugi.mjesec>prvi.mjesec) return true;
			else{
				if(drugi.dan<prvi.dan) return false;
				else if(drugi.dan>prvi.dan) return true;
				else return true;
			}
		}
	}
};

time_t now = time(0);
tm *ltm = localtime(&now);
Datum danas;

struct Vrijeme{
	int sat;
	int minuta;
	
	bool VrijemeUnos(Vrijeme vrijeme){
		if(vrijeme.sat<0 || vrijeme.sat>23) return false;
		else{
			if(vrijeme.minuta<0 || vrijeme.minuta>59) return false;
			else return true;
		}
	}
	bool VrijemeProvjera(Vrijeme polazak, Vrijeme dolazak){
		if(polazak.sat==dolazak.sat || polazak.sat>dolazak.sat) return false;
		else if(polazak.sat<dolazak.sat) return true;
	}
};

struct Putnik{
	string ime;
	string prezime;
	string KontaktBroj;
	Starost uzrast;
	PovratnaKarta karta;
};

struct Autobus{
	string PolazakMjesto;
	string PovratakMjesto;
	Datum DatumPolaska;
	Vrijeme PolazakVrijeme;
	Datum DatumPovratka;
	Vrijeme PovratakVrijeme;
	string Agencija;
	double Cijena;
	int BrojMjesta;
	int ID;
	string ImeVozaca;
	string PrezimeVozaca;
};

void PretragaVozaca (Autobus Podaci[], string ime, string prezime, int i, int n, int br){
	if (i==n){
		if (br==0){
			cout << "\t\tUneseni vozac ne postoji ili nema zakazane voznje." << endl;
		}
	}else if((Podaci[i].ImeVozaca) == ime && (Podaci[i].PrezimeVozaca == prezime)){
		br++;
		cout << "\t\t" << br << ". " << Podaci[i].PolazakMjesto << "-" << Podaci[i].PovratakMjesto << endl;
		return PretragaVozaca (Podaci,ime,prezime,i+1,n,br);
	}else{
		return PretragaVozaca (Podaci,ime,prezime,i+1,n,br);
	}
}

struct LokalniAutobus{
	int velicina;
	string Polazak;
	string Povratak;
	Legenda Dan;
	Vrijeme VrijemePolaska[20];
	Vrijeme VrijemePovratka[20];
	double Karta;
	double Mjesecna;
};

string setPass(bool show_asterisk = true){
    const char BACKSPACE = 8;
    const char ENTER = 13;
    string pass = "";
    char c = ' ';
    while ((c = _getch()) != ENTER){
        if (c == BACKSPACE){
            if (pass.length() != 0){
                if (show_asterisk)
                   cout << "\b \b";
                pass.resize(pass.length() - 1); 
            }
        }else if (c == 0 || c == 224){
            _getch();
            continue;
        }else{
            pass.push_back(c);
            cout << '*';
        }
    }
    cout << endl;
    return pass;
}

int BrSlobodnihMjesta(int ID, int BrSlobodnih){
	string line, pomocna;
	pomocna = to_string(ID);
	pomocna = pomocna + ".txt";
	ifstream Putnici(pomocna);
	if(Putnici.fail()){
		return BrSlobodnih;
	}else{
		while(getline(Putnici,line)){
			BrSlobodnih--;
		}
	}
	return BrSlobodnih;
}

bool Prijava (){
	string line, username, password, temp;
	do{
	cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
	cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
	cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
	cout << "\t\n\t\tLOGIN\t" << endl;
	cout<<"\t"<<setfill('-')<<setw(20)<<"-"<<endl;
	cout << "\tUsername: ";
	getline (cin, username);
	cout << "\tPassword: ";
	password=setPass();
	ifstream Login ("Osoblje.txt");
	if(Login.fail()){
		cout<<"\t"<<setfill('-')<<setw(20)<<"-"<<endl;
		cout << "\n[ERROR] Datoteku nije moguce otvoriti!\n" << endl;
	}else{
		while(!Login.eof()){
			Login >> line;
			Login >> temp;
			if (line.compare(username)==0 && temp.compare(password)==0){
				return true;		
			}
		}
	}
	cout<<"\t"<<setfill('-')<<setw(20)<<"-"<<endl;
	cout << "\n[ERROR]\tPogresan username ili password!\n\tPokusajte ponovo.\n" << endl;
	system ("pause");
	system ("cls");
	}while(true);
}

int Meni1(){
	int izbor;
	cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
	cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
	cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
	cout << "\n\t\tMENI\n";
	cout<<"\t"<<setfill('-')<<setw(20)<<"-";
	cout << "\n\t1. Gradske linije\n";
	cout << "\t2. Vangradske linije\n";
	cout << "\t3. Log Out\n";
	cout << "\t4. Iskljuci program" << endl;
	cout<<"\t"<<setfill('-')<<setw(20)<<"-";
	cout << "\n\tVas izbor: ";
	if(!(cin >> izbor)){
			cout << "\n[ERROR]\tNevalidan unos!\n\tPokusajte ponovo.\n\n";
			cin.clear();
  			cin.ignore(1000, '\n');
			system ("pause");
			system ("cls");
			Meni1();
	}else{
		switch (izbor){
			case 1:
				return 1;
				break;
			case 2:
				return 2;
				break;
			case 3:
				return 3;
				break;
			case 4:
				return 4;
				break;	
			default:
				cout << "\n[ERROR]\tNevalidan unos!\n\tPokusajte ponovo.\n\n";
				system ("pause");
				system ("cls");
				Meni1();
		}
	}
}

int Meni2 (){
	int izbor;
	cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
	cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
	cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
	cout << "\n\t\tGRADSKE LINIJE\n";
	cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
	cout << "\t1. Ispis svih linija" << endl;
	cout << "\t2. Pretraga linija" << endl;
	cout << "\t3. Izlaz" << endl;
	cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
	cout << "\tVas izbor: ";
	cin >> izbor;
	switch (izbor){
	case 1:
		return 1;
		break;
	case 2:
		return 2;
		break;
	case 3:
		return 3;
		break;
	default:
		cout << "Pogresan unos! Pokusajte ponovo.\n";
		system ("pause");
		system ("cls");
		cin.clear();
		cin.ignore(1000,'\n');
		Meni2();
	}
}

int Meni3 (){
	cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
	cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
	cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
	cout << "\n\t\tVANGRADSKE LINIJE\n";
	cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
	int izbor;
	cout << "\t1. Rezervacije" << endl;
	cout << "\t2. Pretraga linija" << endl;
	cout << "\t3. Unos novih linija" << endl;
	cout << "\t4. Otkazivanje rezervacija" << endl;
	cout << "\t5. Izlaz" << endl;
	cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
	cout << "\tVas izbor: ";
	if(!(cin >> izbor)){
			cout << "\n[ERROR]\tNevalidan unos!\n\tPokusajte ponovo.\n\n";
			cin.clear();
  			cin.ignore(1000, '\n');
			system ("pause");
			system ("cls");
			Meni3();
	}else{
		switch (izbor){
			case 1:
				return 1;
				break;
			case 2:
				return 2;
				break;
			case 3:
				return 3;
				break;
			case 4:
				return 4;
				break;
			case 5:
				return 5;
				break;
			default:
				cout << "\n[ERROR]\tNevalidan unos!\n\tPokusajte ponovo.\n\n";
				system ("pause");
				system ("cls");
				cin.clear();
				cin.ignore(1000, '\n');
				Meni3();
		}
	}
}

int MeniPretraga (){
	cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
	cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
	cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
	cout << "\n\t     KATEGORIJE PRETRAGE\n";
	cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
	int izbor;
	cout << "\t1. Agencija" << endl;
	cout << "\t2. Destinacija" << endl;
	cout << "\t3. ID" << endl;
	cout << "\t4. Datum polaska" <<endl;
	cout << "\t5. Vozac" << endl;
	cout << "\t6. Izlaz" << endl;
	cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
	cout << "\tVas izbor: ";
	if(!(cin>>izbor)){
		cout << "\n[ERROR]\tNevalidan unos!\n\tPokusajte ponovo.\n\n";
		cin.clear();
		cin.ignore(1000, '\n');
		system ("pause");
		system ("cls");		
		MeniPretraga();
	}else{
		switch (izbor){
			case 1:
				return 1;
				break;
			case 2:
				return 2;
				break;
			case 3:
				return 3;
				break;
			case 4:
				return 4;
				break;
			case 5: 
				return  5;
				break;
			case 6:
				return 6;
			default:
				cout << "\n[ERROR]\tNevalidan unos!\n\tPokusajte ponovo.\n\n";
				system ("pause");
				system ("cls");
				cin.clear();
				cin.ignore(1000,'\n');
				MeniPretraga();
		}
	}
}

void Meni (){
	danas.godina = 1900 + ltm->tm_year;
	danas.mjesec = 1 + ltm->tm_mon;
	danas.dan = ltm->tm_mday;	
	int BrojLokalnih = 0;
	string line1;
	ifstream Lokalni("Lokalni.txt");
	if(Lokalni.fail()){
		cout << "\tDatoteka nije pronadjena!\n\t";
		system("pause");
		system("cls");
		Meni();
	}
	while(!Lokalni.eof()){
		getline(Lokalni,line1);
		BrojLokalnih++;
	}
	Lokalni.close();
	LokalniAutobus *Lokalac = new LokalniAutobus [BrojLokalnih];
	int h=0;
	Lokalni.open("Lokalni.txt");
	while(!Lokalni.eof()){
		getline(Lokalni,line1);
		int i=0, br=0, b=0, vel = 0;
		string provjera = "", pomocna;
		do{
			if(line1[i]==',') vel++;
			 if(line1[i] == '/'){
				br++;
				provjera = "";
				i++;
			}else if(line1[i]!='/'){
				provjera = provjera + line1[i];
				i++;
				if(line1[i+1]=='/'){
					provjera = provjera + line1[i];
					switch (br){
						case 0:
							Lokalac[h].Polazak = provjera;
							break;
						case 1:
							Lokalac[h].Povratak = provjera;
							break;
						case 2:
							{
							Lokalac[h].velicina = vel;
							Vrijeme a;
							pomocna = "";
							pomocna = pomocna + provjera[0] + provjera[1];
							a.sat = stoi(pomocna);
							a.minuta = 4;
							Lokalac[h].VrijemePolaska[0].sat = a.sat;
							pomocna = "";
							pomocna = pomocna + provjera[3] + provjera[4];
							Lokalac[h].VrijemePolaska[0].minuta = stoi(pomocna);
							int j=1, m=6;
							do{
								pomocna = "";
								pomocna = pomocna + provjera[m] + provjera[m+1];
								Lokalac[h].VrijemePolaska[j].sat = stoi(pomocna);
								pomocna = "";
								pomocna = pomocna + provjera[m+3] + provjera[m+4];
								Lokalac[h].VrijemePolaska[j].minuta = stoi(pomocna);
								m=m+6;
								j++;
							}while(m<provjera.size());
							break;}
						case 3: 
							{
							pomocna = "";
							pomocna = pomocna + provjera[0] + provjera[1];
							Lokalac[h].VrijemePovratka[0].sat = stoi(pomocna);
							pomocna = "";
							pomocna = pomocna + provjera[3] + provjera[4];
							Lokalac[h].VrijemePovratka[0].minuta = stoi(pomocna);
							int j=1, m=6;
							do{
								pomocna = "";
								pomocna = pomocna + provjera[m] + provjera[m+1];
								Lokalac[h].VrijemePovratka[j].sat = stoi(pomocna);
								pomocna = "";
								pomocna = pomocna + provjera[m+3] + provjera[m+4];
								Lokalac[h].VrijemePovratka[j].minuta = stoi(pomocna);
								m=m+6;
								j++;
							}while(m<provjera.size());
							break;}
						case 4:
							Lokalac[h].Karta = stod(provjera);
							break;
						case 5:
							Lokalac[h].Mjesecna = stod (provjera);
							break;
						case 6:
							if(provjera == "SvakiDan") Lokalac[h].Dan = SvakiDan;
							else if(provjera == "Praznik") Lokalac[h].Dan = Praznik;
							else Lokalac[h].Dan = Raspust;
							break;
					}
				}
			}
		}while(br<7);
		h++;
	}
	Lokalni.close();
	int BrojLinija = 0;
	string line;
	ifstream Autobusi("Autobusi.txt");
	if(Autobusi.fail()){
		cout << "\tDatoteka nije pronadjena!\n\t";
		system("pause");
		system("cls");
		Meni();
	}
	while(!Autobusi.eof()){
		getline(Autobusi,line);
		BrojLinija++;
	}
	Autobusi.close();
	Autobus *Podaci = new Autobus [BrojLinija];
	int k=0;
	Autobusi.open("Autobusi.txt");
	while(!Autobusi.eof()){
		string l = "";
		getline(Autobusi,line);
		int i=0, br=0;
		string provjera = "";
		do{
			if(line[i] == '/'){
				br++;
				provjera = "";
				i++;
			}else if(line[i]!='/'){
				provjera = provjera + line[i];
				i++;
				if(line[i+1]=='/'){
					provjera = provjera + line[i];
					switch (br){
						case 0:
							Podaci[k].PolazakMjesto = provjera;
							break;
						case 1:
							Podaci[k].PovratakMjesto = provjera;
							break;
						case 2:
							l = "";
							l = l + provjera[0]+provjera[1];
							Podaci[k].DatumPolaska.dan = stoi(l);
							l = "";
							l = l + provjera[3] + provjera[4];
							Podaci[k].DatumPolaska.mjesec = stoi(l);
							l = "";
							l = l + provjera[6] + provjera[7] + provjera[8] + provjera[9];
							Podaci[k].DatumPolaska.godina = stoi(l); 
							break;
						case 3: 
							l = "";
							l = l + provjera[0] + provjera[1];
							Podaci[k].PolazakVrijeme.sat = stoi(l);
							l = "";
							l = l + provjera[3] + provjera[4];
							Podaci[k].PolazakVrijeme.minuta = stoi(l);
							break;
						case 4:
							l = "";
							l = l + provjera[0]+provjera[1];
							Podaci[k].DatumPovratka.dan = stoi(l);
							l = "";
							l = l + provjera[3] + provjera[4];
							Podaci[k].DatumPovratka.mjesec = stoi(l);
							l = "";
							l = l + provjera[6] + provjera[7] + provjera[8] + provjera[9];
							Podaci[k].DatumPovratka.godina = stoi(l); 
							break;
						case 5:
							l = "";
							l = l + provjera[0] + provjera[1];
							Podaci[k].PovratakVrijeme.sat = stoi(l);
							l = "";
							l = l + provjera[3] + provjera[4];
							Podaci[k].PovratakVrijeme.minuta = stoi(l);
							break;
						case 6:
							Podaci[k].Agencija = provjera;
							break;
						case 7:
							Podaci[k].Cijena=stod(provjera);
							break;
						case 8:
							Podaci[k].BrojMjesta = stoi(provjera);
							break;
						case 9:
							Podaci[k].ID = stoi(provjera);
							break;
						case 10:
							Podaci[k].ImeVozaca = provjera;
							break;
						case 11:
							Podaci[k].PrezimeVozaca = provjera;
							break;
					}
				}
			}
		}while(br<12);
		k++;
	}
	Autobusi.close();
	char s;
	int PrviMeni;
	PrviMeni = Meni1();
	if(PrviMeni == 1){ // 1.1. Ispis svih gradskih linija
		system ("cls");
		int DrugiMeni;
		DrugiMeni = Meni2();
		if(DrugiMeni == 1){
			int odg;
			system ("cls");
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
			cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
			cout << "\n\t\tGRADSKE LINIJE\n";
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
			for (int i=0; i<h; i++){
				if(i==0){
					cout<<endl<<setfill('-')<<setw(40)<<"-"<<endl<<setfill(' ');
					cout<<setw(4)<<left<<" Br"<<setw(14)<<"|"<<setw(21)<<"Relacija"<<setw(9)<<"|";
					cout<<endl<<setfill('-')<<setw(40)<<"-"<<endl<<setfill(' ');
					}
				cout << " "<<i+1 <<setw(2)<< "." << setw(3)<<"|"<< setw(13)<<right<<Lokalac[i].Polazak<<" - "<<setw(16)<<left<<Lokalac[i].Povratak<<"|"<<endl;
			}
			cout<<setfill('-')<<setw(40)<<"-"<<endl<<setfill(' ');
			cout << "\n\nUnesite redni broj linije za detaljniji pregled (0 - IZLAZ): ";
			do{
				cin>>odg;
				if(cin.fail()){
					cin.clear();
					cin.ignore(1000, '\n');
					cout << "[ERROR] Nevalidan unos.\n\tPokusajte ponovo: ";			
				}else if(odg==0){
					system("cls");
					Meni();
				}else if(odg<0 || odg>h){
					cin.clear();
					cin.ignore(1000, '\n');
					cout << "[ERROR] Nevalidan unos.\n\tPokusajte ponovo: ";						
				}else if(odg>0 && odg<=h) break;
			}while(!cin.fail());
			system("cls");
			transform(Lokalac[odg].Polazak.begin(), Lokalac[odg].Polazak.end(), Lokalac[odg].Polazak.begin(), ::toupper);
			transform(Lokalac[odg].Povratak.begin(), Lokalac[odg].Povratak.end(), Lokalac[odg].Povratak.begin(), ::toupper);
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
			cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
			cout << "\n\t"<<setfill(' ')<<setw(13)<<right<<Lokalac[odg].Polazak<<" - "<<Lokalac[odg].Povratak<<endl;
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;			
			cout<<endl<<setfill('-')<<setw(56)<<"-"<<endl<<setfill(' ');
			cout<<setw(5)<<left<<" Br"<<setw(9)<<"|"<<setw(16)<<"Polazak"<<setw(9)<<"|"<<setw(16)<<"Povratak"<<setw(4)<<"|";
			cout<<endl<<setfill('-')<<setw(56)<<"-"<<endl<<setfill(' ');
			for(int i=0;i<=Lokalac[odg].velicina;i++){
				cout << right<<setw(3)<<i+1 << ". " << setw(10)<<left<<"|"<< setfill('0')<<right<<setw(2)<<Lokalac[odg].VrijemePolaska[i].sat<<":"<<setfill('0')<<setw(2)<<right<<Lokalac[odg].VrijemePolaska[i].minuta<<setfill(' ')<<setw(11)<<"|" <<setw(11)<<" "<<setfill('0')<<setw(2)<< Lokalac[odg].VrijemePovratka[i].sat<<":"<<setfill('0')<<setw(2)<<Lokalac[odg].VrijemePovratka[i].minuta<<setfill(' ')<<setw(9)<<"|"<<endl<<left;
			}
			cout<<setfill('-')<<setw(56)<<"-"<<endl<<setfill(' ');
			cout<<endl<<endl;
			cout<<""<<setfill('-')<<setw(57)<<"-"<<endl;
			cout<<"Odrzavanje linije: ";
			if(Lokalac[odg].Dan==7) cout<<"svaki dan.\n";
			else if(Lokalac[odg].Dan ==6 ) cout<<"svaki dan osim nedjelje i praznika.\n";
			else cout<<"svaki radni dan osim za vrijeme skolskog raspusta.\n";
			cout<<"Cijena karte: "<<Lokalac[odg].Karta<<" KM.\n";
			cout<<"Cijena mjesecne karte: "<<Lokalac[odg].Mjesecna<<" KM.\n";
			cout<<""<<setfill('-')<<setw(57)<<"-"<<endl<<endl;
			system("pause");
			system("cls");
			Meni();
		}else if(DrugiMeni == 2){ // 1.2. Pretraga gradskih linija
			system ("cls");
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
			cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
			cout << "\n\t\tPRETRAGA LINIJA\n";
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
			string odrediste1, odrediste2;
			cout << "\tMjesto polaska: ";
			cin.ignore();
			getline(cin, odrediste1);
			cout << "\tOdrediste: ";
			getline(cin, odrediste2);
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
			cout << endl;
			int brLokalni=0;			
			for (int i=0; i<h; i++){
				if (Lokalac[i].Polazak.compare(odrediste1)==0 && Lokalac[i].Povratak.compare(odrediste2)==0){
					brLokalni++;
					cout<<endl<<setfill('-')<<setw(79)<<"-"<<endl<<setfill(' ');
					cout<<setw(5)<<left<<" Br"<<setw(5)<<"|"<<setw(13)<<right<<Lokalac[i].Polazak<<" - "<<setw(16)<<left<<Lokalac[i].Povratak<<setw(4)<<"|"<<setw(13)<<right<<Lokalac[i].Povratak<<" - "<<setw(16)<<left<<Lokalac[i].Polazak<<setw(4)<<"|";
					cout<<endl<<setfill('-')<<setw(79)<<"-"<<endl<<setfill(' ');
				for(int j=0;j<=Lokalac[i].velicina;j++){
					cout << right<<setw(3)<<j+1 << ". " << setw(17)<<left<<"|"<< setfill('0')<<right<<setw(2)<<Lokalac[i].VrijemePolaska[j].sat<<":"<<setfill('0')<<setw(2)<<right<<Lokalac[i].VrijemePolaska[j].minuta<<setfill(' ')<<setw(16)<<"|" <<setw(15)<<" "<<setfill('0')<<setw(2)<< Lokalac[i].VrijemePovratka[j].sat<<":"<<setfill('0')<<setw(2)<<Lokalac[i].VrijemePovratka[j].minuta<<setfill(' ')<<setw(16)<<"|"<<endl<<left;
				}
				cout<<setfill('-')<<setw(79)<<"-"<<endl<<setfill(' ');
				cout<<endl<<endl;
				cout<<""<<setfill('-')<<setw(57)<<"-"<<endl;
				cout<<"Odrzavanje linije: ";
				if(Lokalac[i].Dan==7) cout<<"svaki dan.\n";
				else if(Lokalac[i].Dan ==6 ) cout<<"svaki dan osim nedjelje i praznika.\n";
				else cout<<"svaki radni dan osim za vrijeme skolskog raspusta.\n";
				cout<<"Cijena karte: "<<Lokalac[i].Karta<<" KM.\n";
				cout<<"Cijena mjesecne karte: "<<Lokalac[i].Mjesecna<<" KM.\n";
				cout<<""<<setfill('-')<<setw(57)<<"-"<<endl<<endl;
				if(brLokalni==1) break;
				}else if(Lokalac[i].Povratak.compare(odrediste1)==0 && Lokalac[i].Polazak.compare(odrediste2)==0){
					brLokalni++;
					cout<<endl<<setfill('-')<<setw(79)<<"-"<<endl<<setfill(' ');
					cout<<setw(5)<<left<<" Br"<<setw(5)<<"|"<<setw(13)<<right<<Lokalac[i].Polazak<<" - "<<setw(16)<<left<<Lokalac[i].Povratak<<setw(4)<<"|"<<setw(13)<<right<<Lokalac[i].Povratak<<" - "<<setw(16)<<left<<Lokalac[i].Polazak<<setw(4)<<"|";
					cout<<endl<<setfill('-')<<setw(79)<<"-"<<endl<<setfill(' ');
				for(int j=0;j<=Lokalac[i].velicina;j++){
					cout << right<<setw(3)<<j+1 << ". " << setw(17)<<left<<"|"<< setfill('0')<<right<<setw(2)<<Lokalac[i].VrijemePolaska[j].sat<<":"<<setfill('0')<<setw(2)<<right<<Lokalac[i].VrijemePolaska[j].minuta<<setfill(' ')<<setw(16)<<"|" <<setw(15)<<" "<<setfill('0')<<setw(2)<< Lokalac[i].VrijemePovratka[j].sat<<":"<<setfill('0')<<setw(2)<<Lokalac[i].VrijemePovratka[j].minuta<<setfill(' ')<<setw(16)<<"|"<<endl<<left;
				}
				cout<<setfill('-')<<setw(56)<<"-"<<endl<<setfill(' ');
				cout<<endl<<endl;
				cout<<""<<setfill('-')<<setw(57)<<"-"<<endl;
				cout<<"Odrzavanje linije: ";
				if(Lokalac[i].Dan==7) cout<<"svaki dan.\n";
				else if(Lokalac[i].Dan ==6 ) cout<<"svaki dan osim nedjelje i praznika.\n";
				else cout<<"svaki radni dan osim za vrijeme skolskog raspusta.\n";
				cout<<"Cijena karte: "<<Lokalac[i].Karta<<" KM.\n";
				cout<<"Cijena mjesecne karte: "<<Lokalac[i].Mjesecna<<" KM.\n";
				cout<<""<<setfill('-')<<setw(57)<<"-"<<endl<<endl;
				if(brLokalni==1) break;
				}
			}
			if (brLokalni==0){
				cout << "Nema linije sa unesenim mjestom polaska i odredista." << endl;
			}
			cout << endl;
			system ("pause");
			system ("cls");
			Meni();
		}else if(DrugiMeni == 3){ // 1.3. Povratak na pocetni meni
			system ("cls");
			Meni();
		}
	}else if(PrviMeni == 2){ // 2. Drugi Meni
		system ("cls");
		int TreciMeni;
		TreciMeni = Meni3();
		if(TreciMeni == 1){ // 2.1. Rezervacije
			system ("cls");
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
			cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
			cout << "\n\t\tREZERVACIJE\n";
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
			int izbor;
			string RezervacijaMjesto, RezervacijaMjesto1;
			cout << "\tMjesto polaska: ";
			cin.ignore();
			getline(cin,RezervacijaMjesto1);
			cout << "\tDestinacija: ";
			getline(cin,RezervacijaMjesto);
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
			int brDestinacija1=0;
			int niz[brDestinacija1];
			cout<<setfill(' ');
			for (int i=0; i<k; i++){
				if ((Podaci[i].PovratakMjesto.compare(RezervacijaMjesto)==0) && (Podaci[i].PolazakMjesto.compare(RezervacijaMjesto1)==0) && (BrSlobodnihMjesta(Podaci[i].ID, Podaci[i].BrojMjesta)!=0) && (Podaci[i].DatumPolaska.DatumProvjera(danas, Podaci[i].DatumPolaska)==true)){
					brDestinacija1++;
					if(brDestinacija1==1){
						cout<<endl<<setfill('-')<<setw(96)<<"-"<<endl<<setfill(' ');
						cout<<setw(4)<<left<<" Br"<<setw(9)<<"|"<<setw(16)<<"Polazak"<<setw(9)<<"|"<<setw(16)<<"Povratak"<<setw(4)<<"|"<<setw(15)<<"Cijena karte"<<setw(4)<<"|"<<setw(18)<<"Slobodna mjesta"<<"|"<<endl;
						cout<<setfill('-')<<setw(96)<<"-"<<endl<<setfill(' ');
					}
					cout << " "<<brDestinacija1 <<setw(2)<< "." << setw(3)<<"|"<< setfill('0')<<setw(2)<<Podaci[i].DatumPolaska.dan<<"."<<setfill('0')<<setw(2)<<right<<Podaci[i].DatumPolaska.mjesec<<"."<<Podaci[i].DatumPolaska.godina<<".  " << "(" <<setfill('0')<<setw(2)<< Podaci[i].PolazakVrijeme.sat<<":"<<setfill('0')<<setw(2)<<Podaci[i].PolazakVrijeme.minuta;
					cout << ")"<<setfill(' ')<<setw(5)<<left<<"  |" << setfill('0')<<setw(2)<<Podaci[i].DatumPovratka.dan<<"."<<setfill('0')<<setw(2)<<Podaci[i].DatumPovratka.mjesec<<"."<<Podaci[i].DatumPovratka.godina<<".  " << "(" <<setfill('0')<<setw(2)<<Podaci[i].PovratakVrijeme.sat<<":"<<setfill('0')<<setw(2)<<Podaci[i].PovratakVrijeme.minuta<< ")  "<<setfill(' ')<<setw(5)<<"|"<<setw(6)<<right<< Podaci[i].Cijena << " KM";
					cout << setw(6)<<"|"<<setw(11)<<BrSlobodnihMjesta(Podaci[i].ID, Podaci[i].BrojMjesta) <<setw(11)<<"|"<<endl<<left;
					niz[brDestinacija1]=i;
				}
			}
			if (brDestinacija1==0){
				cout << "\n\tU narednom periodu nema putovanja na zeljenu destinaciju.\n" << endl;
				system ("pause");
				system ("cls");
				Meni ();
			}
			int IzborDatuma;
			cout<<setfill('-')<<setw(96)<<"-"<<endl<<setfill(' ');
			cout << "\n\nUnesite redni broj zeljenog putovanja (0 - IZLAZ): ";
			do{
				cin>>IzborDatuma;
				if(cin.fail()){
					cin.clear();
					cin.ignore(1000, '\n');
					cout << "[ERROR] Nevalidan unos.\n\tPokusajte ponovo: ";			
				}else if(IzborDatuma==0){
					system("cls");
					Meni();
				}else if(IzborDatuma<0 || IzborDatuma>brDestinacija1){
					cin.clear();
					cin.ignore(1000, '\n');
					cout << "[ERROR] Nevalidan unos.\n\tPokusajte ponovo: ";						
				}else if(IzborDatuma>0 && IzborDatuma<=brDestinacija1) break;
			}while(!cin.fail());
			system ("cls");
			int odgovor;
			do{
				line="";
				line= to_string (Podaci[niz[IzborDatuma]].ID);
				line= line + ".txt";
				int brojac1=0;
				ofstream Putnici;
				if(BrSlobodnihMjesta(Podaci[niz[IzborDatuma]].ID, Podaci[niz[IzborDatuma]].BrojMjesta)== Podaci[niz[IzborDatuma]].BrojMjesta){
					Putnici.open(line);
					if (Putnici.fail()){
						cout << "Nemoguce otvoriti datoteku..." << endl;
					}
				}else{
					Putnici.open(line, ios::app);
					if (Putnici.fail()){
						cout << "Nemoguce otvoriti datoteku..." << endl;
					}
				}				
				Putnik putnici;
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout << "\n\t\tREZERVACIJE\n";
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout << "\tIme: ";
				cin.ignore();
				cin >> putnici.ime;
				Putnici << putnici.ime << "/";
				cout << "\tPrezime: ";
				cin >> putnici.prezime;
				Putnici << putnici.prezime << "/";
				cout << "\tKontakt broj: ";
				cin >> putnici.KontaktBroj;
				Putnici << putnici.KontaktBroj << "/";
				cout << "\tUzrast: \n\t   1 - Dijete \n\t   2 - Student \n\t   3 - Odrasli \n\t   4 - Penzioner ";
				int x;
				do{
					cout << "\n\tVas izbor: ";
					if(!(cin>>x) || x<1 || x>4){
						cin.clear();
						cin.ignore(1000,'\n');
						cout<<"\n\t[ERROR] Nevalidan unos.\n\t\tPokusajte ponovo.\n";
					}
				}while (x<1 || x>4 || !(x));
				string UzrastPutnika;
				double CijenaKarte;
				switch (x){
					case 1:
						UzrastPutnika = "Dijete";
						CijenaKarte = (Podaci[niz[IzborDatuma]].Cijena)*0.65;					
						break;
					case 2:
						UzrastPutnika = "Student";
						CijenaKarte = (Podaci[niz[IzborDatuma]].Cijena)*0.9;
						break;
					case 3:
						UzrastPutnika = "Odrasli";
						CijenaKarte = Podaci[niz[IzborDatuma]].Cijena;
						break;
					case 4:
						UzrastPutnika = "Penzioner";
						CijenaKarte = (Podaci[niz[IzborDatuma]].Cijena)*0.85;
						break;
				}
				Putnici << UzrastPutnika << "/";
				int y;
				cout << "\tPovratna karta:\n\t   0 - NE\n\t   1 - DA\n";
				do{
					cout << "\tVas izbor: ";
					if(!(cin>>y) || y<0 || y>1){
						cin.clear();
						cin.ignore(1000,'\n');
						cout<<"\n\t[ERROR] Nevalidan unos.\n\t\tPokusajte ponovo.\n";
					}else{
						break;
					}
				}while(true);
				string Povratna;
				switch (y){
					case 0:
						Povratna = "Jednosmjerna";
						CijenaKarte = CijenaKarte*0.65;
						break;
					case 1:
						Povratna = "Povratna";
						break;
				}
				Putnici << Povratna << "/\n";
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl<<"\n\t";
				system("pause");
				system("cls");
				double novac;
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout << "\n\t\t   NAPLATA\n";
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout << "\tCijena karte: " << CijenaKarte << " KM"<<endl;
				do{
					cout << "\tDato novca: ";
					if(!(cin>>novac)){
						cin.clear();
						cin.ignore(1000,'\n');
						cout<<"\n\t[ERROR] Nevalidan unos.\n\t\tPokusajte ponovo.\n";
					}else if(novac<CijenaKarte){
						cout<<"\n\t[ERROR] Nedovoljna kolicina novca. Nedostaje jos "<<CijenaKarte - novac<<" KM"<<endl;
					}
				}while(!(novac) || novac<CijenaKarte);
				if(novac!=CijenaKarte) cout<<"\tKusur: "<<novac - CijenaKarte<<" KM\n";
				cout<<"\n\tKarta je rezervisana.\n\tSretan put.\n";
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl<<endl;
				do{
					cout<<"\n\tDa li zelite jos rezervacija na ovu destinaciju?\n\t   0 - NE\n\t   1 - DA\n\tVas izbor: ";
					if(!(cin>>odgovor) || odgovor<0 || odgovor>1){
						cin.clear();
						cin.ignore(1000,'\n');
						cout<<"\n\t[ERROR] Nevalidan unos.\n\t\tPokusajte ponovo.\n";
					}else{
						break;
					}
				}while(true);
				if (odgovor==1 && BrSlobodnihMjesta(Podaci[niz[IzborDatuma]].ID, Podaci[niz[IzborDatuma]].BrojMjesta)==0){
					cout << "\n\tNema vise slobodnih mijesta u ovom autobusu.";
				}
				system ("cls");
				Putnici.close();
			}while (odgovor==1 && BrSlobodnihMjesta(Podaci[niz[IzborDatuma]].ID, Podaci[niz[IzborDatuma]].BrojMjesta)>0);
			system ("cls");			
			Meni ();	
			
		}else if(TreciMeni == 2){ // 2.2 Pretraga vangradskih voznji
			system ("cls");
			int Pretrazivanje;
			Pretrazivanje = MeniPretraga();
			
			if(Pretrazivanje == 1){ // 2.2.1 Pretrga po imenu agencije
				system("cls");
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout << "\n\t  PRETRAGA PO IMENU AGENCIJE\n";
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				string agencija, line;
				cout << "\tNaziv agencije: ";
				cin.ignore();
				getline(cin,agencija);
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				int brAgencija=0;
				for (int i=0; i<k; i++){
					if (agencija.compare(Podaci[i].Agencija)==0 && Podaci[i].DatumPolaska.DatumProvjera(danas,Podaci[i].DatumPolaska)==true){
						brAgencija++;
						if(brAgencija==1){
							cout<<endl<<setfill('-')<<setw(119)<<"-"<<endl<<setfill(' ');
							cout<<setw(4)<<left<<" Br"<<setw(13)<<"|"<<setw(20)<<"Relacija"<<setw(9)<<"|"<<setw(16)<<"Polazak"<<setw(9)<<"|"<<setw(16)<<"Povratak"<<setw(4)<<"|"<<setw(7)<<"Karta"<<setw(3)<<"|"<<setw(17)<<"Slobodna mjesta"<<"|"<<endl;
							cout<<setfill('-')<<setw(119)<<"-"<<endl<<setfill(' ');		
						}
						cout << " "<<brAgencija <<setw(2)<< "." << setw(3)<<"|"<< setw(13)<<right<<Podaci[i].PolazakMjesto<<" - "<<setw(14)<<left<<Podaci[i].PovratakMjesto<< setw(3)<<"|"<<setfill('0')<<setw(2)<<right<<Podaci[i].DatumPolaska.dan<<"."<<setfill('0')<<setw(2)<<right<<Podaci[i].DatumPolaska.mjesec<<"."<<Podaci[i].DatumPolaska.godina<<".  " << "(" <<setfill('0')<<setw(2)<< Podaci[i].PolazakVrijeme.sat<<":"<<setfill('0')<<setw(2)<<Podaci[i].PolazakVrijeme.minuta;
						cout << ")"<<setfill(' ')<<setw(5)<<left<<"  |" << setfill('0')<<setw(2)<<Podaci[i].DatumPovratka.dan<<"."<<setfill('0')<<setw(2)<<Podaci[i].DatumPovratka.mjesec<<"."<<Podaci[i].DatumPovratka.godina<<".  " << "(" <<setfill('0')<<setw(2)<<Podaci[i].PovratakVrijeme.sat<<":"<<setfill('0')<<setw(2)<<Podaci[i].PovratakVrijeme.minuta<< ")  "<<setfill(' ')<<setw(2)<<"|"<<setw(4)<<right<< Podaci[i].Cijena << " KM";
						cout << setw(3)<<"|"<<setw(11)<<BrSlobodnihMjesta(Podaci[i].ID, Podaci[i].BrojMjesta) <<setw(9)<<"|"<<endl<<left;
					}
				}
				if (brAgencija==0){
					cout << "\n\tTrenutno nema putovanja koje organizuje " << agencija << "." << endl;
				}
				cout<<setfill('-')<<setw(119)<<"-"<<endl<<setfill(' ');
				cout<<endl;	
				system ("pause");
				system ("cls");
				Meni ();
							
			}else if(Pretrazivanje == 2){ // Pretraga po destinaciji
				string destinacija;
				system ("cls");
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout << "\n\t    PRETRAGA PO DESTINACIJI\n";
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout << "\tDestinacija: ";
				cin.ignore();
				getline (cin, destinacija);
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				int brDestinacija=0;
				for (int i=0; i<k; i++){
					if (Podaci[i].PovratakMjesto.compare(destinacija)==0 && Podaci[i].DatumPolaska.DatumProvjera(danas,Podaci[i].DatumPolaska)==true){
						brDestinacija++;
						if (brDestinacija==1){
							cout<<endl<<setfill('-')<<setw(116)<<"-"<<endl<<setfill(' ');
							cout<<setw(4)<<left<<" Br"<<setw(15)<<"|"<<setw(24)<<"Polazak"<<setw(8)<<"|"<<setw(16)<<"Povratak"<<setw(5)<<"|"<<setw(12)<<"Agencija"<<setw(4)<<"|"<<setw(7)<<"Karta"<<setw(3)<<"|"<<setw(17)<<"Slobodna mjesta"<<"|"<<endl;
							cout<<setfill('-')<<setw(116)<<"-"<<endl<<setfill(' ');									
						}
						cout << " "<<brDestinacija <<setw(2)<< "." << setw(3)<<"|"<< setw(12)<<right<<Podaci[i].PolazakMjesto<<" - "<<setfill('0')<<setw(2)<<right<<Podaci[i].DatumPolaska.dan<<"."<<setfill('0')<<setw(2)<<right<<Podaci[i].DatumPolaska.mjesec<<"."<<Podaci[i].DatumPolaska.godina<<". " << "(" <<setfill('0')<<setw(2)<< Podaci[i].PolazakVrijeme.sat<<":"<<setfill('0')<<setw(2)<<Podaci[i].PolazakVrijeme.minuta;
						cout << ")"<<setfill(' ')<<setw(5)<<left<<"  |" << setfill('0')<<setw(2)<<Podaci[i].DatumPovratka.dan<<"."<<setfill('0')<<setw(2)<<Podaci[i].DatumPovratka.mjesec<<"."<<Podaci[i].DatumPovratka.godina<<". " << "(" <<setfill('0')<<setw(2)<<Podaci[i].PovratakVrijeme.sat<<":"<<setfill('0')<<setw(2)<<Podaci[i].PovratakVrijeme.minuta<< ")  "<<setfill(' ')<<setw(5)<<"|"<<setw(10)<<Podaci[i].Agencija<<setw(2)<<"  | "<<setw(4)<<right<< Podaci[i].Cijena << " KM";
						cout << setw(3)<<"|"<<setw(11)<<BrSlobodnihMjesta(Podaci[i].ID, Podaci[i].BrojMjesta) <<setw(9)<<"|"<<endl<<left;
					}
				}
				if (brDestinacija==0){
					cout << "\n\tTrenutno nema planiranih putovanja u " << destinacija<< endl;
				}
				cout<<setfill('-')<<setw(116)<<"-"<<endl<<setfill(' ');
				cout<<endl;
				system ("pause");
				system ("cls");
				Meni();
				
			}else if(Pretrazivanje == 3){ // Pretraga po ID-u voznje
				system ("cls");
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout << "\n\t  PRETRAGA PO ID-u PUTOVANJA\n";
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout << "\tID: ";
				int ZeljeniID;
				cin.ignore();
				cin >> ZeljeniID;
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				int brID=0;
				for (int i=0; i<k; i++){
					if (Podaci[i].ID == ZeljeniID && Podaci[i].DatumPolaska.DatumProvjera(danas,Podaci[i].DatumPolaska)==true){
						brID++;
						if(brID==1){
							cout<<endl<<setfill('-')<<setw(120)<<"-"<<endl<<setfill(' ');
							cout<<setw(12)<<left<<"  Agencija"<<setw(11)<<"|"<<setw(17)<<"Relacija"<<setw(9)<<"|"<<setw(15)<<"Polazak"<<setw(9)<<"|"<<setw(15)<<"Povratak"<<setw(4)<<"|"<<setw(7)<<"Karta"<<setw(3)<<"|"<<setw(17)<<"Slobodna mjesta"<<"|"<<endl;
							cout<<setfill('-')<<setw(120)<<"-"<<endl<<setfill(' ');									
						}
						cout << " "<<Podaci[i].Agencija <<" "<< setw(3)<<"|"<< setw(11)<<right<<Podaci[i].PolazakMjesto<<" - "<<setw(11)<<left<<Podaci[i].PovratakMjesto<< setw(3)<<"|"<<setfill('0')<<setw(2)<<right<<Podaci[i].DatumPolaska.dan<<"."<<setfill('0')<<setw(2)<<right<<Podaci[i].DatumPolaska.mjesec<<"."<<Podaci[i].DatumPolaska.godina<<". " << "(" <<setfill('0')<<setw(2)<< Podaci[i].PolazakVrijeme.sat<<":"<<setfill('0')<<setw(2)<<Podaci[i].PolazakVrijeme.minuta;
						cout << ")"<<setfill(' ')<<setw(5)<<left<<"  |" << setfill('0')<<setw(2)<<Podaci[i].DatumPovratka.dan<<"."<<setfill('0')<<setw(2)<<Podaci[i].DatumPovratka.mjesec<<"."<<Podaci[i].DatumPovratka.godina<<". " << "(" <<setfill('0')<<setw(2)<<Podaci[i].PovratakVrijeme.sat<<":"<<setfill('0')<<setw(2)<<Podaci[i].PovratakVrijeme.minuta<< ")  "<<setfill(' ')<<setw(2)<<"|"<<setw(4)<<right<< Podaci[i].Cijena << " KM";
						cout << setw(3)<<"|"<<setw(11)<<BrSlobodnihMjesta(Podaci[i].ID, Podaci[i].BrojMjesta) <<setw(9)<<"|"<<endl<<left;
					}
				}
				if (brID==0){
					cout << "\n\tNe postoji putovanja sa ID-om " <<ZeljeniID<< endl;
				}
				cout<<setfill('-')<<setw(120)<<"-"<<endl<<setfill(' ');
				cout<<endl;
				system("pause");
				system ("cls");
				Meni();
				
			}else if(Pretrazivanje == 4){ // Pretraga po datumu polaska
				system ("cls");
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout << "\n\t\tPRETRAGA PO DATUMU POLASKA\n";
				cout<<"\t"<<setfill('-')<<setw(45)<<"-"<<endl;
				Datum dan;
				do{
					cout << "\tDatum polaska (npr. 05.02.2022.): ";
					if(!(cin>>dan.dan>>s>>dan.mjesec>>s>>dan.godina>>s)) {
						cin.clear();
						cin.ignore(1000,'\n');
						cout<<"[GRESKA] Nevalidan datum.\n";	
						}
					else if(!dan.DatumProvjera(danas,dan)){
						cin.clear();
						cin.ignore(1000,'\n');						
						cout<<"[GRESKA] Uneseni datum je prosao.\n";		
					}	
					else if(!dan.DatumUnos(dan)){
						cin.clear();
						cin.ignore(1000,'\n');							
						cout<<"[GRESKA] Nevalidan datum.\n";	
					}
				}while(!dan.DatumUnos(dan) || !dan.DatumProvjera(danas,dan) || !(dan.dan) || !(dan.mjesec || !(dan.godina)));
				int brDatum=0;
				for (int i=0; i<k; i++){
					if (Podaci[i].DatumPolaska.dan == dan.dan && Podaci[i].DatumPolaska.mjesec == dan.mjesec && Podaci[i].DatumPolaska.godina == dan.godina){
						brDatum++;
						if(brDatum==1){
							cout<<endl<<setfill('-')<<setw(119)<<"-"<<endl<<setfill(' ');
							cout<<setw(4)<<left<<" Br"<<setw(13)<<"|"<<setw(20)<<"Relacija"<<setw(9)<<"|"<<setw(16)<<"Polazak"<<setw(9)<<"|"<<setw(16)<<"Agencija"<<setw(4)<<"|"<<setw(7)<<"Karta"<<setw(3)<<"|"<<setw(17)<<"Slobodna mjesta"<<"|"<<endl;
							cout<<setfill('-')<<setw(119)<<"-"<<endl<<setfill(' ');									
						}
						cout << " "<<brDatum <<setw(2)<< "." << setw(3)<<"|"<< setw(13)<<right<<Podaci[i].PolazakMjesto<<" - "<<setw(14)<<left<<Podaci[i].PovratakMjesto<< setw(3)<<"|"<<setfill('0')<<setw(2)<<right<<Podaci[i].DatumPolaska.dan<<"."<<setfill('0')<<setw(2)<<right<<Podaci[i].DatumPolaska.mjesec<<"."<<Podaci[i].DatumPolaska.godina<<".  " << "(" <<setfill('0')<<setw(2)<< Podaci[i].PolazakVrijeme.sat<<":"<<setfill('0')<<setw(2)<<Podaci[i].PolazakVrijeme.minuta;
						cout << ")"<<setfill(' ')<<setw(9)<<left<<"  |" <<setw(18)<<Podaci[i].Agencija << setfill(' ')<<setw(2)<<"|"<<setw(4)<<right<< Podaci[i].Cijena << " KM";
						cout << setw(3)<<"|"<<setw(11)<<BrSlobodnihMjesta(Podaci[i].ID, Podaci[i].BrojMjesta) <<setw(9)<<"|"<<endl<<left;
					}
				}
				if (brDatum==0){
					cout << "\n\tNema zakazanih putovanja na datum " << setfill('0')<<setw(2)<<dan.dan<<"."<<setw(2)<<dan.mjesec<<"."<<dan.godina<<"." << endl;
				}else cout<<setfill('-')<<setw(119)<<"-"<<endl<<setfill(' ');
				cout<<endl;
				system ("pause");
				system ("cls");
				Meni ();
				
			}else if(Pretrazivanje==5){ // Pretraga po vozacu
				system ("cls");
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
				cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
				cout << "\n\t\tPRETRAGA PO VOZACU\n";
				cout<<"\t"<<setfill('-')<<setw(45)<<"-"<<endl;	
				string ime,prezime;
				cin.ignore();			
				cout << "\tUnesite ime vozaca: "; 
				getline (cin,ime);
				cout << "\tUnesite prezime vozaca: ";
				getline(cin,prezime);
				cout << "\tLINIJE KOJE UNIJETI VOZAC VOZI:" <<endl;
				PretragaVozaca(Podaci,ime,prezime,0,k,0);
				system("pause");
				system("cls");
				Meni();
			}			
			
			else{ // Povratak na pocetni meni
				system ("cls");
				Meni();				
			}
			
		}else if(TreciMeni == 3){ // 2.3. Unos novih linija
			system ("cls");
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
			cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
			cout << "\t\n\tUNOS NOVIH LINIJA\t" << endl;
			cout<<"\t"<<setfill('-')<<setw(20)<<"-"<<endl;
			ofstream NovaLinija;
			NovaLinija.open("Autobusi.txt", ios::app);
			Autobus Podaci[k+1];
			cout << "\tMjesto polaska: ";
			cin.ignore();
			getline(cin, Podaci[k].PolazakMjesto);
			NovaLinija << "\n" << Podaci[k].PolazakMjesto << "/";
			cout << "\tDestinacija: ";
			getline (cin, Podaci[k].PovratakMjesto);
			NovaLinija << Podaci[k].PovratakMjesto << "/";
			do{
				cout << "\tDatum polaska (npr. 05.02.2022.): ";
				if(!(cin>>Podaci[k].DatumPolaska.dan>>s>>Podaci[k].DatumPolaska.mjesec>>s>>Podaci[k].DatumPolaska.godina>>s)){
					cin.clear();
					cin.ignore(1000,'\n');
					cout<<"[GRESKA] Nevalidan datum.\n";
				}
				else if(!Podaci[k].DatumPolaska.DatumProvjera(danas,Podaci[k].DatumPolaska)){
					cin.clear();
					cin.ignore(1000,'\n');						
					cout<<"[GRESKA] Uneseni datum je prosao.\n";	
				}
				else if(!Podaci[k].DatumPolaska.DatumUnos(Podaci[k].DatumPolaska)){
					cin.clear();
					cin.ignore(1000,'\n');							
					cout<<"[GRESKA] Nevalidan datum.\n";}
			}while(!Podaci[k].DatumPolaska.DatumUnos(Podaci[k].DatumPolaska) || !Podaci[k].DatumPolaska.DatumProvjera(danas,Podaci[k].DatumPolaska) || !(Podaci[k].DatumPolaska.dan) || !(Podaci[k].DatumPolaska.mjesec) || !(Podaci[k].DatumPolaska.godina));
			if(Podaci[k].DatumPolaska.dan/10<=0) NovaLinija << "0"<<Podaci[k].DatumPolaska.dan<<".";
			else NovaLinija << Podaci[k].DatumPolaska.dan<<".";
			if(Podaci[k].DatumPolaska.mjesec/10<=0) NovaLinija << "0"<<Podaci[k].DatumPolaska.mjesec<<"."<<Podaci[k].DatumPolaska.godina<<"./";
			else NovaLinija<<Podaci[k].DatumPolaska.mjesec<<"."<<Podaci[k].DatumPolaska.godina<<"./";
			cout << "\tVrijeme polaska (npr. 16:05): ";
			do{
				cin>>Podaci[k].PolazakVrijeme.sat>>s>>Podaci[k].PolazakVrijeme.minuta;
				if(cin.fail()){
					cin.clear();
					cin.ignore(1000, '\n');
					cout << "[ERROR] Nevalidan unos.\n\tPokusajte ponovo: ";			
				}else if(!(Podaci[k].PolazakVrijeme.VrijemeUnos(Podaci[k].PolazakVrijeme))){
					cin.clear();
					cin.ignore(1000, '\n');
					cout << "[ERROR] Nevalidan unos.\n\tPokusajte ponovo: ";						
				}else if(Podaci[k].PolazakVrijeme.VrijemeUnos(Podaci[k].PolazakVrijeme))break;
			}while(!cin.fail());
			if(Podaci[k].PolazakVrijeme.sat/10<=0) NovaLinija << "0"<< Podaci[k].PolazakVrijeme.sat<<":";
			else NovaLinija<<Podaci[k].PolazakVrijeme.sat<<":";
			if(Podaci[k].PolazakVrijeme.minuta/10<=0) NovaLinija<<"0"<<Podaci[k].PolazakVrijeme.minuta<<"/";
			else NovaLinija<<Podaci[k].PolazakVrijeme.minuta<<"/";
			do{
				cout << "\tDatum povratka: ";
				if(!(cin>>Podaci[k].DatumPovratka.dan>>s>>Podaci[k].DatumPovratka.mjesec>>s>>Podaci[k].DatumPovratka.godina>>s)){
					cin.clear();
					cin.ignore(1000,'\n');							
					cout<<"[GRESKA] Nevalidan datum.\n";						
				}
				else if(!Podaci[k].DatumPovratka.DatumProvjera(Podaci[k].DatumPolaska, Podaci[k].DatumPovratka)){
					cin.clear();
					cin.ignore(1000,'\n');							
					cout << "[GRESKA] Datum povratka ne moze biti prije datuma polaska.\n";					
				}
				if(!Podaci[k].DatumPovratka.DatumUnos(Podaci[k].DatumPovratka)){
					cin.clear();
					cin.ignore(1000,'\n');							
					cout<<"[GRESKA] Nevalidan datum.\n";					
				}
			}while(!Podaci[k].DatumPovratka.DatumProvjera(Podaci[k].DatumPolaska, Podaci[k].DatumPovratka) || !Podaci[k].DatumPovratka.DatumUnos(Podaci[k].DatumPovratka) || !(Podaci[k].DatumPovratka.dan) || !(Podaci[k].DatumPovratka.mjesec) || !(Podaci[k].DatumPovratka.godina));
			if(Podaci[k].DatumPovratka.dan/10<=0) NovaLinija << "0"<<Podaci[k].DatumPovratka.dan<<".";
			else NovaLinija << Podaci[k].DatumPovratka.dan<<".";
			if(Podaci[k].DatumPovratka.mjesec/10<=0) NovaLinija << "0"<<Podaci[k].DatumPovratka.mjesec<<"."<<Podaci[k].DatumPovratka.godina<<"./";
			else NovaLinija<<Podaci[k].DatumPovratka.mjesec<<"."<<Podaci[k].DatumPovratka.godina<<"./";
			int pr;
			cout << "\tVrijeme povratka: ";
			do{
				cin>>Podaci[k].PovratakVrijeme.sat>>s>>Podaci[k].PovratakVrijeme.minuta;
				if(cin.fail()){
					cin.clear();
					cin.ignore(1000, '\n');
					cout << "[ERROR] Nevalidan unos.\n\tPokusajte ponovo: ";			
				}else if(!(Podaci[k].PovratakVrijeme.VrijemeUnos(Podaci[k].PovratakVrijeme))){
					cin.clear();
					cin.ignore(1000, '\n');
					cout << "[ERROR] Nevalidan unos.\n\tPokusajte ponovo: ";						
				}else if(Podaci[k].DatumPolaska.dan == Podaci[k].DatumPovratka.dan && Podaci[k].DatumPolaska.mjesec == Podaci[k].DatumPovratka.mjesec && Podaci[k].DatumPolaska.godina == Podaci[k].DatumPovratka.godina){
					if(!Podaci[k].PovratakVrijeme.VrijemeProvjera(Podaci[k].PolazakVrijeme, Podaci[k].PovratakVrijeme)){
						cin.clear();
						cin.ignore(1000,'\n');	
						cout<<"[GRESKA] Datum polaska i povratka su isti, time vrijeme povratka ne moze biti prije vremena polaska.\n";
					}
				}else if(Podaci[k].PovratakVrijeme.VrijemeUnos(Podaci[k].PovratakVrijeme))break;
			}while(!cin.fail());
			if(Podaci[k].PovratakVrijeme.sat/10<=0) NovaLinija << "0"<< Podaci[k].PovratakVrijeme.sat<<":";
			else NovaLinija<<Podaci[k].PovratakVrijeme.sat<<":";
			if(Podaci[k].PovratakVrijeme.minuta/10<=0) NovaLinija<<"0"<<Podaci[k].PovratakVrijeme.minuta<<"/";
			else NovaLinija<<Podaci[k].PovratakVrijeme.minuta<<"/";
			cout << "\tAgencija: ";
			cin.ignore();
			getline (cin, Podaci[k].Agencija);
			NovaLinija << Podaci[k].Agencija << "/";
			do{
				cout << "\tCijena povratne karte: ";
				if(!(cin >> Podaci[k].Cijena) || Podaci[k].Cijena<1){
					cin.clear();
					cin.ignore(1000,'\n');							
					cout<<"[GRESKA] Nevalidan unos.\n";	
				}		
			}while(!(Podaci[k].Cijena) || Podaci[k].Cijena<1);
			NovaLinija << Podaci[k].Cijena << "/";
			do{
				cout << "\tBroj mjesta: ";
				if(!(cin >> Podaci[k].BrojMjesta) || Podaci[k].BrojMjesta<1){
					cin.clear();
					cin.ignore(1000,'\n');							
					cout<<"[GRESKA] Nevalidan unos.\n";						
				}			
			}while(Podaci[k].BrojMjesta<1 || !(Podaci[k].BrojMjesta));
			NovaLinija << Podaci[k].BrojMjesta << "/";
			do{
				cout << "\tID voznje: ";
				if(!(cin>> Podaci[k].ID) || Podaci[k].ID<1){
					cin.clear();
					cin.ignore(1000,'\n');							
					cout<<"[GRESKA] Nevalidan unos.\n";						
				}
			}while(Podaci[k].ID<1 || !(Podaci[k].ID));
			NovaLinija << Podaci[k].ID << "/";
			cin.ignore();
			cout << "\tIme vozaca: ";
			getline(cin, Podaci[k].ImeVozaca);
			NovaLinija << Podaci[k].ImeVozaca << "/";
			cout << "\tPrezime vozaca: ";
			getline (cin, Podaci[k].PrezimeVozaca);
			NovaLinija << Podaci[k].PrezimeVozaca << "/";
			cout << "\tNOVA LINIJA JE UNESENA." << endl;
			cout<<"\t"<<setfill('-')<<setw(20)<<"-"<<endl;
			NovaLinija.close();
			system ("pause");
			system ("cls");
			Meni();	
							
		}else if(TreciMeni == 4){ // 2.4. Otkazivanje rezervacije
			system ("cls");
			string OtkazivanjeMjesto, OtkazivanjeMjesto1;
			int brOtkazivanje1=0;
			int niz[brOtkazivanje1];
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
			cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
			cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
			cout << "\t\n\t OTKAZIVANJE REZERVACIJE\t" << endl;
			cout<<"\t"<<setfill('-')<<setw(25)<<"-"<<endl;
			cout << "\tMjesto polaska: ";
			cin.ignore();
			getline(cin,OtkazivanjeMjesto1);
			cout << "\tDestinacija: ";
			getline(cin, OtkazivanjeMjesto);
			cout<<"\t"<<setfill('-')<<setw(25)<<"-"<<endl<<endl;
			for (int i=0; i<k; i++){
				if ((Podaci[i].PovratakMjesto.compare(OtkazivanjeMjesto)==0) && (Podaci[i].PolazakMjesto.compare(OtkazivanjeMjesto1)==0) && (BrSlobodnihMjesta(Podaci[i].ID, Podaci[i].BrojMjesta)!=0)){
					brOtkazivanje1++;
					if(brOtkazivanje1==1){
						cout<<endl<<setfill('-')<<setw(96)<<"-"<<endl<<setfill(' ');
						cout<<setw(4)<<left<<" Br"<<setw(9)<<"|"<<setw(16)<<"Polazak"<<setw(9)<<"|"<<setw(16)<<"Povratak"<<setw(4)<<"|"<<setw(15)<<"Cijena karte"<<setw(8)<<"|"<<setw(14)<<"Agencija"<<"|"<<endl;
						cout<<setfill('-')<<setw(96)<<"-"<<endl<<setfill(' ');						
					}
					cout << " "<<brOtkazivanje1 <<setw(2)<< "." << setw(3)<<"|"<< setfill('0')<<setw(2)<<Podaci[i].DatumPolaska.dan<<"."<<setfill('0')<<setw(2)<<right<<Podaci[i].DatumPolaska.mjesec<<"."<<Podaci[i].DatumPolaska.godina<<".  " << "(" <<setfill('0')<<setw(2)<< Podaci[i].PolazakVrijeme.sat<<":"<<setfill('0')<<setw(2)<<Podaci[i].PolazakVrijeme.minuta;
					cout << ")"<<setfill(' ')<<setw(5)<<left<<"  |" << setfill('0')<<setw(2)<<Podaci[i].DatumPovratka.dan<<"."<<setfill('0')<<setw(2)<<Podaci[i].DatumPovratka.mjesec<<"."<<Podaci[i].DatumPovratka.godina<<".  " << "(" <<setfill('0')<<setw(2)<<Podaci[i].PovratakVrijeme.sat<<":"<<setfill('0')<<setw(2)<<Podaci[i].PovratakVrijeme.minuta<< ")  "<<setfill(' ')<<setw(5)<<"|"<<setw(6)<<right<< Podaci[i].Cijena << " KM";
					cout << setw(6)<<"|"<<setw(15)<<Podaci[i].Agencija <<setw(7)<<"|"<<endl<<left;
					niz[brOtkazivanje1]=i;
				}
			}
			if (brOtkazivanje1==0){
				cout << "\t[GRESKA] Ne postoji linija na unesenu destinaciju." << endl;
				system ("pause");
				system ("cls");
				Meni();
			}
			cout<<setfill('-')<<setw(96)<<"-"<<endl<<setfill(' ');	
			int IzborDatuma1;
			cout << "\nUnesite redni broj zeljenog putovanja (0 - IZLAZ): ";
			do{
				cin>>IzborDatuma1;
				if(cin.fail()){
					cin.clear();
					cin.ignore(1000, '\n');
					cout << "[ERROR] Nevalidan unos.\n\tPokusajte ponovo: ";			
				}else if(IzborDatuma1==0){
					system("cls");
					Meni();
				}else if(IzborDatuma1<0 || IzborDatuma1>brOtkazivanje1){
					cin.clear();
					cin.ignore(1000, '\n');
					cout << "[ERROR] Nevalidan unos.\n\tPokusajte ponovo: ";						
				}else if(IzborDatuma1>0 && IzborDatuma1<=brOtkazivanje1) break;
			}while(!cin.fail());
			line="";
			line= to_string (Podaci[niz[IzborDatuma1]].ID);
			line=line + ".txt";
			ifstream Putnici;
			int BrojacLinija=0;
			Putnici.open(line);
			string line1=line;
			while (!Putnici.eof()){
				getline (Putnici,line);
				BrojacLinija++;
			}			
			Putnici.close();
			Putnici.open(line1);
			Putnik Osobe[BrojacLinija];
			line="";
			BrojacLinija=0;
			while(!Putnici.eof()){
			getline(Putnici,line);
			int j=0, br1=0;
			string provjera="";
			do{
				if(line[j] == '/'){
					br1++;
					provjera = "";
					j++;
				}else if(line[j]!='/'){
					provjera = provjera + line[j];
					j++;
					if(line[j+1]=='/'){
						provjera = provjera + line[j];
						switch (br1){
							case 0:
								Osobe[BrojacLinija].ime = provjera;
								break;
							case 1:
								Osobe[BrojacLinija].prezime = provjera;
								break;
							case 2:
								Osobe[BrojacLinija].KontaktBroj = provjera;
								break;
							case 3: 
								int pomocna;
								if (provjera == "Dijete"){
									Osobe[BrojacLinija].uzrast = Dijete;
								}else if(provjera == "Student"){
									Osobe[BrojacLinija].uzrast = Student;
								}else if (provjera == "Odrasli"){
									Osobe[BrojacLinija].uzrast = Odrasli;
								}else{
									Osobe[BrojacLinija].uzrast = Penzioner;
								}								
							case 4:
								if (provjera == "Jednosmjerna"){
									Osobe[BrojacLinija].karta = Jednosmjerna;
								}else{
									Osobe[BrojacLinija].karta = Povratna;
								}
								break;
						}
					}
				}
			}while(br1<5);
			BrojacLinija++;
		}
		Putnici.close();
		string IME, PREZIME;
		cin.ignore();
		system ("cls");
		cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
		cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
		cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
		cout << "\t\n\t OTKAZIVANJE REZERVACIJE\t" << endl;
		cout<<"\t"<<setfill('-')<<setw(25)<<"-"<<endl;
		cout << "\tIme: ";
		getline (cin, IME);
		cout << "\tPrezime: ";
		getline (cin, PREZIME);
		cout<<"\t"<<setfill('-')<<setw(25)<<"-"<<endl;
		int indeks=-1;
		for (int i=0; i<BrojacLinija; i++){
			if (IME.compare(Osobe[i].ime)==0 && PREZIME.compare(Osobe[i].prezime)==0){
				indeks=i;
				break;
			}
		}
		if (indeks==-1){
			cout << "\nNe postoji rezervacija na uneseno ime i prezime!\n" << endl;
			system ("pause");
			system ("cls");
			Meni();
		}
		Putnici.open(line1);
		ofstream Novi;
		Novi.open("temp.txt");
		char c;
		int BR=1;
		while (Putnici.get(c)){
			if (c== '\n'){
				BR++;
			}
			if (BR!= (indeks+1)){
				Novi << c;
			}
		}
		Novi.close();
		Putnici.close();
		char pomocna[line1.length()];
		strcpy (pomocna, line1.c_str());
		remove (pomocna);
		rename ("temp.txt", pomocna);
		cout << "\nVasa rezervacija je uspjesno otkazana!\n" << endl;
		system ("pause");
		system ("cls");
		Meni();	
				
		}else if(TreciMeni == 5){ // 2.5. Povratak na pocetni meni
			system ("cls");
			Meni ();
		}
	}else if(PrviMeni == 3){ // 1.3. Odjava
		system ("cls");
		cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
		cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
		cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
		cout << "\n\t    Odjavljivanje...." << endl<<endl;
		system ("pause");
		system ("cls");
		cin.ignore();
		Prijava();
		system ("cls");
		Meni ();
	}else if(PrviMeni == 4){ // 1.4. Gasenje (kraj programa)
		system ("cls");
		cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
		cout<<"\t| AUTOBUSKA STANICA U ZENICI |\n";
		cout<<"\t"<<setfill('-')<<setw(30)<<"-"<<endl;
		cout << "\n\t    Iskljucivanje...." << endl<<endl;
		system ("pause");
		system ("cls");
		return;
	}
	delete [] Podaci;
}

int main(){
	system("Color 2");
//	Prijava ();
//	system ("cls");
	Meni ();	
	return 0;
}
