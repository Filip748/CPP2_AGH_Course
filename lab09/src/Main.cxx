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
#include <vector>

#include "Iterators.h"

int main() {

    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};


    auto it = Iterators::VectorIterator(vec); // Tu jeszcze nic się nie dzieje.



    std::vector<int> result_1 = Iterators::VectorIterator(vec).collect();
    
    std::cout << "Oczekujemy: 1 2 3 4 5 6 7 8 9 10" << std::endl;
    for(const auto& el : result_1) {
        std::cout << el << " ";
    }
    std::cout << std::endl;




    std::vector<int> result_2 = Iterators::VectorIterator(vec)
        .filter([](int x){return x % 2 == 0;})
        .collect();

    std::cout << "Oczekujemy: 2 4 6 8 10" << std::endl;
    for(const auto& el : result_2) {
        std::cout << el << " ";
    }
    std::cout << std::endl;




    std::vector<int> result_3 = Iterators::VectorIterator(vec)
        .filter([](int x){return x % 2 == 0;})
        .take(3)
        .collect();        

    std::cout << "Oczekujemy: 2 4 6" << std::endl;
    for(const auto& el : result_3) {
        std::cout << el << " ";
    }
    std::cout << std::endl;




    std::vector<int> result_4 = Iterators::VectorIterator(vec)
        .filter([](int x){return x % 2 == 0;})
        .take(3)
        .map([](int x){return x * x;})
        .collect();      

    std::cout << "Oczekujemy: 4 16 36" << std::endl;
    for(const auto& el : result_4) {
        std::cout << el << " ";
    }
    std::cout << std::endl;
}

/** 

To żeby odpocząć od C++, trochę programowania funkcyjnego i cywilizacji (podobieństwo do innych języków czysto przypadkowe) [1].
Szablony są chyba nielegalne, więc zrobimy dla wektora intów, ale uogólnienie jest trywialne (:)).
Jesteśmy też ograniczeni do std::function, które będzie trochę mniej wydajne niż szablony, przez type erasure i wskaźniki [2].


Idea jest podobna do buildera, który pisaliśmy, tylko tym razem budujemy wyrażenie, które będzie transformować wektor danych.
Zrobimy to, oczywiście, w jak najwydajniejszy sposób. :)
Czyli chcemy, żeby ostatnie wyrażenie z maina skompilowało się do czegoś (w miarę) równoważnego:

    std::vector<int> out;
    out.reserve(3);

    for (int x : vec) {
        if (x % 2 == 0) {
            out.push_back(x * x);
            if (out.size() == 3) break;
        }
    }

Co narzuca nam dwa ograniczenia (właściwie jedno):
    * zanim przejdziemy do kolejnego elementu, musimy wykonać wszystkie transformacje na obecnym
    * nie możemy tworzyć pośrednich wektorów, więc musimy zrobić tzw. lazy evaluation i wykonać całość dopiero przy collect()

Innymi słowy, wszystko przed wywołaniem collect() jest ciągle iteratorem, po wywołaniu - dostajemy finalny wektor z wynikiem.


Dopuścimy też pojawienie się kilku filtrów i map, ale założymy, że jeśli ktoś potrzebuje więcej niż 5 każdego,
to powinien się poważnie zastanowić co robi i użyjemy statycznych tablic do ich przechowania. :)
(przypominam, że std::vector z rozmiarem 5, to ciągle dynamiczna alokacja)


Klasa iteratora musi:

    1) Widzieć aktualny stan wektora (unikamy tworzenia kopii)
    2) Przechowywać informacje o wszystkich transformacjach:
        * filtry (int -> bool), maks 5
        * mapy (int -> int), maks 5
        * ile elementów bierzemy maksymalnie (take)

    3) Implementować wszystkie transformacje:
        - filter - decyduje czy dany element przechodzi do wyniku
        - map - transformuje element
        - take - decyduje ile elementów bierzemy (po przejściu wszystkich filtrów) 

    W przypadku filtrów i map, traktujemy je jak osobne transformacje (w szczególności, dla map ważna jest kolejność).
    Dla take wystarczy wziąć minimum ze wszystkich wywołań.

    4) Implementować collect(), które wykonuje wszystkie transformacje i zwraca wynik w postaci wektora.
    Kilka uwag:
        * jeżeli w łańcuchu pojawiło się take, to wiemy ile maksymalnie elementów będzie w wyniku
        * jeżeli nie, też możemy coś założyć na podstawie wektora wejściowego
        * pamiętamy co chcemy osiągnąć i co powinno być wewnęrzną, a co zewnętrzną pętlą (robimy tylko jedną pętlę po wejściowym wektorze :))
        * istnieje jedna optymalna kolejność wykonywania wszystkich transformacji


        

[1]
To be fair, doczekaliśmy się czegoś podobnego w C++20 po wprowadzeniu zakresów (ranges)
i wyglądałoby jakoś tak: 

    std::vector<int> result = vec
        | std::views::filter([](int x){return x % 2 == 0;})
        | std::views::take(3)
        | std::views::transform([](int x){return x * x;})
        | std::ranges::to<std::vector>();


Tak, to jest przeciążony operator bitowego ora... Strzelam, że inspirowany pipe'ami z linuxa.


W innym języku :), wygląda to tak:

    let vec = vec![1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
    println!("{:?}", vec);

    let result: Vec<i32> = vec
        .into_iter()
        .filter(|x| x % 2 == 0)
        .take(3)
        .map(|x| x * x)
        .collect();    
        
    println!("{:?}", result);


[2]
Mamy do wyboru std::function albo szablony, bo lambda rozpada się w czasie kompilacji na funktor z nazwą generowaną przez kompilator.
Tzn. coś w rodzaju:

struct __nazwa_nadana_przez_kompilator {

    // przechwycone wartości, np.
    int cap1;

    __nazwa_nadana_przez_kompilator(int i) : cap1(i) {}

    bool operator() ( int i ) const { return ..... }

};


std::function dodaje jeszcze jeden poziom abstrakcji i wymazuje typ,
tzn. wygląda jakoś tak:

struct Function {

    void* dane_obiektu;

    bool (*call)(void*, int);
    void (*destroy)(void*);
    
    itd.
}

+ generuje tzw. trampoliny, które robią odpowiedni cast, na zasadzie:

bool call_fn(void* obj, int x) {
    return (*static_cast<odpowiedni_typ*>(obj))(x);
}

Long story short, to jest v-table i tzw. runtime polymorphism without inheritance.
Ze wszystkimi konsekwencjami dla wydajności.

Szablony pozwoliłyby nam uniknąć tego kosztu i przekazać lambdę jako funktor.
*/