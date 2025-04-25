#include "HeapSort.h"
#include <algorithm>

// Funkcja pomocnicza przywracająca właściwości kopca maksymalnego
template <typename T>
void HeapSort<T>::heapify(T* tablica, int rozmiar, int korzen) {
    int najwiekszy = korzen;         // Zakładamy, że korzeń jest największy
    int lewy = 2 * korzen + 1;       // Indeks lewego dziecka
    int prawy = 2 * korzen + 2;      // Indeks prawego dziecka

    // Jeśli lewe dziecko istnieje i jest większe od korzenia
    if (lewy < rozmiar && tablica[lewy] > tablica[najwiekszy])
        najwiekszy = lewy;

    // Jeśli prawe dziecko istnieje i jest większe od dotychczas największego
    if (prawy < rozmiar && tablica[prawy] > tablica[najwiekszy])
        najwiekszy = prawy;

    // Jeśli największy nie jest korzeniem, zamieniamy to i rekurencyjnie przywracamy kopiec
    if (najwiekszy != korzen) {
        std::swap(tablica[korzen], tablica[najwiekszy]);
        heapify(tablica, rozmiar, najwiekszy);
    }
}

// Główna funkcja sortująca
template <typename T>
void HeapSort<T>::sortuj(T* tablica, int rozmiar) {
    // Budowanie kopca maksymalnego
    for (int i = rozmiar / 2 - 1; i >= 0; --i)
        heapify(tablica, rozmiar, i);

    // Wyciąganie elementów z kopca jeden po drugim
    for (int i = rozmiar - 1; i >= 0; --i) {
        std::swap(tablica[0], tablica[i]); // Przenieś największy element na koniec
        heapify(tablica, i, 0);            // Przywróć kopiec dla pozostałych elementów
    }
}

// Zwraca nazwę algorytmu
template <typename T>
const char* HeapSort<T>::nazwa() const {
    return "Heap Sort";
}

template class HeapSort<int>;
template class HeapSort<float>;
