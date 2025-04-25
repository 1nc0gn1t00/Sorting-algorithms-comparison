#ifndef HEAPSORT_H
#define HEAPSORT_H

#include "Sortowanie.h"

// Deklaracja szablonu klasy HeapSort
template <typename T>
class HeapSort : public Sortowanie<T> {
public:
    void sortuj(T* tablica, int n) override;
    const char* nazwa() const override;

private:
    void heapify(T* tablica, int n, int i);
};

#endif // HEAPSORT_H
