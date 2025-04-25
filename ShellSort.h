#ifndef SHELLSORT_H
#define SHELLSORT_H

#include "Sortowanie.h"
#include <vector>

enum class TypPrzerw {
    Shell,
    FrankLazarus
};

// Deklaracja klasy-szablonu ShellSort
template <typename T>
class ShellSort : public Sortowanie<T> {
public:
    ShellSort(TypPrzerw typPrzerw = TypPrzerw::Shell);
    void sortuj(T* tablica, int n) override;
    const char* nazwa() const override;

private:
    TypPrzerw typPrzerw;
    std::vector<int> generujPrzerwy(int n);
};

#endif // SHELLSORT_H
