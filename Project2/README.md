<h1 align="center">Wielowątkowy serwer czatu</h1>
<h3 align="center">Drugi program napisany na potrzeby projektu z kursu Systemy Operacyjne 2</h3>



## Spis treści
- [Uruchomienie projektu](#uruchomienie-projektu)
- [Opis problemu](#opis-problemu)
- [Opis wątków](#opis-watkow)
- [Sekcje krytyczne](#sekcje-krytyczne)



## Uruchomienie projektu
Poniższy zrzut ekranu prezentuje uruchomienie programu.
Najpierw uruchamiamy program serwera poleceniem `python3 server.py`, a następnie klientów poleceniem `python3 client.py`. 
- Przy uruchomieniu programu klienta bez wcześniejszego uruchomienia serwera, program nie działa.

<p align="center"> 
<img src="https://github.com/user-attachments/assets/e9705aee-8027-4128-8690-0e5ee900c055">
</p>

<p align="center"> 
<img src="https://github.com/user-attachments/assets/981572cc-59c0-42ee-a27a-e3050dd8eb01">
</p>





## Opis problemu
Projekt implementuje prosty, wielowątkowy serwer czatu, który umożliwia komunikację między wieloma klientami jednocześnie. Serwer nasłuchuje na określonym porcie (8080) i po nawiązaniu połączenia z klientem, tworzy nowy wątek dedykowany obsłudze tego klienta. Głównym zadaniem serwera jest odbieranie wiadomości od połączonych klientów i przesyłanie ich do wszystkich pozostałych uczestników rozmowy. Dzięki temu każdy klient może brać udział w grupowej konwersacji, co symuluje klasyczny czat.

## Opis wątków
- **Wątki serwera:** Każdy klient, po połączeniu się z serwerem, jest obsługiwany przez dedykowany wątek. Wątek ten odpowiada za odbieranie wiadomości od danego klienta oraz przekazywanie otrzymanych danych do wszystkich pozostałych podłączonych klientów.
- **Wątek klienta:** Po stronie klienta uruchamiany jest osobny wątek, który nieustannie nasłuchuje wiadomości przychodzących od serwera. Dzięki temu użytkownik może jednocześnie pisać wiadomości i otrzymywać komunikaty z czatu.

## Sekcje krytyczne
- **Dostęp do listy połączonych klientów:** Lista `connected_clients` jest chroniona za pomocą mechanizmu `client_list_lock`. Zapewnia to, że operacje dodawania oraz usuwania klientów odbywają się w sposób bezpieczny, bez ryzyka kolizji między wątkami.
- **Wypisywanie komunikatów na konsolę:** Operacje związane z logowaniem informacji (np. łączenie, rozłączanie, błędy) są synchronizowane poprzez `output_lock`. Chroni to przed mieszaniem się komunikatów z różnych wątków, co zapewnia czytelność logów.
