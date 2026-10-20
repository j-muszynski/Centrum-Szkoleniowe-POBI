# System Zarządzania Ośrodkiem Szkolenia Kierowców (OSK)

Projekt zaliczeniowy z przedmiotu **Programowanie Obiektowe**. Jest to w pełni funkcjonalna aplikacja konsolowa służąca do kompleksowego zarządzania szkołą nauki jazdy. System automatyzuje procesy planowania jazd, zarządzania flotą pojazdów, ewidencji kursantów i instruktorów oraz rozliczeń finansowych.

## Kluczowe funkcjonalności

- **Zarządzanie harmonogramem jazd:** Zaawansowany moduł planowania sesji uwzględniający dostępność czasową instruktorów, kursantów oraz pojazdów (ochrona przed nakładaniem się terminów).
- **Obsługa uprawnień i kategorii:** System automatycznie weryfikuje, czy przypisany instruktor posiada kwalifikacje do prowadzenia kursu w danej kategorii (AM, A, B, C, D, CE, DE) oraz czy dobrany pojazd spełnia wymogi techniczne.
- **Rozliczenia finansowe:** Dynamiczne obliczanie kosztów kursów na podstawie stawki godzinowej dedykowanej dla konkretnego typu pojazdu oraz sumowanie całkowitego przychodu ośrodka.
- **Trwała baza danych:** Stan aplikacji jest automatycznie zapisywany i wczytywany z plików tekstowych przy użyciu serializacji.

## Wykorzystane technologie

- **Język:** C++ (standard C++20)
- **System budowania:** CMake
- **Biblioteki zewnętrzne:** Boost (`Serialization`, `Date_Time`, `Unit Test Framework`)

## Architektura i wzorce projektowe

Projekt został zrealizowany z naciskiem na czysty kod i paradygmaty obiektowe (SOLID):

- **Wzorzec Repozytorium (Repository Pattern):** Abstrakcyjna klasa `AbstractFileRepository<T>` zarządza kolekcjami w pamięci, delegując zapis i odczyt do generycznego interfejsu `IDataStorage<T>`.
- **Polimorfizm i dziedziczenie:** Rozbudowana hierarchia klas z klasami abstrakcyjnymi (np. `Person` dziedziczone przez `Student` i `Instructor`, oraz `Vehicle` rozszerzane przez `HeavyVehicle`, `Motorcycle` itp.).
- **Zarządzanie pamięcią:** Konsekwentne wykorzystanie inteligentnych wskaźników (`std::shared_ptr`) oraz własnych aliasów typów (m.in. `StudentPtr`, `VehiclePtr`) do zarządzania cyklem życia obiektów.
- **Wstrzykiwanie zależności (Dependency Injection):** Centralna klasa `Coordinator` integruje wszystkie menedżery (`CourseManager`, `VehicleManager` itp.), co ułatwia testowanie i rozbudowę systemu.

## Jak uruchomić projekt lokalnie

### Wymagania wstępne

Aby skompilować projekt, musisz mieć zainstalowany kompilator obsługujący **C++20**, **CMake** (min. wersja 3.20) oraz bibliotekę **Boost** na swoim systemie.

### Budowanie i uruchamianie

**1. Sklonuj repozytorium**

```bash
git clone https://github.com/j-muszynski/Centrum-Szkoleniowe-POBI.git
cd Centrum-Szkoleniowe-POBI
```

**2. Wygeneruj pliki budowania za pomocą CMake**

```bash
mkdir build && cd build
cmake ..
```

**3. Skompiluj projekt**

```bash
make
```

**4. Uruchom aplikację główną**

```bash
./program/Program
```

### Uruchamianie testów jednostkowych

Logika biznesowa oraz operacje na repozytoriach zostały pokryte testami jednostkowymi (Boost.Test). Aby je uruchomić, wpisz w folderze `build`:

```bash
./library/LibraryTester
```

Alternatywnie możesz użyć CTest:

```bash
make check
```

## Autorzy

- [Jakub Muszyński](https://github.com/j-muszynski)
- [Paweł Duda](https://github.com/Dudz1u)
