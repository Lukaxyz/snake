#include <iostream>
#include <windows.h>
#include <ctime>
#include <conio.h>

using namespace std;

//Struktura pozicija/koordinata.
struct pozicija {
int x,y; //koordinate za poziciju
};

//Bazna Klasa a ujedno i polje po kojem ce se zmija pomjerati.
class polje_klasa {
  //Odredjuje visinu i sirinu polja
    static const int visina;
    static const int sirina;
    char * polje; //Pokazivac na dinamicki alocirani niz
    polje_klasa(const polje_klasa &A); //Konstruktor kopije

public:
  //Konstruktor polja
    polje_klasa() {
  polje = new char[visina];
  for(int i = 0; i < visina; ++i) {
  polje[i] = new char[sirina];
	}
  }
  //Destruktor, brise dinamicki alociran niz
  	~polje_klasa() {
  for(int i = 0; i < visina; ++i) {
  	delete[] polje[i];
	}
  delete[] polje;
}
  //Rezervise prostor polja gdje ce biti
void ispisi() {
  for(int i = 0; i < visina; ++i) {
      for(int j = 0; j < sirina; ++j) {
          cout << polje[i][j];
              }
              cout << endl;
        }
    }
  	/*
  	*  Cisti polje, i uredjuje ga nakon pojedene hrane.
  	*  Pravi Polje sa odredjenim simbolima.
  	*/
    void cisti() {
        for(int i = 0; i < visina; ++i) {
            for(int j = 0; j < sirina; ++j) {
                if (i == visina - 1 || i == 0){ polje[i][j] = char(45);}
                else if(j == sirina -1 || j == 0 ){ polje[i][j] = char(124);
                }else polje[i][j] = ' ';
            }
        }
    }
  //Vraca Sirinu/Visinu Polja
    int daj_sirina() const {return sirina;}
    int daj_visina() const {return visina;}
  //Ispunjava polje charovima "sta".
    void nacrtaj(int y, int x, char sta){
        polje[y][x] = sta;
    }
} polje;

//Klasa za hranu.
class hrana_klasa {
  	//Struktura poz.
    pozicija poz;
    char simbol;
public:
    hrana_klasa(): simbol('$'), poz() {
      //Stavlja poziciju na -1.
        poz.x = poz.y = -1;
    }
    void stavi_poz(int x, int y) {
      //Omogucava mijenjanje pozicije.
        poz.x = x;
        poz.y = y;
    }
    void repozicija(const polje_klasa & polje) {
        poz.x = (rand() % polje.daj_sirina());
        poz.y = (rand() % polje.daj_visina());
    }
int daj_x() const {return poz.x;}
int daj_y() const {return poz.y;}
char daj_simbol() const {return simbol;}
} hrana;

//Klasa Zmija.
class zmija_klasa {
  //Komande za pomjeranje.
    enum {UP, DOWN, LEFT, RIGHT} dir;
    char simbol, glava_simbol;
    pozicija poz[100];
  //Pozicija glave ce se mijenjat.
    pozicija & glava;
    int brzina;
    int velicina;
    bool mozePomjerit;
public:
  //Konstruktor - konstrukcija zmije
    zmija_klasa(int x, int y):
        simbol('o'), glava_simbol('O'), poz(),
        brzina(1), velicina(1), dir(RIGHT),
        glava(poz[0]), mozePomjerit(true)
    {
        poz[0].x = x;
        poz[0].y = y;
    }
  //Provjerava da li je zmija pojela hranu
    bool provjeri_hranu(const hrana_klasa &hrana) {
        if(hrana.daj_x() == glava.x && hrana.daj_y() == glava.y)
          //Povecava se zmija za 1 mjesto
            {velicina += 1;
            return true;
        }
        return false;
    }
  //Ne dozvoljava pomjeranje zmije za 180 stepeni.
    void unesi(const polje_klasa & polje) {
        if(GetAsyncKeyState(VK_UP) && dir != DOWN) {
            dir = UP;
        }
        if(GetAsyncKeyState(VK_DOWN) && dir != UP) {
            dir = DOWN;
        }
        if(GetAsyncKeyState(VK_LEFT) && dir != RIGHT) {
            dir = LEFT;
        }
        if(GetAsyncKeyState(VK_RIGHT) && dir != LEFT) {
            dir = RIGHT;
        }
    }
  //Gleda stranu zmije na kojoj se nalazi.
    void pomjeriSe(const polje_klasa & polje) {
        pozicija sljedeca = {0, 0};
        switch(dir) {
        case UP:
            sljedeca.y = -brzina;
            break;
        case DOWN:
            sljedeca.y = brzina;
            break;
        case LEFT:
            sljedeca.x = -brzina;
            break;
        case RIGHT:
            sljedeca.x = brzina;
        }
        for(int i = velicina - 1; i > 0; --i) {
            poz[i] = poz[i-1];
        }
        glava.x += sljedeca.x;
        glava.y += sljedeca.y;
        if(glava.x < 0 || glava.y < 0 || glava.x >= polje.daj_sirina() || glava.y >= polje.daj_visina()) {
            throw "Izasli ste sa polja. ";}

        for (int i=1; i<sizeof(poz); i++){
            if(glava.x==poz[i].x && glava.y==poz[i].y)
                throw "Pojeli ste se. ";
        }}
  		//Pravi glavu ili tijelo zavisno od velicine zmije
    void nacrtaj(polje_klasa & polje) {
        for(int i = 0; i < velicina; ++i) {
            if(i == 0) {
                polje.nacrtaj(poz[i].y, poz[i].x, glava_simbol);
            } else {
                polje.nacrtaj(poz[i].y, poz[i].x, simbol);
            }
        }
    }
    int daj_x() const { return glava.x; }
    int daj_y() const { return glava.y; }
    char daj_simbol() const { return simbol; }
} zmijica(1, 1);

//Definiramo velicinu polja.
const int polje_klasa::visina = 15;
const int polje_klasa::sirina = 35;


int main() {
  srand(time(0));
system("Color 00");
int opcija;
cout << R"(
                                  .---.
                                  |   |
               __  __   ___   .--.'---'
              |  |/  `.'   `. |__|.---.
              |   .-.  .-.   '.--.|   |
              |  |  |  |  |  ||  ||   |    __
    .--------.|  |  |  |  |  ||  ||   | .:--.'.
    |____    ||  |  |  |  |  ||  ||   |/ |   \ |
        /   / |  |  |  |  |  ||  ||   |`" __ | |
      .'   /  |__|  |__|  |__||__||   | .'.''| |
     /    /___                 __.'   '/ /   | |_
    |         |               |      ' \ \._,\ '/
    |_________|               |____.'   `--'  `"
        )" << endl;

      cout << "\t************************************\n\t************************************\n";



     //Prikazuje opcije - Glavni Meni
     cout << right << "\t1) \t Pokrenite Igru " << endl;
     cout << right << "\t2) \t Izadjite \n" << endl;

      cout << "\t************************************\n\t************************************\n";
     //Trazi od korisnika unos 1 ili 2
     cout << "\t\tIzaberite : ";
  do {
     cin >> opcija;

     if (opcija > 3) {
            cout << "Pogresan ste broj unijeli. ";
            break;
     }
     else if(opcija == 1) // Ako je korisnik upisao 1

     {
	int bodovi = 0;
    polje.cisti();
    hrana.stavi_poz(5, 5);
    while(1) {
        polje.cisti();
        zmijica.unesi(polje);
        try {
            zmijica.pomjeriSe(polje);

        } catch (const char * er) {
            polje.cisti();
            cerr << er << endl;
            system("pause");
            return -1;
        }
        zmijica.nacrtaj(polje);
        polje.nacrtaj(hrana.daj_y(), hrana.daj_x(), hrana.daj_simbol());
        if(zmijica.provjeri_hranu(hrana)) {
            hrana.repozicija(polje);
        	bodovi += 10;}

        cout << "\t Bodovi: " << bodovi << endl;

        polje.ispisi();
        Sleep(1000/35); //Refresh Rate za Igru
        system("Color 8A");
        system("cls");
    		}
     	}

  }
  while(opcija != 2);  //Ako je korisnik unio 2 - zavrsava se program

    return 0;
}

