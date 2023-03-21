# Laboratorio 5

L'obiettivo di questo laboratorio è di distribuire su più file il codice
preparato durante il [terquartozo laboratorio](../lab4/README.md) per
implementare e testare la versione "migliorata" della classe `statistics`.
In aggiunta, utilizzeremo la famiglia di _tool_ _CMake_ per facilitare il 
processo di compilazione.

---

- [Laboratorio 5](#laboratorio-5)
  - [Area di lavoro](#area-di-lavoro)
  - [Distribuzione del codice in più file](#distribuzione-del-codice-in-più-file)
    - [Preparazione di `statistics.hpp`](#preparazione-di-statisticshpp)
    - [Test della _One-Definition Rule_ ed uso di _include guard_](#test-della-one-definition-rule-ed-uso-di-include-guard)
    - [Utilizzo di `Statistics` in più di una translation-unit](#utilizzo-di-statistics-in-più-di-una-translation-unit)
    - [Definizione di funzioni in `statistics.cpp` __DA CONTROLLARE__](#definizione-di-funzioni-in-statisticscpp-da-controllare)

---

## Area di lavoro

Creiamo una nuova directory di lavoro (ad esempio `pf_labs/lab5`) e aggiungiamo
il file `.clang-format`. Possiamo copiarlo dalla cartella utilizzata per il primo
laboratorio, o generarlo ex-novo.

> :exclamation: Per esempio, potete procedere in modo analogo a quanto fatto
> [qui](../lab1/README.md#area-di-lavoro).

In seguito scarichiamo il file
[`doctest.h`](https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h)
tramite il seguente comando:

```bash
$ pwd
/home/fferrari/pf_labs/lab5
$ curl https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h -o doctest.h
```

Andiamo infine a copiare nell'area di lavoro il programma sviluppato durante il
quarto laboratorio.

Siete liberi di scegliere se effettuare una copia del codice che voi stessi
avete sviluppato, o scaricare la soluzione da noi proposta tramite il comando:

```bash
$ curl https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2022/main/lab4/soluzioni/statistics.cpp -o statistics.test.cpp
```

Fatto ciò, l'area di lavoro deve trovarsi in questo stato:

```bash
$ pwd
/home/fferrari/pf_labs/lab5
$ ls -A
.clang-format  doctest.h statistics.test.cpp
```

## Distribuzione del codice in più file

Quello che ci prefiggiamo di fare durante questa esercitazione è separare il
codice presente all'interno di `statistics.test.cpp`, in modo tale da giungere
in una situazione in cui ci siano:

- due file (`statistics.hpp` e `statistics.test.cpp`) che includono tutto il
  codice necessario per il calcolo delle grandezze statistiche di interesse,
  quindi tutte le _class_, _struct_ e le funzioni a esclusione dei test;
- un file `statistics.test.cpp` contenente solo i test e il supporto per la
  loro esecuzione.

In particolare, vogliamo raggiungere una condizione finale in cui:

- `statistics.hpp` contiene la definizione della classe `Sample`, ma includa solo
  le dichiarazioni delle funzioni membro in essa contenute, così come l'overload
  dell'operatore somma implementato come funzione libera;
- le definizioni dei metodi relativi a `Sample` (`size`, `add`, `statistics` e
  `entries`), l'overload dell'operatore `+=`, la definizione dell'operatore somma
  implementato come una funzione libera dovranno invece essere implementate
  all'interno di `statistics.test.cpp`.

Ciò ci permetterà di comprendere l'uso delle _include guard_, della parola
chiave `inline` e di familiarizzare con alcuni errori comuni di compilazione e
linking.

Durante il laboratorio vi invitiamo a tenere aperte le
[slide](https://github.com/Programmazione-per-la-Fisica/pf2022/releases/latest)
presentate a lezione.

### Preparazione di `statistics.hpp`

Cominciamo creando un nuovo file `statistics.hpp`, dove spostiamo tutte le classi
e le funzioni contenute in `statistics.test.cpp`.

Dentro `statistics.test.cpp` dovrà rimanere solamente il contenuto relativo ai
test:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("Testing the class handling a floating point data sample") {
  ...
}
```

Se proviamo a compilare ora `statistics.test.cpp` otterremo errori di
compilazione, dovuti alla mancanza di `Statistics`:

```bash
$ g++ -Wall -Wextra statistics.test.cpp -o statistics
statistics.test.cpp: In function 'void DOCTEST_ANON_FUNC_8()':
statistics.test.cpp:6:3: error: 'Sample' was not declared in this scope
    6 |   Sample sample;
      |   ^~~~~~
In file included from statistics.test.cpp:2:
```

Per risolverli, è sufficiente inserire `#include "statistics.hpp"`, che è il file
contenente il codice che abbiamo spostato in precedenza.
Dopotutto il preprocessore si può immaginare come un motore di copia/incolla del
contenuto del file incluso nel punto in cui avviene l'inclusione.

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "statistics.hpp"

TEST_CASE("Testing the class handling a floating point data sample") {
  ...
}
```

Verifichiamo __compilando di nuovo__.

### Test della _One-Definition Rule_ ed uso di _include guard_

Cosa succede se includiamo `statistics.hpp` __due__ volte? Abbiamo visto che può
capitare che lo stesso header file possa essere incluso, direttamente o
indirettamente, più volte nella stessa _translation unit_ (TU).

Proviamo quindi a farlo:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "statistics.hpp"
#include "statistics.hpp"

TEST_CASE("Testing the class handling a floating point data sample") {
  ...
}
```

e compiliamo:

```bash
$ g++ -Wall -Wextra statistics.test.cpp -o statistics
In file included from statistics.test.cpp:4:
statistics.hpp:6:8: error: redefinition of 'struct Statistics'
    6 | struct Statistics {
      |        ^~~~~~~~~~
In file included from statistics.test.cpp:3:
statistics.hpp:6:8: note: previous definition of 'struct Statistics'
    6 | struct Statistics {
      |        ^~~~~~~~~~
statistics.hpp:13:7: error: redefinition of 'class Sample'
   13 | class Sample {
      |       ^~~~~~
statistics.hpp:13:7: note: previous definition of 'class Sample'
   13 | class Sample {
      |       ^~~~~~
```

Possiamo notare ridefinizioni, sia della struct `Statistics` che della classe
`Sample`. Siccome siamo all'interno della stessa TU, il compilatore è in grado
di diagnosticare queste violazioni della _One-Definition Rule_.

Anche in questo caso la soluzione è semplice: inseriamo un'_include guard_ in
`statistics.hpp`, in modo tale che, se l'header file viene incluso più volte
nella stessa TU, il suo contenuto venga considerato solo la prima volta.

Aggiungiamo quindi all'inizio e alla fine del file:

```c++
#ifndef STATISTICS_HPP
#define STATISTICS_HPP

...

#endif
```

> __NOTA__: `STATISTICS_HPP` è una _macro_ del preprocessore. Invece di
> `STATISTICS_HPP` avremmo potuto scegliere un nome qualsiasi, ma è necessario
> garantirne l'unicità tra tutti gli header file che fanno parte del nostro
> progetto, o che appartengono ad altri progetti ma sono inclusi dal nostro.
> Scegliere un nome che richiami il nome del file, scritto tutto in lettere
> maiuscole, è una delle convenzioni maggiormente utilizzate (che quindi
> adottiamo anche noi).

A questo punto siamo in grado di compilare ed eseguire correttamente i nostri
test. Ci siamo ricondotti ad una situazione stabile:

```bash
$ g++ -Wall -Wextra statistics.test.cpp -o statistics
$ ./statistics
[doctest] doctest version is "2.4.7"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases:  1 |  1 passed | 0 failed | 0 skipped
[doctest] assertions: 29 | 29 passed | 0 failed |
[doctest] Status: SUCCESS!
```

Dopo questa verifica, provvediamo a rimuovere dal file `statistics.test.cpp` lo
_statement_ `#include "statistics.hpp"` duplicato, che abbiamo utilizzato per il
test.

### Utilizzo di `Statistics` in più di una translation-unit

Supponiamo ora che il nostro componente software sia parte, o venga usato, in un
progetto con molti file e che quindi `statistics.hpp` possa essere incluso in più
_TU_.

Il modo più semplice per porsi in tale situazione, e quindi verificare che stiamo
facendo le cose per bene, è di aggiungere una TU formata da un file sorgente che
si limiti a includere l'header corrispondente. Questa TU viene poi integrata nel
nostro comando di compilazione.

Creiamo quindi un file `statistics.cpp`, il cui contenuto sia semplicemente:

```c++
#include "statistics.hpp"
```

Prima di aggiungerlo al comando di compilazione precedente, proviamo a
compilarlo separatamente (cioè, come singola _TU_).

Questo permette di verificare che l'header file `statistics.hpp` sia completo,
che cioè includa tutto ciò che serve alle entità che dichiara/definisce.

```bash
$ g++ -Wall -Wextra -c statistics.cpp -o statistics
$ ls -t | grep statistics
statistics.o
statistics.cpp
statistics
statistics.hpp
statistics.test.cpp

```

> __NOTA__: in questo caso si è aggiunta l'opzione `-c` al comando di
> compilazione, per dire al compilatore di non eseguire la fase di _linking_ e
> creare solamente _file oggetto_.  `statistics.o`

Verificato che sia tutto a posto, possiamo provare a compilare tutto insieme:

```bash
$ g++ -Wall -Wextra statistics.test.cpp statistics.cpp -o statistics
duplicate symbol '__ZplRK6SampleS1_' in:
    /var/folders/px/bwnqrtg96d1_djn9xdllhvz40000gn/T//ccTSEySD.o
    /var/folders/px/bwnqrtg96d1_djn9xdllhvz40000gn/T//ccjhrmgE.o
ld: 1 duplicate symbol for architecture x86_64
collect2: error: ld returned 1 exit status

```

Possiamo notare un errore di _linking_, dovuto alla funzione
`Sample operator+(const Sample& l, const Sample& r)`, definita al di fuori della
classe `Sample`.

Ciò è dovuto al fatto che, essendo definita in un header file, ed essendo questo
header file incluso in due _TU_,
`Sample operator+(const Sample& l, const Sample& r)` risulta definita due volte
all'interno dello stesso programma.

Prima di risolvere il problema, replichiamolo anche per una funzione membro
(metodo), ad esempio `Sample::add(double x)`.

Dentro la classe ci limitiamo a dichiararla e spostiamo la definizione 
all'esterno della classe:

```c++
class Sample {
  ...
  void add(double x);
  ...
};

...

void Sample::add(double x) {entries_.push_back(x);}
```

> __NOTA__: ricordarsi di aggiungere il nome della classe seguito dallo _scope
> operator_ (cioè `Sample::`) davanti al nome del metodo.

Se riproviamo a compilare, oltre all'errore precedente, ne otteniamo un altro:

```bash
$ g++ -Wall -Wextra regression.test.cpp regression.cpp
duplicate symbol '__ZN6Sample3addEd' in:
    /var/folders/px/bwnqrtg96d1_djn9xdllhvz40000gn/T//ccyfeSwq.o
    /var/folders/px/bwnqrtg96d1_djn9xdllhvz40000gn/T//ccPJNRud.o
duplicate symbol '__ZplRK6SampleS1_' in:
    /var/folders/px/bwnqrtg96d1_djn9xdllhvz40000gn/T//ccyfeSwq.o
    /var/folders/px/bwnqrtg96d1_djn9xdllhvz40000gn/T//ccPJNRud.o
ld: 2 duplicate symbols for architecture x86_64
collect2: error: ld returned 1 exit status
```

Per risolvere il problema è sufficiente aggiungere la parola chiave `inline` di
fronte alle definizioni delle funzioni definite fuori dalla classe:

```c++
inline void Sample::add(double x) { entries_.push_back(x);}

inline Sample operator+(const Sample& l, const Sample& r) {
  Sample sum{l};
  return sum += r;
}

inline auto statistics(Sample const & sample) {
  // the following call would fail compilation if the fit method weren't const
  return sample.statistics();
}
```

Verifichiamo a questo punto che il codice compili e funzioni correttamente:

```bash
$ g++ -Wall -Wextra statistics.test.cpp statistics.cpp -o statistics
$ ./statistics
[doctest] doctest version is "2.4.7"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases:  1 |  1 passed | 0 failed | 0 skipped
[doctest] assertions: 29 | 29 passed | 0 failed |
[doctest] Status: SUCCESS!
```

### Definizione di funzioni in `statistics.cpp` __DA CONTROLLARE__

Come discusso a lezione, la configurazione raggiunta al punto precedente è di
per se sufficiente per ottenere la separazione necessaria per utilizzare
`Sample` all'interno di molteplici TU di uno stesso programma.

Un'altra alternativa è quella di spostare la definizione delle funzioni
contenute in `statistics.hpp` all'interno di `statistics.cpp`, mantenendo in
`statistics.hpp` solo la loro dichiarazione.

__Entrambe le strategie hanno vantaggi e svantaggi__, siccome il nostro scopo è
quello di impratichirci con tutte le possibili opzioni di suddivisione del
codice, proviamo ad implementarle entrambe.

Cominciamo spostando la definizione di `Sample::add(double x)` all'interno di
`statistics.cpp`, che diventa:

```c++
#include "statistics.hpp"

void Sample::add(double x) { 
  ... 
}
```

Verifichiamo quindi che compilazione e test vadano a buon fine.

> __NOTA__: una volta spostata la definizione di
> `Sample::add(double x)` in `statistics.cpp` dobbiamo ricordarci
> di rimuovere la parola chiave `inline`. Cosa succede se non lo facciamo?
