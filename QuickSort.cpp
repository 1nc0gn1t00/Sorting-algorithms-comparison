#include "QuickSort.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

// konstruktor
template<typename T>
QuickSort<T>::QuickSort(PivotStrategy strat)
: strategy(strat) // Inicjalizacja strategii wyboru pivota
{
    std::srand(static_cast<unsigned>(std::time(nullptr))); // Inicjalizacja generatora liczb pseudolosowych
}

// sortuj() wywołuje rekurencyjny quickSort
template<typename T>
void QuickSort<T>::sortuj(T* tablica, int n) {
    quickSort(tablica, 0, n - 1); // Wywołanie funkcji quickSort na całej tablicy
}

template<typename T>
const char* QuickSort<T>::nazwa() const {
    switch (strategy) { // Zwraca nazwę algorytmu w zależności od strategii wyboru pivota
        case PivotStrategy::First:  return "Quick Sort (Lewy pivot)";
        case PivotStrategy::Last:   return "Quick Sort (Prawy pivot))";
        case PivotStrategy::Middle: return "Quick Sort (Srodkowy pivot)";
        case PivotStrategy::Random: return "Quick Sort (Losowy pivot)";
    }
    return "Quick Sort"; // Domyślna nazwa
}

// quickSort (rekurencja)
template<typename T>
void QuickSort<T>::quickSort(T* a, int l, int r) {
    if (l >= r) return; // Warunek zakończenia rekurencji
    int p = partition(a, l, r); // Podział tablicy na części względem pivota
    quickSort(a, l, p - 1); // Rekurencyjne sortowanie lewej części
    quickSort(a, p + 1, r); // Rekurencyjne sortowanie prawej części
}

// podzialy
template<typename T>
int QuickSort<T>::partition(T* a, int l, int r) {
    int pivotIndex = choosePivot(l, r); // Wybór indeksu pivota
    std::swap(a[pivotIndex], a[r]); // Przeniesienie pivota na koniec
    T pivot = a[r]; // Ustawienie pivota
    int i = l; // Indeks dla mniejszej części
    for (int j = l; j < r; ++j) { // Iteracja po elementach
        if (a[j] < pivot) { // Jeśli element jest mniejszy od pivota
            std::swap(a[i], a[j]); // Zamiana elementów
            ++i; // Przesunięcie indeksu
        }
    }
    std::swap(a[i], a[r]); // Przeniesienie pivota na właściwe miejsce
    return i; // Zwrócenie pozycji pivota
}

// Wybor pivota
template<typename T>
int QuickSort<T>::choosePivot(int l, int r) {
    switch (strategy) { // Wybór pivota w zależności od strategii
        case PivotStrategy::First:  return l; // Pierwszy element
        case PivotStrategy::Last:   return r; // Ostatni element
        case PivotStrategy::Middle: return l + (r - l) / 2; // Środkowy element
        case PivotStrategy::Random: return l + std::rand() % (r - l + 1); // Losowy element
    }
    return l + (r - l) / 2; // Domyślnie srodkowy element
}

template class QuickSort<int>; // Instancja dla typu int
template class QuickSort<float>; // Instancja dla typu float