// Adrian Bednarek 203502 ACiR 1A Gra à la Kono
#include <iostream>
#include <cmath>

using namespace std;

const int ROZMIAR_PLANSZY = 4; // stala okreslajaca rozmiar planszy
char plansza[ROZMIAR_PLANSZY][ROZMIAR_PLANSZY] = {
    {'b', 'b', 'b', 'b'},
    {' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' '},
    {'c', 'c', 'c', 'c'}
};

void wyswietlPlansze() {
    cout << "      A   B   C   D\n";
    for (int i = 0; i < ROZMIAR_PLANSZY; i++) {
        cout << "    " << "\xce\xcd\xcd\xcd\xce\xcd\xcd\xcd\xce\xcd\xcd\xcd\xce\xcd\xcd\xcd\xb9" << "\n"; // kody ASCII
        cout << "  " << i + 1 << " ";
        for (int j = 0; j < ROZMIAR_PLANSZY; j++) {
            cout << "\xba " << plansza[i][j] << " "; // wyswietlanie pola
        }
        cout << "\xba\n";
    }
    cout << "    \xca\xcd\xcd\xcd\xca\xcd\xcd\xcd\xca\xcd\xcd\xcd\xca\xcd\xcd\xcd\xbc\n";
}

bool czyRuchPoprawny(int x1, int y1, int xDocelowe, int yDocelowe) {
    if (xDocelowe < 0 || xDocelowe >= ROZMIAR_PLANSZY || yDocelowe < 0 || yDocelowe >= ROZMIAR_PLANSZY) // ruch poza plansze
        return false;
    if (plansza[xDocelowe][yDocelowe] != ' ') // pole docelowe musi byc puste
        return false;
    if (abs(x1 - xDocelowe) + abs(y1 - yDocelowe) != 1) // ruch tylko w pionie lub poziomie o jedno pole
        return false;
    if (plansza[x1][y1] == ' ') // wybrane pole nie moze byc puste
        return false;
    return true;
}

bool czyMoznaZbic(int x, int y, int& xDocelowe, int& yDocelowe, int& xP, int& yP) {
    int kierunki[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} }; // mozliwe kierunki ruchu
    char swojPion = plansza[x][y]; // pion gracza wykonujacego ruch
    char przeciwnik = (swojPion == 'b') ? 'c' : 'b'; // pion przeciwnika
    // sprawdzamy kazdy z kierunkow
    for (auto& kierunek : kierunki) {
        int nx = x + kierunek[0];       // pozycja swojego pionka
        int ny = y + kierunek[1];
        int wx = x + 3 * kierunek[0];   // pozycja wroga
        int wy = y + 3 * kierunek[1];
        int cx = x + 2 * kierunek[0];   // pole docelowe (po zbiciu)
        int cy = y + 2 * kierunek[1];

        // sprawdzamy czy pionek gracza przeskakuje nad wlasnym pionem i czy laduje przed wrogiem
        if (nx >= 0 && nx < ROZMIAR_PLANSZY && ny >= 0 && ny < ROZMIAR_PLANSZY &&
            plansza[nx][ny] == swojPion && // nad swoim pionem
            wx >= 0 && wx < ROZMIAR_PLANSZY && wy >= 0 && wy < ROZMIAR_PLANSZY &&
            plansza[wx][wy] == przeciwnik && // wrog obok swojego pionka
            cx >= 0 && cx < ROZMIAR_PLANSZY && cy >= 0 && cy < ROZMIAR_PLANSZY &&
            plansza[cx][cy] == ' ') { // pole docelowe puste
            xDocelowe = cx;
            yDocelowe = cy;
            xP = wx;
            yP = wy;
            return true;
        }
    }
    return false;
}

bool czySaDostepneRuchy(char pion) {
    for (int i = 0; i < ROZMIAR_PLANSZY; i++) {
        for (int j = 0; j < ROZMIAR_PLANSZY; j++) {
            if (plansza[i][j] == pion) {
                int kierunki[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
                for (auto& kierunek : kierunki) {
                    int x2 = i + kierunek[0];
                    int y2 = j + kierunek[1];
                    if (x2 >= 0 && x2 < ROZMIAR_PLANSZY && y2 >= 0 && y2 < ROZMIAR_PLANSZY && plansza[x2][y2] == ' ') // wolne pole obok
                        return true;
                }
            }
        }
    }
    return false;
}

// ruch komputera (b)
void ruchKomputera() {
    for (int i = 0; i < ROZMIAR_PLANSZY; i++) {
        for (int j = 0; j < ROZMIAR_PLANSZY; j++) {
            if (plansza[i][j] == 'b') {    // szukamy bialych pionow
                int xDocelowe, yDocelowe, xp, yp;
                if (czyMoznaZbic(i, j, xDocelowe, yDocelowe, xp, yp)) {
                    plansza[xp][yp] = 'b'; // przesuniecie pionka
                    plansza[i][j] = ' ';   // usuniecie oryginalnej pozycji
                    return;
                }
            }
        }
    }

    // jesli nie można zbic wykonaj zwykly ruch
    for (int i = 0; i < ROZMIAR_PLANSZY; i++) {
        for (int j = 0; j < ROZMIAR_PLANSZY; j++) {
            if (plansza[i][j] == 'b') {
                int kierunki[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
                for (auto& kierunek : kierunki) {
                    int x2 = i + kierunek[0];
                    int y2 = j + kierunek[1];
                    if (czyRuchPoprawny(i, j, x2, y2)) {
                        plansza[x2][y2] = 'b';
                        plansza[i][j] = ' ';
                        return;
                    }
                }
            }
        }
    }
}

// ruch gracza (c)
void ruchGracza(const char* nazwaGracza, char pion) {
    int x1, y1, x2, y2;
    char kolumna1, kolumna2;
    while (true) {
        cout << nazwaGracza << " (" << pion << "), podaj ruch (np. A1 A3): ";
        cin >> kolumna1 >> x1 >> kolumna2 >> x2;
        y1 = kolumna1 - 'A';
        y2 = kolumna2 - 'A';
        x1--; x2--;
        int xDocelowe, yDocelowe, xp, yp;
        if (plansza[x1][y1] == 'b') {
            cout << "Nieprawidlowy ruch. Sprobuj ponownie.\n";
        }
        else if (czyRuchPoprawny(x1, y1, x2, y2)) {
            plansza[x2][y2] = pion;
            plansza[x1][y1] = ' ';
            break;
        }
        else if (czyMoznaZbic(x1, y1, xDocelowe, yDocelowe, xp, yp) && xDocelowe == x2 && yDocelowe == y2) {
            plansza[xp][yp] = pion;
            plansza[x1][y1] = ' ';
            break;
        }
        else {
            cout << "Nieprawidlowy ruch. Sprobuj ponownie.\n";
        }
    }
}

bool czyWygrana(char przeciwnik) {
    if (!czySaDostepneRuchy(przeciwnik))
        return true; // przeciwnik nie ma mozliwosci ruchu
    for (int i = 0; i < ROZMIAR_PLANSZY; i++) {
        for (int j = 0; j < ROZMIAR_PLANSZY; j++) {
            if (plansza[i][j] == przeciwnik)
                return false;
        }
    }
    return true;     // wszystkie pionki przeciwnika zbite
}

int main() {
    setlocale(LC_CTYPE, "LATIN-2"); //kody ascii

    char nazwaGracza1[20], nazwaGracza2[20];
    cout << "Podaj nazwe Gracza 1 (biale - b): ";
    cin >> nazwaGracza1;
    cout << "Podaj nazwe Gracza 2 (czarne - c): ";
    cin >> nazwaGracza2;

    cout << "Stan poczatkowy planszy:\n";
    wyswietlPlansze();

    bool turaBialych = true;
    while (true) {
        if (turaBialych) {
            cout << "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd" << endl;
            cout << nazwaGracza1 << " wykonuje ruch (b):\n";
            ruchKomputera();
        }
        else {
            cout << "\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd\xcd" << endl;
            cout << nazwaGracza2 << " wykonuje ruch (c):\n";
            ruchGracza(nazwaGracza2, 'c');
        }

        wyswietlPlansze();

        if (czyWygrana('c')) {
            cout << "******** " << nazwaGracza1 << " wygrywa!" << " ********\n";
            break;
        }
        if (czyWygrana('b')) {
            cout << "******** " << nazwaGracza2 << " wygrywa!" << " ********\n";
            break;
        }
        turaBialych = !turaBialych;
    }
    return 0;
}