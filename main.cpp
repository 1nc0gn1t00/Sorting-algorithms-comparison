#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <algorithm>
#include <random>

#include "InsertionSort.h"
#include "HeapSort.h"
#include "ShellSort.h"
#include "QuickSort.h"

using namespace std;
using namespace chrono;

template <typename T>
void wyswietlTablice(T* tablica, int n) {
    for (int i = 0; i < n; i++) {
        cout << tablica[i] << " ";
    }
    cout << endl;
}

template <typename T>
void wczytajTabliceZPliku(T*& tablica, int& n, const string& nazwaPliku) {
    ifstream plik(nazwaPliku);
    if (!plik.is_open()) {
        cout << "Blad otwarcia pliku!" << endl;
        return;
    }

    plik >> n;
    if (plik.fail() || n <= 0) {
        cout << "Blad: Nieprawidlowy format pliku lub rozmiar tablicy!" << endl;
        plik.close();
        return;
    }

    delete[] tablica;
    tablica = new T[n];

    for (int i = 0; i < n; i++) {
        T wartosc;
        plik >> wartosc;
        if (plik.fail()) {
            cout << "Blad: Dane w pliku nie sa zgodne z wybranym typem!"<<endl;
            delete[] tablica;
            tablica = nullptr;
            n = 0;
            plik.close();
            return;
        }
        tablica[i] = wartosc;
    }

    plik.close();
    cout << "Tablica wczytana z pliku:" << endl;
    wyswietlTablice(tablica, n);
}

template <typename T>
void generujTabliceLosowa(T*& tablica, int n) {
    delete[] tablica;
    tablica = new T[n];
    for (int i = 0; i < n; i++) {
        if constexpr (std::is_same_v<T, float>) {
            tablica[i] = static_cast<float>(rand() % 10000) / 100.0f; // Zakres dla float
        } else {
            tablica[i] = static_cast<int>(rand() % 100000); // Zwiększony zakres dla int
        }
    }
}

template <typename T>
void generujTabliceRosnaca(T*& tablica, int n) {
    generujTabliceLosowa(tablica, n); // Generowanie losowej tablicy
    std::sort(tablica, tablica + n); // Sortowanie całej tablicy rosnąco
}

template <typename T>
void generujTabliceMalejaca(T*& tablica, int n) {
    generujTabliceLosowa(tablica, n); // Generowanie losowej tablicy
    std::sort(tablica, tablica + n, std::greater<T>()); // Sortowanie całej tablicy malejąco
}

template <typename T>
void generujTabliceCzesciowoPosortowana(T*& tablica, int n, double procent) {
    generujTabliceLosowa(tablica, n); // Generowanie losowej tablicy
    int sortedPart = static_cast<int>(n * procent); // Obliczenie rozmiaru posortowanej części
    std::sort(tablica, tablica + sortedPart); // Sortowanie części tablicy rosnąco
    // Pozostała część pozostaje losowa
}

// Pomocnicza procedura sprawdzająca poprawnosc uporzadkowania elemetow w tablicy
template <typename T>
bool czyPosortowana(T* tablica, int n) {
    for (int i = 1; i < n; i++) {
        if (tablica[i - 1] > tablica[i]) {
            return false;
        }
    }
    return true;
}

template <typename T, typename Sorter>
void wykonajISprawdzCzas(Sorter& algorytm, T* tablica, int n, const string& nazwa) {
    T* kopia = new T[n];
    copy(tablica, tablica + n, kopia);
    cout << "\nSortowanie: " << nazwa << endl;
    cout << "Tablica przed sortowaniem:" << endl;
    wyswietlTablice(kopia, n);
    auto start = high_resolution_clock::now();
    algorytm.sortuj(kopia, n);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Tablica po sortowaniu:" << endl;
    wyswietlTablice(kopia, n);
    cout << "Czas sortowania: " << duration.count() / 1000.0 << " ms" << endl;

    if (czyPosortowana(kopia, n)) {
        cout << "Weryfikacja: Tablica jest poprawnie posortowana." << endl;
    } else {
        cout << "Weryfikacja: Tablica NIE jest poprawnie posortowana!" << endl;
    }

    delete[] kopia;
}

template <typename T, typename Sorter>
double wielokrotnePomiary(Sorter& algorytm, int rozmiarTablicy, int liczbaPowtorzen, int typRozkladu) {
    double sumaCzasow = 0.0;

    for (int i = 0; i < liczbaPowtorzen; i++) {
        T* tablica = nullptr;

        // Generowanie tablicy w zależności od wybranego rozkładu
        switch (typRozkladu) {
            case 1: generujTabliceLosowa(tablica, rozmiarTablicy); break;
            case 2: generujTabliceRosnaca(tablica, rozmiarTablicy); break;
            case 3: generujTabliceMalejaca(tablica, rozmiarTablicy); break;
            case 4: generujTabliceCzesciowoPosortowana(tablica, rozmiarTablicy, 0.33); break;
            case 5: generujTabliceCzesciowoPosortowana(tablica, rozmiarTablicy, 0.66); break;
            default:
                cout << "Nieprawidlowy typ rozkladu!" << endl;
                delete[] tablica;
                return -1.0;
        }

        // Mierzenie czasu sortowania
        auto start = high_resolution_clock::now();
        algorytm.sortuj(tablica, rozmiarTablicy);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);

        sumaCzasow += duration.count();
        delete[] tablica;
    }

    // Zwracanie średniego czasu w milisekundach
    return sumaCzasow / liczbaPowtorzen / 1000.0;
}

template <typename T, typename Sorter>
void wielokrotnePomiaryDlaPredefiniowanychRozmiarow(Sorter& algorytm, int typRozkladu) {
    vector<int> rozmiary = {10000, 20000, 40000, 80000, 160000, 320000, 640000};
    cout << "\nTestowanie algorytmu: " << algorytm.nazwa() << " ---" << endl;
    cout << "Rozklad danych: " << typRozkladu << endl;

    // Nagłówki tabeli
    cout << left << setw(15) << "Rozmiar" << setw(15) << "Czas [ms]" << endl;

    for (int rozmiar : rozmiary) {
        double sredniCzas = wielokrotnePomiary<T, Sorter>(algorytm, rozmiar, 2, typRozkladu);
        // Wyświetlanie wierszy tabeli
        cout << left << setw(15) << rozmiar << setw(15) << fixed << setprecision(3) << sredniCzas << endl;
    }
}

template <typename T>
void obslugaMenu() {
    T* tablica = nullptr;
    int n = 0;

    while (true) {
        cout << "\nMENU GLOWNE" << endl;
        cout << "1. Wczytaj tablice z pliku" << endl;
        cout << "2. Wygeneruj losowa tablice" << endl;
        cout << "3. Wyswietl aktualna tablice" << endl;
        cout << "4. Sortuj tablice" << endl;
        cout << "5. Wielokrotne pomiary dla dowolnego rozmiaru tablic" << endl;
        cout << "6. Wielokrotne pomiary dla predefiniowanych rozmiarow tablic (10k,20k,...,640k)" << endl;
        cout << "0. Wyjscie" << endl;
        cout << "Wybierz opcje: ";
        int opcja;
        cin >> opcja;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Nieprawidlowe dane!" << endl;
            continue;
        }

        switch (opcja) {
            case 1: {
                string nazwaPliku;
                cout << "Podaj nazwe pliku: ";
                cin >> nazwaPliku;
                wczytajTabliceZPliku(tablica, n, nazwaPliku);
                break;
            }
            case 2: {
                cout << "Podaj rozmiar tablicy: ";
                cin >> n;
                int typRozkladu;
                cout << "TYP ROZKLADU DANYCH W TABLICY" << endl;
                cout << "1. Losowa" << endl;
                cout << "2. Posortowana rosnaco" << endl;
                cout << "3. Posortowana malejaco" << endl;
                cout << "4. Czesciowo posortowana (33%)" << endl;
                cout << "5. Czesciowo posortowana (66%)" << endl;
                cout << "Wybierz opcje: ";
                cin >> typRozkladu;

                switch (typRozkladu) {
                    case 1: generujTabliceLosowa(tablica, n); break;
                    case 2: generujTabliceRosnaca(tablica, n); break;
                    case 3: generujTabliceMalejaca(tablica, n); break;
                    case 4: generujTabliceCzesciowoPosortowana(tablica, n, 0.33); break;
                    case 5: generujTabliceCzesciowoPosortowana(tablica, n, 0.66); break;
                    default: cout << "Nieprawidlowy wybor!" << endl; break;
                }
                break;
            }
            case 3: {
                if (n == 0) {
                    cout << "Brak danych do wyswietlenia!" << endl;
                } else {
                    wyswietlTablice(tablica, n);
                }
                break;
            }
            case 4: {
                if (n == 0) {
                    cout << "Brak tablicy do posortowania!" << endl;
                    break;
                }
                int wybor;
                cout << "\nALGORYTM SORTOWANIA" << endl;
                cout << "1. Insertion Sort" << endl;
                cout << "2. Heap Sort" << endl;
                cout << "3. Shell Sort" << endl;
                cout << "4. Quick Sort" << endl;
                cout << "0. Powrot" << endl;
                cout << "Wybierz opcje: ";
                cin >> wybor;

                switch (wybor) {
                    case 1: {
                        InsertionSort<T> alg;
                        wykonajISprawdzCzas(alg, tablica, n, "Insertion Sort");
                        break;
                    }
                    case 2: {
                        HeapSort<T> alg;
                        wykonajISprawdzCzas(alg, tablica, n, "Heap Sort");
                        break;
                    }
                    case 3: {
                        int sekwencja;
                        cout << "\nWybierz sekwencje odstepow dla Shell Sorta:\n";
                        cout << "1. Shell (n/2, n/4, ...)\n";
                        cout << "2. Frank & Lazarus (1, 4, 13, ...)\n";
                        cout << "Wybierz opcje: ";
                        cin >> sekwencja;
                        TypPrzerw shellType;
                        switch (sekwencja) {
                            case 1:
                                shellType = TypPrzerw::Shell;
                                break;
                            case 2:
                                shellType = TypPrzerw::FrankLazarus;
                                break;
                            default:
                                cout << "Nieprawidlowy wybor! Ustawiono domyslnie sekwencje odstepow Shella." << endl;
                                shellType = TypPrzerw::Shell;
                                break;
                        }
                        ShellSort<T> alg(shellType);
                        wykonajISprawdzCzas(alg, tablica, n, alg.nazwa());
                        break;
                    }
                    case 4: {
                        int piwot;
                        cout << "\nWybierz strategie pivotu dla Quick Sorta:\n";
                        cout << "1. Pierwszy\n";
                        cout << "2. Ostatni\n";
                        cout << "3. Srodkowy\n";
                        cout << "4. Losowy\n";
                        cout << "Wybierz opcje: ";
                        cin >> piwot;
                        PivotStrategy qType = static_cast<PivotStrategy>(piwot - 1);
                        QuickSort<T> alg(qType);
                        wykonajISprawdzCzas(alg, tablica, n, alg.nazwa());
                        break;
                    }
                    case 0: break;
                    default: cout << "Nieprawidlowy wybor!" << endl;
                }
                break;
            }
            case 5: {
                int liczbaPowtorzen,rozmiarTablicy, typRozkladu, alg;
                cout << "WIELOKROTNE POMIARY\n";
                cout << "Podaj rozmiar tablicy: ";
                cin >> rozmiarTablicy;
                cout << "Podaj liczbe testow: ";
                cin >> liczbaPowtorzen;

                cout << "TYP ROZKLADU DANYCH W TABLICY\n";
                cout << "1. Losowa\n";
                cout << "2. Rosnaca\n";
                cout << "3. Malejaca\n";
                cout << "4. Czesciowo posortowana (33%)\n";
                cout << "5. Czesciowo posortowana (66%)\n";
                cout << "Wybierz opcje: ";
                cin >> typRozkladu;

                cout << "\nALGORYTM\n";
                cout << "1. Insertion Sort\n";
                cout << "2. Heap Sort\n";
                cout << "3. Shell Sort\n";
                cout << "4. Quick Sort\n";
                cout << "0. Powrot\n";
                cout << "Wybierz opcje: ";
                cin >> alg;

                switch (alg) {
                    case 1: {
                        InsertionSort<T> s;
                        double avg = wielokrotnePomiary<T, InsertionSort<T>>(s, rozmiarTablicy, liczbaPowtorzen, typRozkladu);
                        cout << "Insertion Sort - sredni czas: " << fixed << setprecision(3) << avg << " ms\n";
                        break;
                    }
                    case 2: {
                        HeapSort<T> s;
                        double avg = wielokrotnePomiary<T, HeapSort<T>>(s, rozmiarTablicy, liczbaPowtorzen, typRozkladu);
                        cout << "Heap Sort - sredni czas: " << fixed << setprecision(3) << avg << " ms\n";
                        break;
                    }
                    case 3: {
                        int sekwencja;
                        cout << "\nWybierz sekwencje odstepow dla Shell Sorta:\n";
                        cout << "1. Shell (n/2, n/4, ...)\n";
                        cout << "2. Frank & Lazarus (1, 4, 13, ...)\n";
                        cout << "Wybierz opcje: ";
                        cin >> sekwencja;
                        TypPrzerw shellType = sekwencja == 2 ? TypPrzerw::FrankLazarus : TypPrzerw::Shell;
                        ShellSort<T> alg(shellType);
                        double avg = wielokrotnePomiary<T, ShellSort<T>>(alg, rozmiarTablicy, liczbaPowtorzen, typRozkladu);
                        cout << alg.nazwa() << " - sredni czas: " << fixed << setprecision(3) << avg << " ms\n";
                        break;
                    }
                    case 4: {
                        int p;
                        cout << "\nWybierz strategie pivotu dla Quick Sorta:\n";
                        cout << "1. Pierwszy\n";
                        cout << "2. Ostatni\n";
                        cout << "3. Srodkowy\n";
                        cout << "4. Losowy\n";
                        cout << "Wybierz opcje: ";
                        cin >> p;
                        PivotStrategy ps = static_cast<PivotStrategy>(p - 1);
                        QuickSort<T> s(ps);
                        double avg = wielokrotnePomiary<T, QuickSort<T>>(s, rozmiarTablicy, liczbaPowtorzen, typRozkladu);
                        cout << s.nazwa() << " - sredni czas: " << fixed << setprecision(3) << avg << " ms\n";
                        break;
                    }
                }
                break;
            }
            case 6: {
                int typRozkladu, alg;
                cout << "TYP ROZKLADU DANYCH W TABLICY\n";
                cout << "1. Losowa\n";
                cout << "2. Rosnaca\n";
                cout << "3. Malejaca\n";
                cout << "4. Czesciowo posortowana (33%)\n";
                cout << "5. Czesciowo posortowana (66%)\n";
                cout << "Wybierz opcje: ";
                cin >> typRozkladu;

                cout << "\nALGORYTM\n";
                cout << "1. Insertion Sort\n";
                cout << "2. Heap Sort\n";
                cout << "3. Shell Sort\n";
                cout << "4. Quick Sort\n";
                cout << "0. Powrot\n";
                cout << "Wybierz opcje: ";
                cin >> alg;

                switch (alg) {
                    case 1: {
                        InsertionSort<T> s;
                        wielokrotnePomiaryDlaPredefiniowanychRozmiarow<T, InsertionSort<T>>(s, typRozkladu);
                        break;
                    }
                    case 2: {
                        HeapSort<T> s;
                        wielokrotnePomiaryDlaPredefiniowanychRozmiarow<T, HeapSort<T>>(s, typRozkladu);
                        break;
                    }
                    case 3: {
                        int sekwencja;
                        cout << "\nWybierz sekwencje odstepow dla Shell Sorta:\n";
                        cout << "1. Shell (n/2, n/4, ...)\n";
                        cout << "2. Frank & Lazarus (1, 4, 13, ...)\n";
                        cout << "Wybierz opcje: ";
                        cin >> sekwencja;
                        TypPrzerw shellType = sekwencja == 2 ? TypPrzerw::FrankLazarus : TypPrzerw::Shell;
                        ShellSort<T> alg(shellType);
                        wielokrotnePomiaryDlaPredefiniowanychRozmiarow<T, ShellSort<T>>(alg, typRozkladu);
                        break;
                    }
                    case 4: {
                        int p;
                        cout << "\nWybierz strategie pivotu dla Quick Sorta:\n";
                        cout << "1. Pierwszy\n";
                        cout << "2. Ostatni\n";
                        cout << "3. Srodkowy\n";
                        cout << "4. Losowy\n";
                        cout << "Wybierz opcje: ";
                        cin >> p;
                        PivotStrategy ps = static_cast<PivotStrategy>(p - 1);
                        QuickSort<T> s(ps);
                        wielokrotnePomiaryDlaPredefiniowanychRozmiarow<T, QuickSort<T>>(s, typRozkladu);
                        break;
                    }
                }
                break;
            }
            case 0:
                delete[] tablica;
                return;
            default:
                cout << "Nieprawidlowy wybor!" << endl;
                break;
        }
    }
}

int main() {
    srand(time(NULL));
    while (true) {
        cout << "WYBIERZ TYP DANYCH\n";
        cout << "1. int\n";
        cout << "2. float\n";
        cout << "0. Wyjscie\n";
        cout << "Wybierz opcje: ";
        int typDanych;
        cin >> typDanych;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Nieprawidlowe dane!" << endl;
            continue;
        }

        if (typDanych == 0) {
            cout << "Koniec programu." << endl;
            break;
        } else if (typDanych == 1) {
            obslugaMenu<int>();
        } else if (typDanych == 2) {
            obslugaMenu<float>();
        } else {
            cout << "Nieprawidlowy wybor!" << endl;
        }
    }

    return 0;
}