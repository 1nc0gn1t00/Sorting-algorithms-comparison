#ifndef SORTOWANIE_H
#define SORTOWANIE_H

template<typename T>
class Sortowanie {
    // Szablon klasy bazowej dla algorytmów sortowania, umożliwiający obsługę różnych typów danych (T).
public:
    virtual void sortuj(T* tablica, int rozmiar) = 0;
    // Wirtualna metoda, którą muszą zaimplementować klasy pochodne.
    // Służy do sortowania tablicy o danym rozmiarze

    virtual const char* nazwa() const = 0;
    // Wirtualna metoda zwracająca nazwę algorytmu sortowania.
    // Umożliwia identyfikację konkretnej implementacji.

    virtual ~Sortowanie() = default;
    // Wirtualny destruktor z domyślną implementacją.
    // Zapewnia poprawne usuwanie obiektów klas pochodnych.
};

#endif