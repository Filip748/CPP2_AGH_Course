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
#include <memory>

#include "Expression.h"
#include "ExpressionParser.h"

int main() {

    auto expr1 = ExpressionParser::parseRPN("2 3 + 4 *");
    std::cout << "expr1 = (2 + 3) * 4 -> " << expr1->toString() << std::endl;

    auto expr2 = ExpressionParser::parseRPN("x 2 + x *");
    std::cout << "expr2 = (x + 2) * x -> " << expr2->toString() << std::endl;

    auto expr3 = ExpressionParser::parseRPN("5 1 2 + 4 * + 3 -");
    std::cout << "expr3 = 5 + ((1 + 2) * 4) - 3 -> " << expr3->toString() << std::endl;



    auto expr4 = ExpressionParser::parseRPN("x ~");
    std::cout << "expr4 = -x -> " << expr4->toString() << std::endl;
    std::cout << "expr4(x = 3) = -3? " << expr4->eval({ {"x", 3} }) << std::endl;

    auto expr5 = ExpressionParser::parseRPN("x 2 + ~");
    std::cout << "expr5 = -(x + 2) -> " << expr5->toString() << std::endl;
    std::cout << "expr5(x = 3) = -(3 + 2) = -5? " << expr5->eval({ {"x", 3} }) << std::endl;



    auto expr6 = ExpressionParser::parseRPN("2 5 + 3 +"); // (2 + 5) + 3
    auto simplified6 = expr6->simplify();

    std::cout << "((2 + 5) + 3) -> (10?) : " << simplified6->toString() << std::endl;


    auto expr7 = ExpressionParser::parseRPN("2 5 + 3 + x *"); // ((2 + 5) + 3) * x
    auto simplified7 = expr7->simplify();

    std::cout << "((2 + 5) + 3) * x ->  (10 * x?) : " << simplified7->toString() << std::endl;




    auto expr8 = ExpressionParser::parseRPN("x 0 +");
    std::cout << "x + 0 -> " << expr8->simplify()->toString() << " (x?)" << std::endl;

    auto expr9 = ExpressionParser::parseRPN("0 x +");
    std::cout << "0 + x -> " << expr9->simplify()->toString() << " (x?)" << std::endl;

    auto expr10 = ExpressionParser::parseRPN("x 0 *");
    std::cout << "x * 0 -> " << expr10->simplify()->toString() << " (0?)" << std::endl;

    auto expr11 = ExpressionParser::parseRPN("x 1 *");
    std::cout << "x * 1 -> " << expr11->simplify()->toString() << " (x?)" << std::endl;

    auto expr12 = ExpressionParser::parseRPN("1 x *");
    std::cout << "1 * x -> " << expr12->simplify()->toString() << " (x?)" << std::endl;

    auto expr13 = ExpressionParser::parseRPN("x 1 /");
    std::cout << "x / 1 -> " << expr13->simplify()->toString() << " (x?)" << std::endl;

    auto expr14 = ExpressionParser::parseRPN("0 x -");
    std::cout << "0 - x -> " << expr14->simplify()->toString() << " (-x?)" << std::endl;




    auto expr15 = ExpressionParser::parseRPN("2 3 + 0 + x * 1 *"); // ((2+3)+0)*x*1
    auto simplified15 = expr15->simplify();

    std::cout << "((2 + 3) + 0) * x * 1 ->  (5 * x?) : " << simplified15->toString() << std::endl;
}

/** 
Skoro najtrudniejszą cześć mamy za sobą, to spróbujemy całość trochę rozbudować,
a przy okazji poćwiczymy chodzenie po drzewie. :)

Dodamy:

    1) Metodę konwertującą drzewo (lub jego część) na string, który będzie nam łatwiej zinterpretować.
        * Nie kombinujemy w żaden sposób przy wypisywaniu, po prostu otaczamy każde binarne działanie nawiasami.

    2) Unarny operator negacji, zwany potocznie minusem. :)
        * Tutaj pojawia się nam mały problem, bo musimy w jakiś sposób decydować czy '-' oznacza odejmowanie dwóch wartości, czy operator negacji.
        * I generalnie RPN ma z tym problem, bo o ile da się zrobić to tak, żeby działało w większości rozsądnych przypadków (decydując po wielkości stosu),
        o tyle i tak będą edge casy gdzie taki parser się wyłoży. Na przykład:
            5 - (-3) -> 5 3 - - -> -2 zamiast 8

        Dlatego, takie problemy rozwiązuje się na etapie tokenizacji, wprowadzając "wirtualne" operatory, tzn. "unary minus" z symbolem innym niż -,
        co pozwala go zidentyfikować przy tworzeniu wyrażenia. Możemy to zrobić, bo obrabiając wyrażenie w "naturalnej" formie mamy dodatkowy kontekst,
        który tracimy przechodząc na RPN (np. nawiasy, położenie minusa w wyrażeniu). Innym rozwiązaniem jest też zastąpienie wszystkich wystąpień
        "-x" na "0 - x".

        Przy wybieraniu symbolu mamy jeszcze ten potencjalny problem, że dopuszczamy zmienne. Także jednoliterowe, więc literki odpadają.
        Podobnie, zmienne często zawierają _, więc odpada. Jednym z naturalnych kandydatów (którego użyłem w zadaniu) jest tylda, ~. I nawet do minusa podobna. :D


        * To, co jest istotne, to że traktujemy ten operator jako node z jednym dzieckiem i nie próbujemy od razu podmienić wartości stałych na ujemne.

        * Przy wypisywaniu, tak samo bierzemy go w nawias, tzn.
            * (-x)
            * (-(x + 2))

    3) Metodę simplify, która skraca dające się uprościć gałęzie. Na przykład:
        
        * ((2 + 5) + 3) * x -> 10 * x, czyli wykonujemy wszystkie działania, które można wykonać bez wstawiania wartości pod zmienną
        * to jest tzw. constant folding

    Dodatkowo, implementujemy uproszczenia algebraiczne, gdzie eliminujemy zera i jedynki (także w obecności zmiennych):

        * Operujemy na wyrażeniach, które ktoś gdzieś wpisał ręcznie, więc jeśli pojawia się 1 to mamy pewność, że zawsze będzie to dokładna bitowa reprezentacja 1ki (nawet jeśli mamy double).
        Podobnie 0, tzn. możemy bezpiecznie założyć że 0 to 0x0000000000000000.

    zera:

        x + 0 -> x 
        0 + x -> x

        x - 0 -> x
        0 - x -> -x

        x * 0 -> 0
        0 * x -> 0

    jedynki:

        x * 1 -> x 
        1 * x -> x

        x / 1 -> x
        
    dla dowolnego x, nie tylko zmiennej.


    x / x sobie odpuścimy, bo jak wiemy z matematyki jest tricky. :)
    Trzeba by wtedy sprawdzać całe gałęzie i upewniać się, że na pewno możemy uprościć.
*/