#include "ShellSort.h"
#include <algorithm> // dla std::reverse

// Konstruktor – ustawia wybrany typ sekwencji przerw
template<typename T>
ShellSort<T>::ShellSort(TypPrzerw typ) : typPrzerw(typ) {}

// Generowanie sekwencji przerw – w zależności od wybranego typu
template<typename T>
std::vector<int> ShellSort<T>::generujPrzerwy(int rozmiar) {
    std::vector<int> przerwy;

    if (typPrzerw == TypPrzerw::Shell) {
        // Sekwencja Shella – kolejne podziały przez 2
        for (int krok = rozmiar / 2; krok > 0; krok /= 2)
            przerwy.push_back(krok);
    } else {
        // Sekwencja Franka i Lazarusa: h = 1, 4, 13, ...
        int krok = 1;
        while (krok < rozmiar) {
            przerwy.push_back(krok);
            krok = 3 * krok + 1;
        }
        std::reverse(przerwy.begin(), przerwy.end()); // największe przerwy najpierw
    }

    return przerwy;
}

// Implementacja algorytmu sortowania Shella
template<typename T>
void ShellSort<T>::sortuj(T* tablica, int rozmiar) {
    auto przerwy = generujPrzerwy(rozmiar);

    // Dla każdej przerwy wykonujemy wstawianie z krokiem
    for (int krok : przerwy) {
        for (int i = krok; i < rozmiar; ++i) {
            T element = tablica[i];
            int j = i;

            // Przesuwaj elementy większe od aktualnego o krok w prawo
            while (j >= krok && tablica[j - krok] > element) {
                tablica[j] = tablica[j - krok];
                j -= krok;
            }
            tablica[j] = element;
        }
    }
}

// Zwracamy nazwę algorytmu w zależności od wybranej sekwencji przerw
template<typename T>
const char* ShellSort<T>::nazwa() const {
    return typPrzerw == TypPrzerw::Shell
        ? "Shell Sort (Shell)"
        : "Shell Sort (Frank & Lazarus)";
}

template class ShellSort<int>;
template class ShellSort<float>;
