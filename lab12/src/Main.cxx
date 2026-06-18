#include <iostream>
#include <numeric> // accumulate
#include <iterator> // distance

#include "DataStore.h"

int main() {


    DataStore store(10 /*capacity hint*/);

    store.addDataset("Zbiornik 1", { 12.5, 15.2, 14.8, 13.1 });
    store.addDataset("Zbiornik 2", { 22.0, 19.5, 21.3, 20.7 });
    store.addDataset("Zbiornik 3", { 8.4, 7.9, 9.1, 8.7 });
    store.addDataset("Zbiornik 4", { 31.2, 29.8, 30.5, 32.1 });

    DataSet existing_dataset{"Zbiornik 5", { 17.6, 18.2, 16.9, 17.3 } };
    store.addDataset(existing_dataset);


    const DataSet const_existing_dataset{"Zbiornik 6", { 12.6, 2.2, 21.3, 11.5 } };
    store.addDataset(const_existing_dataset);

    store.addDataset( DataSet{"Zbiornik 7", { 51.2, 32.1, 16.5, 22.4 }} /* r-value/pr-value */);

    std::cout << "\nPrzed sortowaniem:" << std::endl;
    for (auto it = store.begin(); it != store.end(); ++it) {
        std::cout << *it << std::endl;
    }

    store.sort([](auto begin, auto end) { return std::accumulate(begin, end, 0.0) / std::distance(begin, end); }, SortingDirection::ASCENDING);


    std::cout << "\nPo sortowaniu:" << std::endl;
    for (auto it = store.begin(); it != store.end(); ++it) {
        std::cout << *it << std::endl;
    }
}

/*
Oczekiwane:

Przed sortowaniem:
Zbiornik 1: [12.5, 15.2, 14.8, 13.1]
Zbiornik 2: [22, 19.5, 21.3, 20.7]
Zbiornik 3: [8.4, 7.9, 9.1, 8.7]
Zbiornik 4: [31.2, 29.8, 30.5, 32.1]
Zbiornik 5: [17.6, 18.2, 16.9, 17.3]
Zbiornik 6: [12.6, 2.2, 21.3, 11.5]
Zbiornik 7: [51.2, 32.1, 16.5, 22.4]

Po sortowaniu:
Zbiornik 3: [8.4, 7.9, 9.1, 8.7]
Zbiornik 6: [12.6, 2.2, 21.3, 11.5]
Zbiornik 1: [12.5, 15.2, 14.8, 13.1]
Zbiornik 5: [17.6, 18.2, 16.9, 17.3]
Zbiornik 2: [22, 19.5, 21.3, 20.7]
Zbiornik 7: [51.2, 32.1, 16.5, 22.4]
Zbiornik 4: [31.2, 29.8, 30.5, 32.1]

*/

/*
Na wszelki wypadek, zróbmy kumulację rzeczy które musicie wiedzieć o szablonach, poza ich standardowym sposobem wykorzystania 
(i poza zawężaniem typów; to, w razie czego, jest w rozwiązaniach dla fizyków na teamsach, przez SFINAE i koncepty).
Gdyby coś nie było jasne, mówcie. Wytłumaczymy na tablicy.


Założenia:

    1) Używamy wektora do przechowania, żeby móc go sortować [1]. 

    2) Każdy dataset to struktura ze stringiem i wektorem doubli.

    3) Iterator może być zwykłym proxy dla tego z wektora. Nie trzeba pisać dedykowanego. :D


    4) Pierwszy sposób przekazania (ten gdzie podajemy nazwę i dane explicite) korzysta z idiomu copy-and-move.
    Tzn. obydwie wartości powinny być przeniesione do struktury, a sama struktura do wektora.
    Uwaga: to wymaga traktowania argumentu jak wektora, a nie listy inicjalizacyjnej.

    5) Drugi sposób dodawania (ten gdzie przekazujemy istniejącą strukturę) korzysta z idiomu perfect forwarding,
    czyli jednego szablonu, który załatwia wszystkie możliwe konfiguracje i wybiera odpowiednio przeniesienie/kopiowanie.
    (w razie czego pytać!)
    
    
    6) Funkcja sortująca przyjmuje lambdę bez korzystania z std::function (tzn. przez szablon) [2].

    Jej argumenty, tzn. begin i end, to iteratory wektora danych konkretnego datasetu, a nie samego wektora datasetów.
    Na ich podstawie wyliczamy metrykę, która służy nam do sortowania (tu akurat średnia).
    
    Nie chcemy wykonywać kosztownego obliczania metryki przy każdym porównaniu, więc metoda sortująca powinna być napisana tak, 
    żeby najpierw obliczyć jej wartość dla każdego datasetu i ją zcache'ować, a następnie wykorzystać te obliczone wartości do sortowania.



    
    [1] Z mapą jest tricky, bo nawet ta posortowana nie ma random access iterator (więc sort nie zadziała) i trzeba tworzyć nową z inaczej zdefiniowaną nierównością
    (da się ją podać do konstruktora). Wtedy utrzymuje tę narzuconą kolejność przy dodawaniu elementów, ale nie pozwala jej zmienić bez utworzenia nowej.

    [2] Unikamy przejścia przez v-table z std::function.
*/
