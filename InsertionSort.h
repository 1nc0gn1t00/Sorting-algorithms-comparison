#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H

#include "Sortowanie.h"

// Deklaracja klasy-szablonu
template<typename T>
class InsertionSort : public Sortowanie<T> {
public:
    void sortuj(T* tablica, int rozmiar) override;
    const char* nazwa() const override;
};

#endif // INSERTIONSORT_H
