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
#include <algorithm>
#include <string>

#include "ComparatorBuilder.h" // W założeniu załącza też bibliotekę komparatorów

int main() {


    // Testy poszczególnych komparatorów

    {
        CaseInsensitiveLess cmp;

        std::cout << cmp("mario", "Zelda") << " (oczekiwane: 1)\n";
        std::cout << cmp("Zelda", "mario") << " (oczekiwane: 0)\n";
        std::cout << cmp("Mario", "mario") << " (oczekiwane: 0)\n";
    }

    {
        CaseSensitiveLess cmp;

        std::cout << cmp("Mario", "mario") << " (oczekiwane: 1)\n";
        std::cout << cmp("mario", "Mario") << " (oczekiwane: 0)\n";
    }

    {
        LengthLess cmp;

        std::cout << cmp("Link", "Zelda") << " (oczekiwane: 1)\n";
        std::cout << cmp("Zelda", "Link") << " (oczekiwane: 0)\n";
        std::cout << cmp("Link", "link") << " (oczekiwane: 0)\n";
    }

    // Test komparatora z dekoratorem (to nie jest unikalny koncept pythonowy :P)

    {
        CaseInsensitiveLess base;
        IgnorePrefixComparator cmp(base, "The ");

        std::cout << cmp("The Witcher", "Zelda") << " (oczekiwane: 1)\n";
        std::cout << cmp("Zelda", "The Witcher") << " (oczekiwane: 0)\n";
        std::cout << cmp("The Witcher", "the witcher") << " (oczekiwane: 0)\n";
    }

    
    // Test złożenia komparatorów

    {
        CaseInsensitiveLess ci;
        CaseSensitiveLess cs;
        LengthLess len;

        CompositeComparator cmp;
        cmp.add(&ci);
        cmp.add(&cs);
        cmp.add(&len);

        std::cout << cmp("mario", "Mario") << " (oczekiwane: 0)\n";
        std::cout << cmp("Mario", "mario") << " (oczekiwane: 1)\n";
        std::cout << cmp("Link", "link") << " (oczekiwane: 1)\n";
    }


    // I w końcu builder
    
    {
        std::vector<std::string> names = {
            "Zelda", "mario", "Link", "kirby", "Bowser",
            "link", "Mario", "The Witcher", "the witcher" // Poprosiłem GPT o listę imion z jednego uniwersum, które przetestują takie porównania (:
        };


        // To jest najważniejsza część tego zadania, reszta to tylko testy, żeby nie pisać wszystkiego na raz

        ComparatorBuilder builder;  // Musimy tak, bo używamy komparatorów, które przechowuje - czyli instancja musi istnieć, żeby cmp działało
        auto cmp = builder
            .caseInsensitive()
            .thenCaseSensitive()
            .thenByLength()
            .ignorePrefix("The ")   // działa na poprzedzający komparator, czyli "by length"
            .build();

        /////////////////////////////////////////////////////////


        std::sort(names.begin(), names.end(), cmp);

        std::cout << "\nPosortowane:\n";
        for (const auto& n : names)
            std::cout << n << "\n";


        std::cout << "\nOczekiwana kolejność:\n";
        std::cout << "Bowser\n";
        std::cout << "kirby\n";
        std::cout << "Link\n";
        std::cout << "link\n";
        std::cout << "Mario\n";
        std::cout << "mario\n";
        std::cout << "The Witcher\n";
        std::cout << "the witcher\n";
        std::cout << "Zelda\n";
    }

    return 0;
}

/** 

Przy okazji kolejnego zadania z operatorów :), zróbmy coś co się Wam przyda, niezależnie od tego co i w jakim języku będziecie pisać.
Nazywa się to builder pattern i jest jednym z (dość niewielu) wzorców, których rzeczywiście używa się bardzo często,
bo można tak rozwiązać wiele problemów w elegancki sposób. Zadanie jest na podstawie tego, co było zasugerowane na teamsach,
ale używa się go wszędzie tam, gdzie składamy coś z wielu klocków (configi, duże obiekty z opcjonalnymi polami, UI, zapytania (querries), JSONy, itd.)
i chcemy uniknąć ogromnych konstruktorów / nieczytelnego kodu.

Idea jest taka - chcemy sortować stringi i potrzebujemy do tego funkcji która je porównuje i narzuca jakąś hierarchię,
ale zamiast za każdym razem pisać nową albo ręcznie wykonywać kilka sortowań po sobie, chcemy mieć łatwy sposób na zdefiniowanie funkcji,
łączącej kilka różnych kryteriów sortowania na raz. Zrealizujemy to przy użyciu rodziny funktorów i klasy budowniczego, która będzie je kombinować.
Przy czym, co ważne, w taki sposób, że będzie to rzeczywiście sortowanie na podstawie kilku kryteriów - czyli kolejny warunek ma znaczenie dopiero, 
gdy dwa elementy są równe względem wszystkich poprzednich.

Jednocześnie, zrobimy to tak, że mimo używania wskaźników na klasę bazową, wszystko będzie siedzieć albo na stosie, albo w ciągłym bloku pamięci na stercie.
Główną "wadą" tego rozwiązania jest to, że obiekt buildera musi istnieć, żeby powstały komparator mógł istnieć i działać (bo używamy wskaźników do jego pól).
Trzeba też uważać na implementację, gdybyśmy z jednego buildera chcieli tworzyć kilka komparatorów i używać ich jednocześnie, bo realokacja pamięci używanej przez std::vector
zepsuje wszystkie wskaźniki/referencje do jego elementów. 

Jak już wiemy :), naturalną poprawką byłoby użycie unii/std::variant i przechowywanie w CompositeComparator kopii wszystkich komparatorów (static nie zadziała, bo mamy wrapper).
Wtedy moglibyśmy też zrobić bardziej eleganckie:

    auto cmp = ComparatorBuilder()
        .caseInsensitive()
        .thenCaseSensitive()
        .thenByLength()
        .ignorePrefix("The ")
        .build();

Ale dzisiaj zrobimy wersję przez dziedziczenie, żeby je ćwiczyć. Chociaż warto sobie, dla praktyki, spróować przerobić całość na unie.


Potrzebujemy:

1. Dwóch funkcji pomocniczych (nie byłoby złym pomysłem trzymanie wszystkiego w jakimś namespace :)):

    * funkcji, która przyjmuje string i zwraca nowy, gdzie wszystkie litery są małe (można użyć np. std::tolower, które działa na unsigned char)
    * funkcji, która przyjmuje string i zwraca nowy, który nie zawiera podanego prefixu (rfind, starts_with, substr, etc.)

2. Rodziny komparatorów, które dziedziczą z tej samej klasy bazowej StringComparator (dzięki temu, możemy je włożyć w jeden kontener).

    * wszystkie to funktory, które potrafią przyjąć dwa stringi i zwrócić bool, mówiący czy string_lewy < string_prawy, według zadanego kryterium

    * potrzebujemy trzech "prostych" (Less na końcu oznacza, że implementujemy wersję "<"):

        * CaseInsensitiveLess - który sprawdza nierówność (leksykograficznie, string implementuje <) bez brania pod uwagę tego, czy znak jest wielki czy mały
        * CaseSensitiveLess - jak wyżej, ale uwzględniając wielkość znaków
        * LengthLess - który porównuje długości

    
    * jednego "złożonego", IgnorePrefixComparator, który pozwala na zignorowanie prefixów w czasie używania któregoś z "prostych" komparatorów

        * nie da się tego zrealizować inaczej niż jako wrapper/dekorator na prosty komparator
            
    
    * w końcu, potrzebujemy komparatora, który reprezentuje kolekcję innych komparatorów - CompositeComparator, który powinien:

        * przechowywać kolekcję (jako jeden kontener) komparatorów i pozwalać na dodawanie nowych
        * samemu być funktorem, który przyjmuje dwa stringi i wykonuje na nich wszystkie porównania, w kolejności dodania do kontenera
            * pamiętamy, że zależy nam na rzeczywistym sortowaniu z wieloma kryteriami, a nie tylko czymś, co efektywnie byłoby sortowaniem po ostatnim przekazanym warunku
            * czyli decyzja musi być oparta na pierwszym komparatorze, dla którego string_lewy != string_prawy i musimy testować zarówno string_lewy < string_prawy, jak i string_lewy > string_prawy


3. Klasy ComparatorBuilder, która składa CompositeComparator na podstawie naszych decyzji, wyrażonych wywołaniem metod.
Kluczowe uwagi:

    * CompositeComparator powstaje dopiero, gdy zawołamy build(). Inaczej, nie bylibyśmy w stanie utworzyć tego łańcuszka wywołań.
        * Czyli builder musi też do tego czasu samemu przechowywać wszystkie dodane komparatory.

    * Powiedzieliśmy, że chcemy, żeby wszystko było na stosie albo w ciągłym bloku pamięci, czyli musimy mieć jakieś instancje komparatorów wewnątrz buildera, do których będziemy się odnosić.
        * Nasze proste komparatory są efektywnie bezstanowe, czyli wystarczy nam po jednej instancji każdego
        * Z ignorowaniem prefixów jest trudniej, bo to wrapper, który zależy od przekazanego komparatora
            * potrzebujemy kontenera na tyle instancji, ile wymaga nasz złożony komparator 
            (ale dzięki temu są w jednym miejscu pamięci, a nie rozrzucone jak gdybyśmy używali do każdego new)

    * Wywołanie build() składa i zwraca CompositeComparator, który jednak używa instancji potrzebnych komparatorów przechowywanych w builderze

    * Wrapper ignorujący prefix odnosi się do poprzedzającego go prostego komparatora
        * Czyli opakowuje ostatnio dodany komparator

*/