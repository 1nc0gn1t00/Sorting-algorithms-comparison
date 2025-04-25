#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "Sortowanie.h"

// Strategie wyboru pivota w QuickSort
enum class PivotStrategy {
    First,
    Last,
    Middle,
    Random
};

// Szablon klasy QuickSort ze strategią wyboru pivota
template<typename T>
class QuickSort : public Sortowanie<T> {
public:
    // Konstruktor: przyjmujemy strategię pivotu (domyślnie Middle)
    QuickSort(PivotStrategy strat = PivotStrategy::Middle);

    // Metoda sortująca tablicę o rozmiarze n
    void sortuj(T* tablica, int n) override;

    // Nazwa algorytmu (do wyświetlenia w menu)
    const char* nazwa() const override;

private:
    PivotStrategy strategy;

    // Rekurencyjna funkcja quicksort na przedziale [l, r]
    void quickSort(T* a, int l, int r);

    // Partition: dzieli przedział [l, r] wokół pivotu, zwraca nową pozycję pivotu
    int partition(T* a, int l, int r);

    // Wybiera indeks pivotu w przedziale [l, r]
    int choosePivot(int l, int r);
};

#endif // QUICKSORT_H
