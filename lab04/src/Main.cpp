/*
Najważniejsze informacje:
- nie korzystamy z internetu, notatek, kodu sąsiada, etc.
- nie można w żaden sposób modyfikować maina (chyba, że zawiera tylko komentarz z prośbą o jego wypełnienie :))
- CMake musi mieć flagi generujące warningi, tzn. minimum -Wall -Wextra -Wpedantic
- przypominam o istnieniu valgrinda
- będę się czepiać const correctness

- na upela wrzucamy archiwum z plikami źródłowymi oraz CMakeLists.txt, bez katalogu build
    * w prostych programach proszę się trzymać podziału na include i src, 
    * jeśli pojawi nam się większy projekt, podział na podkatalogi z użyciem include_directory w CMake jest jak najbardziej ok
    * tar -czvf nazwa.tar.gz zad/ :)
- proszę czytać wstępy i feedback
*/

#include "TileMap.h"
#include "TileMapView.h"

#include <iostream>

int main() {

    TileMap tile_map(4, 4);

    /*
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        Nie chcę w żaden sposób narzucać wewnętrznej struktury danych,
        więc poniżej są dwa miejsca do wstawienia - wypełnianie danych w pętli poniżej 
        i modyfikacja kawałek dalej (oznaczone !).
        
        Pusta mapa == całkowicie niepoprawne wykonanie. :)
        !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    */
    for (int row = 0; row < 4; ++row) {
        for (int column = 0; column < 4; ++column) {

            // Proszę użyć tych danych (reszta dowolna), żeby każda kratka była unikalna.
            bool near_river = (row % 2 == 0);
            bool sea_access = (row % 2 == 1);
            bool has_forest = (column % 2 == 0);
            bool has_road = (column % 2 == 1);

            tile_map(row, column) =  Tile{Terrain::FIELDS, near_river, sea_access, has_forest, has_road, Building::NONE};
            // !!!!!!!!!!!!!!!!!!!!! Proszę wypełnić !!!!!!!!!!!!!!!!!!!!!!!!!!
        }
    }

    std::cout << "Test implementacji TileMap" << std::endl;
    std::cout << "Operatory dostępu i mapowanie (powinno być true): "
        << ((tile_map[2] == tile_map(0, 2)) || (tile_map[2] == tile_map(2, 0))) << std::endl; // Zależne od implementacji, tzn. row/column-major

    const TileMap const_tile_map = tile_map;
    std::cout << "Operatory dostępu (const) (powinno być true): " 
        << ((const_tile_map[2] == const_tile_map(0, 2)) || (const_tile_map[2] == const_tile_map(2, 0))) << std::endl; // Zależne od implementacji, tzn. row/column-major



    TileMapView entire_map_view = tile_map.createView();
    TileMapView map_view = tile_map.createView(1, 2, 2, 3);

    std::cout << "Test implementacji TileMapView" << std::endl;
    std::cout << "Test zakresu i offsetów (powinny być true): " 
        << (tile_map(1, 2) == map_view(0, 0)) << ", " 
        << (tile_map(2, 3) == map_view(1, 1)) << ", " 
        << (tile_map(0, 0) == entire_map_view(0, 0))  << ", " 
        << (tile_map(3, 3) == entire_map_view(3, 3)) 
        << std::endl;

    std::cout << "Operatory dostępu i mapowanie (powinno być true): "
        << ((map_view[2] == map_view(0, 2)) || (map_view[2] == map_view(2, 0))) << std::endl; // Zależne od implementacji, tzn. row/column-major

    const TileMapView const_map_view = map_view;
    std::cout << "Operatory dostępu (const) (powinno być true): " 
        << ((const_map_view[2] == const_map_view(0, 2)) || (const_map_view[2] == const_map_view(2, 0))) << std::endl; // Zależne od implementacji, tzn. row/column-major


    std::cout << "Przed zmianą mapy (powinno być true): " << (tile_map(1, 2) == map_view(0, 0)) << std::endl;

    tile_map(1, 2).hasRoad = !tile_map(1,2).hasRoad; // !!!!!!!!!!!!!!!!!!!!! Proszę coś zmienić (np. zaprzeczyć flagę) !!!!!!!!!!!!!!!!!!!!!!!!!!

    std::cout << "Po zmianie (powinno być true): " << (tile_map(1, 2) == map_view(0, 0)) << std::endl;



    std::cout << "Test granic iteratora:" << std::endl;
    auto first = map_view.begin();
    auto last = map_view.end();
    --last;

    std::cout << "Początek: " << (*first == map_view(0, 0)) << std::endl;
    std::cout << "Koniec: " << (*last == map_view(1, 1)) << std::endl;
     

    std::cout << "Wypisanie z użyciem iteratora" << std::endl;
    int count = 0;
    for(auto tile_it = map_view.begin(); tile_it != map_view.end(); ++tile_it) {
        std::cout << *tile_it << std::endl;
        ++count;
    }
    std::cout << "Ilość wypisanych elementów (powinny być 4): " << count << std::endl;


    return 0;
}

/** 

Żeby trochę wyrównać znajomośc terminologii :) i utrwalić to, o czym mówiliśmy przez ostatnie dwa zadania, 
zaimplementujemy prostą mapę 2D, widok (view) i iterator. A przy okazji przećwiczymy kompozycję danych i wykorzystanie enumeracji.

Uwaga - w mainie są dwa miejsca do wypełnienia (oznaczone !).


Proszę zaimplementować:

1) Klasę TileMap, która:
    
    * Przechowuje informacje o mapie 2D, podzielonej na kwadratowe kratki:

        ** Klasa powinna udostępniać dostęp liniowy, przy użyciu operatora [],
        ** oraz dostęp do konkretnej kratki, przez operator ().
        ** Dostęp powinien być możliwy zarówno przez TileMap, jak i const TileMap.

    * Przyjmijmy, że każda kratka zawiera informacje:
        
        ** o typie terenu (fields, hills, mountains), koszcie ruchu (5, 15, brak), możliwości przejścia (tak, tak, nie)
        ** czy teren jest:
            *** w pobliżu rzeki
            *** ma dostęp do morza
            *** jest zalesiony
            *** ma drogę
        ** czy kratka (tu może być prawdziwa tylko jedna na raz):
            *** jest miastem
            *** ma farmę
            *** ma tartak (uprzedzając, lumbermill :))
            *** ma kopalnię

        ** sposób przechowywania tych danych jest dowolny, stąd miejsca do wypełnienia w mainie

    * Udostępnia metodę createView, która:
        
        ** wywołana bez argumentów, zwraca widok całej mapy
        ** pozwala na podanie zakresu (min/max x, min/max y), aby utworzyć widok odpowiedniego wycinka mapy
            

2) Klasę TileMapView, która:

    * Widzi zawsze aktualny stan mapy.

        ** Ważne! Widok nie przechowuje kopii mapy, tylko widzi jej aktualny stan.
        ** Jeżeli widok nie obejmuje całej mapy, to używa offsetów tak, by jego wycinek był indeksowany od (0, 0)
            ** obydwie przekazane wartości (min/max) są inkluzywne, tzn. min = 2, max = 4, oznacza indeksy 2, 3, 4
            ** Na przykład, dla widoku (3, 6, ...):
                mapa    0 1 2 3 4 5 6 7
                widok   - - - 0 1 2 3 -

    * Udostępnia przez swój interfejs dostęp do operatorów [] i () mapy.

    * Implementuje metody begin() i end(), które zwracają Iterator ustawiony na początek
    oraz na indeks za ostatnim właściwym elementem. ^



3) Klasę TileMapView::Iterator (zazwyczaj iteratory definiuje się w scope klasy, chyba że są w jakiś sposób generyczne), 
która:

    * Pozwala tylko na liniową iterację.

    * Widzi aktualny stan widoku.

        ** Widoku, nie mapy.
        ** W szczególności, jeśli widok obejmuje tylko wycinek mapy, iterator również porusza się po wycinku.
        ** Ponownie, nie jest to kopia.

    * Implementuje operatory:

        * == i !=, porównujący aktualną pozycję, na którą wskazuje iterator,
        * ++, zwiększający pozycję o 1 ^^,
        * --, zmniejszający o 1,
        * operator dereferencji, który zwraca kratkę mapy, na którą iterator wskazuje
            ** sygnatura to Typ& operator*()






    ^ Normalnie moglibyśmy chcieć zaimplementować jeszcze ConstIterator, 
    ale byłby identyczny tylko zwracał stały element, więc w zadaniu sobie darujemy.
    Według konwencji (przynajmniej tej w bibliotece standardowej), tak jak begin() i end() 
    zwracają iteraty, tak cbeing() i cend() zwracają stałe iteratory. 

    ^^ W tym wypadku, iterator zajmuje się prostą liniową iteracją, więc wystarcza nam indeks w tablicy.
    W ogólności, iterator jest uogólnieniem mechanizmu iteracji i może być zaimplementowany w dowolny sposób,
    pod warunkiem, że ma jakiś początek i koniec oraz porusza się po kolejnych elementach, aż do tego końca dotrze. 

    Pozwala to na zdefiniowanie sensownego zachowania także w nietrywialnych sytuacjach. Na przykład, kontenery niebędące ciągłe w pamięci
    (tak jak queue, którego użyliśmy w projekcie) mogą nie pozwalać na indeksowanie operatorem [i], ale ciągle udostepniać iterator,
    pozwalający przejść przez wszystkie ich elementy. Oczywiście, funkcjonują też iteratory takie jak ReverseIterator.
*/