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

#include <iostream>

#include "Querying.h"

int main() {

    std::vector<int> data = {5, 10, 15, 20, 25, 30, 50};

    Querying::CompositeQuery queries(5 /*pre-alocation size hint*/);
    queries.addQuery(new Querying::Simple::GreaterThan(10));
    queries.addQuery(new Querying::Simple::LessThan(30));


    std::cout << "Oryginał (10 < x < 30):\n";
    std::vector<int> result1 = Querying::filter(data, queries);

    std::cout << "Oczekujemy 15, 20 i 25.\n";
    for(auto const& r : result1)
        std::cout << r << "\n";


    std::cout << "Kopia ((10 < x < 30) && (x & 1) == 0 :)):\n";
    Querying::CompositeQuery queries_copy = queries;
    queries_copy.addQuery(new Querying::Simple::IsEven());

    std::vector<int> result2 = Querying::filter(data, queries_copy);

    std::cout << "Oczekujemy 20.\n";
    for(auto const& r : result2)
        std::cout << r << "\n";


    std::cout << "Powtórka oryginału, cross-check:\n";
    std::vector<int> result3 = Querying::filter(data, queries);

    std::cout << "Oczekujemy 15, 20 i 25.\n";
    for(auto const& r : result3)
        std::cout << r << "\n";    

        

    // Nie jestem złośliwy, ale poćwiczmy też zagnieżdzenia :)

    std::cout << "x != 20:\n";
    Querying::CompositeQuery not_query;
    not_query.addQuery(new Querying::Simple::NotQuery(new Querying::Simple::EqualTo(20)));

    std::vector<int> not_result = Querying::filter(data, not_query);

    std::cout << "Oczekujemy 5, 10, 15, 25, 30, 50.\n";
    for(auto const& r : not_result)
        std::cout << r << "\n";    


    return 0;
}

/** 
    Wróćmy do nieocenianych motywów. :)

    Tym razem bez buildera, bo trudność jest w innym miejscu.


    CompositeQuery przejmuje własność nad zapytaniami. 
    Podobnie, NotQuery przejmuje własnośc nad swoim wewnętrznym Query.
    (boję się jeszcze używać unique_ptr w zadaniach :))

    Hint w konstruktorze pozwala zarezerwować miejsce na przekazaną ilość elementów.
    Powinien mieć jakąś rozsądną wartość domyślną, np. 10.

    Każde proste query działa na pojedynczym integerze. 
    Żeby był przepuszczony przez CompositeQuery, wszystkie jego składowe muszą zwrócić true.

    Puste CompositeQuery przepuszcza wszystkie elementy.

    Dla czytelności, proste query mają swój własny, zagnieżdżony, namespace.
*/