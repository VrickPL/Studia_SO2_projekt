<h1 align="center">Problem jedzących filozofów</h1>
<h3 align="center">Program napisany na potrzeby projektu z kursu Systemy Operacyjne 2</h3>



## Table of Contents 
- [Uruchomienie projektu](#uruchomienie-projektu)
- [Opis problemu](#opis-problemu)
- [Opis wątków](#opis-watkow)
- [Sekcje krytyczne](#sekcje-krytyczne)



## Uruchomienie projektu
Poniższy zrzut ekranu prezentuje proces kompilacji oraz uruchomienia programu.
Najpierw program jest kompilowany poleceniem: `g++ main.cpp -std=c++17 -o main`, a następnie jest uruchamiany przez polecenie `./main [liczba_filozofow]`.
- Przy uruchomieniu bez argumentu lub z niepoprawną liczbą filozofów (mniej niż 2) program zwraca odpowiedni komunikat błędu.
- Po uruchomieniu z wartością 2, program przechodzi do symulacji.

<p align="center"> 
<img src="https://github.com/user-attachments/assets/6f52e393-74b6-47e6-9815-531af125f1f9">
</p>



## Opis problemu
Problem jedzących filozofów jest klasycznym przykładem zagadnienia synchronizacji w systemach wielowątkowych. Kilku filozofów siedzi przy okrągłym stole i przechodzi cykl: myślenie, próba zdobycia widelców, jedzenie. Aby rozpocząć posiłek, każdy filozof musi posiadać dwa widelce – lewy i prawy, co prowadzi do współzawodnictwa o zasoby.  
Wyzwania polegają na:
- Uniknięciu zakleszczenia (deadlock), w którym każdy filozof czeka na drugi widelec.
- Zapewnieniu sprawiedliwej dystrybucji zasobów, aby żaden filozof nie był nieskończenie głodzony.

Link do wikipedii (źródło): [https://pl.wikipedia.org/wiki/Problem_ucztujących_filozofów](https://pl.wikipedia.org/wiki/Problem_ucztujących_filozofów)



## Opis watkow
W implementacji każdy filozof jest reprezentowany przez osobny wątek.  
- **Filozofowie:** Każdy wątek reprezentuje filozofa, który cyklicznie:
  - **Myśli:** Filozof spędza pewien czas w stanie "myślenia", podczas którego nie korzysta ze wspólnych zasobów.
  - **Próbuje zabrać widelce:** Filozof podejmuje próbę zdobycia dwóch widelców (lewy i prawy) w zależności od swojego numeru. Filozofowie o parzystym numerze próbują najpierw wziąć lewy widelec, a o nieparzystym – najpierw prawy.
  - **Je:** Po zdobyciu obu widelców, filozof zaczyna jeść, po czym zwalnia widelce.
- **Główna synchronizacja:** Wypisywanie komunikatów na konsolę odbywa się w krytycznej sekcji zabezpieczonej przy użyciu klasy `Lock`, aby uniknąć konfliktu między wątkami.



## Sekcje krytyczne
Program zawiera kilka sekcji krytycznych, w których konieczne jest zarządzanie dostępem do wspólnych zasobów:
- **Dostęp do konsoli:**  
  Aby zapewnić czytelność wypisywanych komunikatów, dostęp do strumienia wyjściowego (`std::cout`) jest chroniony za pomocą obiektu `coutLock` (klasa `Lock`). Dzięki temu komunikaty z różnych wątków nie mieszają się.
- **Pozyskiwanie widelców:**  
  Każdy widelec (instancja klasy `Fork`) zawiera mechanizm sprawdzania dostępności, który przy próbie zmiany stanu wykorzystuje operacje atomowe.  
  - Taka synchronizacja jest kluczowa, ponieważ jednoczesne próby przejęcia widelców przez wielu filozofów mogą prowadzić do zakleszczenia.
  - Strategiczne pobieranie widelców zależnie od numeru filozofa (zmieniona kolejność dla filozofów parzystych i nieparzystych) pozwala na uniknięcie sytuacji, w których każdy filozof trzyma jeden widelec i czeka na drugi.

