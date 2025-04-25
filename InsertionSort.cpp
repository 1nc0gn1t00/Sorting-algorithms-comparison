#include "InsertionSort.h"

// Definicja metody sortuj
template<typename T>
void InsertionSort<T>::sortuj(T* tablica, int rozmiar) {
    // Przechodzimy przez każdy element tablicy zaczynając od drugiego
    for (int i = 1; i < rozmiar; i++) {
        T klucz = tablica[i];   // Zapamiętujemy aktualny element (klucz)
        int j = i - 1;

        // Przesuwamy elementy większe od klucza o jedno miejsce w prawo,
        // dopóki nie znajdziemy odpowiedniego miejsca dla klucza
        while (j >= 0 && tablica[j] > klucz) {
            tablica[j + 1] = tablica[j];  // Przesunięcie elementu
            j--;
        }

        // Wstawiamy klucz na odpowiednie miejsce w posortowanej części tablicy
        tablica[j + 1] = klucz;
    }
}

// Definicja metody zwracającej nazwę algorytmu
template<typename T>
const char* InsertionSort<T>::nazwa() const {
    return "Insertion Sort";
}

// Generujemy specjalizacje klasy dla typów int i float,
// aby kompilator wiedział, że te wersje mają być utworzone.
template class InsertionSort<int>;
template class InsertionSort<float>;
