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

    auto expr1 = ExpressionParser::parseRPN ( "2 3 + 4 *" ); // (2 + 3) * 4
    std::cout << "expr1->eval({}) = (2 + 3) * 4 = 20? " << expr1->eval({}) << std::endl;
    std::cout << "expr1->eval() = (2 + 3) * 4 = 20? " << expr1->eval() << std::endl;


    auto expr2 = ExpressionParser::parseRPN ( "x 2 + x *" ); // (x + 2) * x
    Expression::Context ctx1 { {"x", 3} };
    Expression::Context ctx2 { {"x", 5} };

    std::cout << "expr2->eval({{x, 3}}) = (3 + 2) * 3 = 15? " << expr2->eval(ctx1) << std::endl;
    std::cout << "expr2->eval({{x, 5}}) = (5 + 2) * 5 = 35? " << expr2->eval(ctx2) << std::endl;

    std::cout << "(*expr2)[0].eval({{x, 3}}) = (3 + 2) = 5? " << (*expr2)[0].eval({{"x", 3}}) << std::endl;
    std::cout << "(*expr2)[1].eval({{x, 3}}) = 3? " << (*expr2)[1].eval({{"x", 3}}) << std::endl;
    

    auto expr3 = ExpressionParser::parseRPN ( "zmienna_1 2 + zmienna_2 *" ); // (zmienna_1 + 2) * zmienna_2
    std::cout << "expr3->eval({{zmienna_1, 3}, {zmienna_2, 5}}) = (3 + 2) * 5 = 25? " 
                << expr3->eval({{"zmienna_1", 3}, {"zmienna_2", 5}}) << std::endl;


    auto expr4 = ExpressionParser::parseRPN ( "5 1 2 + 4 * + 3 -" ); // 5 + ((1 + 2) * 4) - 3
    std::cout << "expr4->eval() = 5 + ((1 + 2) * 4) - 3 = 14? " << expr4->eval() << std::endl;


    std::vector<std::unique_ptr<Expression>> batch;
    batch.push_back(ExpressionParser::parseRPN("1 1.5 +"));
    batch.push_back(ExpressionParser::parseRPN("1 2.5 +"));
    batch.push_back(ExpressionParser::parseRPN("1 3.5 +"));

    for(const auto& e : batch) 
        std::cout << e->eval() << std::endl;
}

/** 

Ponieważ z projektem możemy sobie pozwolić na więcej :), napiszemy uproszczoną wersję parsera wyrażeń,
którą delikatnie zoptymalizujemy na zajęciach. Ograniczymy się wyłącznie do czterech podstawowych działań, tzn. dodawania, odejmowania, mnożenia i dzielenia,
ale pozwolimy też na używanie zmiennych.


Czyli nasze wyrażenia będą, mniej więcej, postaci:

    (2 + 5) * 3 * x - y / 2

itd., gdzie na życzenie możemy podstawić konkretne wartości pod x i y, czy dowolną inną zmienną.

A więc możemy takim parserem obsłużyć bardzo wiele, przynajmniej jeśli chodzi o podstawowe wyrażenia matematyczne.
I wcale nie jest to dalekie od tego, co robią kompilatory, interpretery i biblioteki do obliczeń symbolicznych.
Wręcz przeciwnie, główne idee są takie same. :)


Jak się pewnie domyślacie, ta nasza "naturalna" notacja jest mało kompatybilna z komputerami czy przyjazna do procesowania,
wiec ułatwimy sobie życie i zaczniemy od razu od odpowiedniejszej wersji - notacji postfiksowej.
Nota bene, RPN od Reverse Polish Notation, bo pan który ją wprowadził nazywał się Łukasiewicz. 

W tej notacji, zapisujemy najpierw wszystkie operandy, a po nich działanie. Jednocześnie, uwzględnia ona już priorytet operatorów i ewentualne nawiasy.
Na potrzeby zadania, założymy, że input jest już w formie RPN, bo konwersja z wersji naturalnej na RPN jest dość nietrywialna. 
(od razu mówię, że nie będziemy tego robić na zajęciach :D)


Zatem, w notacji RPN, mamy:

1 + 2 -> 1 2 +
1 + 2 + 3 -> 1 2 + 3 +
(1 + 2) + 3 -> 1 2 + 3 +

ale:

1 + (2 + 3) -> 1 2 3 + +

bo uwzględniamy nawias.

Podobnie z każdym innym działaniem, które wchodzi w miejsce +, albo zmiennymi, które wchodzą w miejsce literałów.
Na przykład:

1 + 2 * x -> 1 2 x * +

gdzie kolejność jest zamieniona, bo uwzględniamy priorytet mnożenia nad dodawaniem.



Tyle wstępu, przejdźmy do implementacji. 

Potrzebujemy - przede wszystkim - wewnętrznej struktury, która będzie w stanie reprezentować dowolne wyrażenie.
Naturalnym wyborem jest drzewo, bo zmienne i stałe możemy zaimplementować jako nody bez dzieci, a operacje binarne 
jako nody z dwójką (lewym i prawym), które mogą być albo stałą/zmienną, albo kolejną operacją binarną.
Możemy też wtedy bardzo łatwo wyznaczyć wartość wyrażenia, wyznaczając wartości kolejnych gałęzi.

Takie drzewa nazywają się AST, od Abstract Syntax Tree (https://en.wikipedia.org/wiki/Abstract_syntax_tree), i używa się ich nie tylko do wyrażeń matematycznych,
ale też do reprezentowania struktury całego kodu w czasie kompilacji (na wiki jest zresztą fajny przykład z obrazkiem).


Nasze drzewo będzie oparte o nody, ale same nody będą reprezentować różne rzeczy, więc potrzebujemy elastyczności.
Jednocześnie, drzewa (przynajmniej klasyczne implementacje :)) są z natury nieciągłe w pamięci, więc nie będziemy kombinować i użyjemy zwykłego dziedziczenia. 


Podstawową komórką będzie wyrażenie (Expression), które implementuje następujący interfejs:

    1) operator [], który zwraca jego dzieci - zakładamy, że 0 to lewe, a 1 to prawe.
    2) metodę eval, która wyznacza i zwraca wartość wyrażenia
        * metoda ta musi przyjmować tzw. context, by umożliwić nam korzystanie ze zmiennych
        * context to nic innego jak mapowanie (nazwa zmiennej) -> (wartość)


Komórki będą mieć trzy warianty (a właściwie to 6 :)): 

    1) Constant, który tylko zwraca swoją wartość i nie ma dzieci (wypadałoby jakoś zabezpieczyć ścieżkę operatora [] :)).
    2) Variable, który zna swoją nazwę, a zwracaną wartość bierze z kontekstu i nie ma dzieci.
    3) BinaryOperator (dla każdej operacji), który ma dwójkę dzieci i wykonuje na nich działanie


Zauważcie, że sama struktura wykonuje za nas praktycznie całą pracę przy wyznaczaniu wartości, bo wystarczy wywołać eval z kontekstem na korzeniu,
żeby obliczyć całość.


Jedynym, co nam pozostało, jest zamiana wejściowego stringa w drzewo. Pierwszym krokiem tego etapu jest tzw. tokenizacja, czyli podział wejścia
na najmniejsze możliwe elementy (tokeny), które mają już konkretne przełożenie na elementy naszego drzewa.

Dodamy tu jeszcze jedno uproszczenie - założymy, że wszystkie tokeny będą zawsze przedzielone jedną spacją.
Dzięki temu, unikniemy konieczności wstępnego obrabiania całego stringa, które jest straszną męczarnią i nic nie wnosi. :)

Zatem, na potrzeby zadania, zakładamy dwie rzeczy:

    1) Wejście będzie zawsze ładnie i poprawnie sformatowane, np. "1 2 + 3 + 4 * 5 *"
        * każdy token będzie przedzielony dokładnie jedną spacją

    2) Każda stała/zmienna liczbowa będzie typu double.
        * możecie sobie wyobrazić jak bolesna jest tokenizacja na poziomie języka programowania :)


Samo parsowanie formalnie dzieli się na dwa etapy. Tokenizacją zajmuje się tzw. Lexer, który identyfikuje elementy języka/wyrażenia. 
Jego zadaniem jest:

    1) Podzielić cały input na tokeny - w naszym wypadku, będzie to prosty podział na spacjach.
        * oczywiście, normalnie jest to bardziej złożony proces :)
        * zakładamy brak problemów, podwójnych spacji, itd.
    2) Zidentyfikować i oznaczyć je - u nas będą to liczba, identyfikator i operator
        * normalnie moglibyśmy mieć jeszcze podział na słowa kluczowe, itd.


Czyli inputem lexera jest string z wyrażeniem, a outputem tablica tokenów.
Tokenem w naszym przypadku jest coś, co zna typ (enum) i przechowuje (jako string) swoją zawartość. 

Jedynym problemem, przy naszych założeniach, jest rozróżnienie literałów liczbowych i nazw zmiennych.

Można to zrobić na kilka sposobów, ale standardem jest testowanie czy fragment stringa, odpowiadający tokenowi, da się zamienić na double.
Jeżeli tak, to uznajemy go za literał liczbowy. Najlepszym (chyba :)) rozwiązaniem jest użycie std::strtod i sprawdzenie błędów konwersji.
Surową tablicę charów można wyciągnąć ze stringa robiąc s.c_str(). 

Żeby sprawdzić czy nazwa zmiennej jest prawidłowa, musimy sprawdzić czy pierwszy znak jest alfabetycznym lub podkreśleniem _,
oraz czy reszta znaków jest alfanumeryczna lub podkreśleniem _. Innymi słowy, czy nazwa nie zaczyna się od cyfry i czy zawiera
wyłącznie znaki alfanumeryczne lub podkreślenia. Tu się przyda std::isalpha i std::isalnum.


Mając już listę tokenów, możemy przejśc do głównej części parsowania, która buduje na ich podstawie drzewo.
Zapisanie wyrażenia w RPN bardzo nam pomoże, bo jak (po intensywnym wpatrywaniu) zauważycie, całe wyrażenie przyjmuje formę stosu.
W konsekwencji, my też możemy użyć stosu jako tymczasowej pomocy w budowaniu drzewa (w C++ istnieje już implementacja, std::stack).

Reguły są bardzo proste:

    1) Jeżeli token jest literałem liczbowym lub zmienną, wkładamy go na stos
    2) Jeżeli jest operatorem binarnym, zastępuje dwa ostatnie elementy ich działaniem
        * zakładamy, że elementy są na stosie w kolejności:
            [ .... ][lewy][prawy]


Jeżeli przejdziemy tak przez wszystkie tokeny, to zbudowaliśmy pełne wyrażenie, a stos zawiera tylko jeden element - korzeń drzewa.
To jest też dobre miejsce na crosscheck, bo jeżeli nie zawiera jednego elementu, to coś poszło nie tak.
*/