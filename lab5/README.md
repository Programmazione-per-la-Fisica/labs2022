# Laboratorio 5

L'obiettivo di questo laboratorio è di distribuire su più file il codice
preparato durante il [quarto laboratorio](../lab4/README.md) per
implementare e testare la versione "migliorata" della classe `Sample`.
In aggiunta, utilizzeremo la famiglia di _tool_ _CMake_ per facilitare il
processo di compilazione.

---

- [Laboratorio 5](#laboratorio-5)
  - [Area di lavoro](#area-di-lavoro)
  - [Distribuzione del codice in più file](#distribuzione-del-codice-in-più-file)
    - [Preparazione di `statistics.hpp`](#preparazione-di-statisticshpp)
    - [Utilizzo del namespace `pf`](#utilizzo-del-namespace-pf)
    - [Test della _One-Definition Rule_ ed uso di _include guard_](#test-della-one-definition-rule-ed-uso-di-include-guard)
    - [Utilizzo di `Statistics` in più di una _translation-unit_](#utilizzo-di-statistics-in-più-di-una-translation-unit)
    - [Definizione di funzioni in `statistics.cpp`](#definizione-di-funzioni-in-statisticscpp)
  - [Compilazione tramite `CMake`](#compilazione-tramite-cmake)
  - [Consegna facoltativa del lavoro svolto](#consegna-facoltativa-del-lavoro-svolto)
  - [Approfondimenti ed esercizi](#approfondimenti-ed-esercizi)

---

## Area di lavoro

Creiamo una nuova directory di lavoro (ad esempio `pf_labs/lab5`) e aggiungiamo
il file `.clang-format`. Possiamo copiarlo dalla cartella utilizzata per il primo
laboratorio, o generarlo ex-novo.

> :exclamation: Per esempio, potremmo procedere in modo analogo a quanto fatto
> [qui](../lab1/README.md#area-di-lavoro).

In seguito scarichiamo il file
[`doctest.h`](https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h)
tramite il seguente comando:

```bash
$ pwd
/home/fferrari/pf_labs/lab5
$ curl https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h -o doctest.h
```

Andiamo poi a copiare nell'area di lavoro il programma sviluppato durante il
quarto laboratorio.

Siete liberi di scegliere se effettuare una copia del codice che voi stessi
avete sviluppato, o scaricare la soluzione da noi proposta tramite il comando:

```bash
$ curl https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2022/main/lab4/soluzioni/statistics_algo.cpp -o statistics_algo.cpp
```

Seguendo le convenzioni discusse a lezione, rinominiamo infine il file
relativo al programma, in modo che sia ben chiaro questo contiene _unit test_:

```bash
$ mv statistics_algo.cpp statistics.test.cpp
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

- due file (`statistics.hpp` e `statistics.cpp`) che includono tutto il
  codice necessario per il calcolo delle grandezze statistiche di interesse,
  quindi tutte le _class_, _struct_ e le funzioni a esclusione dei test;
- un file `statistics.test.cpp` contenente solo i test e il supporto per la
  loro esecuzione.

In particolare, vogliamo raggiungere una condizione finale in cui:

- `statistics.hpp` contenga la definizione della classe `Sample`, ma includa solo
  le dichiarazioni delle funzioni membro in essa contenute, così come l'overload
  dell'operatore somma implementato come funzione libera;
- le definizioni dei metodi relativi a `Sample` (`size`, `add`, `statistics` e
  `entries`, l'overload dell'operatore `+=`), e la definizione dell'operatore
  somma implementato come una funzione libera dovranno invece essere
  implementate all'interno di `statistics.cpp`.

Ciò ci permetterà di comprendere l'uso delle _include guard_, della parola
chiave `inline` e di familiarizzare con alcuni errori comuni di compilazione e
linking.

Durante il laboratorio vi invitiamo a tenere aperte le
[slide](https://github.com/Programmazione-per-la-Fisica/pf2022/releases/latest)
presentate a lezione.

### Preparazione di `statistics.hpp`

Cominciamo creando un nuovo file `statistics.hpp`, dove spostiamo tutte le
classi, le funzioni e gli _include statement_ contenuti in `statistics.test.cpp`
(ad eccezione di `#include "doctest.h"`).

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
$ g++ -Wall -Wextra statistics.test.cpp -o statistics.t
statistics.test.cpp: In function 'void DOCTEST_ANON_FUNC_14()':
statistics.test.cpp:5:2: error: 'Sample' was not declared in this scope
   5 |   Sample sample;
      |   ^~~~~~
In file included from statistics.test.cpp:2:
...
```

Per risolverli, è sufficiente inserire `#include "statistics.hpp"`, che è il file
contenente il codice che abbiamo spostato in precedenza.
Dopotutto il preprocessore si può immaginare come un motore di copia/incolla del
contenuto del file incluso nel punto in cui avviene l'inclusione.

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "statistics.hpp"

#include "doctest.h"

TEST_CASE("Testing the class handling a floating point data sample") {
  ...
}
```

Verifichiamo **compilando di nuovo**: es eseguendo i test

```bash
$ g++ -Wall -Wextra statistics.test.cpp -o statistics.t
$ ./statistics.t
[doctest] doctest version is "2.4.11"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases:  1 |  1 passed | 0 failed | 0 skipped
[doctest] assertions: 29 | 29 passed | 0 failed |
[doctest] Status: SUCCESS!
```

> :exclamation: Notate come il file `statistics.hpp` **non sia incluso nel
> comando di compilazione**.

### Utilizzo del namespace `pf`

Come discusso a lezione, nel caso progetti complessi risulta utile utilizzare
`namespace` per garantire che non ci siano conflitti tra i diversi identificatori
(es.: nomi delle funzioni) utilizzati nelle loro varie parti.

A titolo d'esempio, introduciamo quindi il namespace `pf` per racchiudere,
`Sample`, `Statistics` e l'overload dell'operatore `operator+`.

> :exclamation: spesso un _namespace_ può contenere parti di una libreria
> eterogenee e diverse tra loro (pensate alla _Standard Template Library_),
> pertanto il nome di un dato namespace è spesso "generico" e non
> necessariamente riflette il nome di una funzionalità specifica di una data
> libreria.

Per farlo, andiamo a racchiudere il blocco di codice che abbiamo appena spostato
in `statistics.hpp` all'interno del namespace:

```c++
namespace pf {
  struct Statistics {
    double mean{};
    double sigma{};
    double mean_err{};
    double median{};
  };

  class Sample {
    ...
  };
  
  ...
}
```

In seguito andiamo, ad anteporre `pf::` prima della costruzione di oggetti di
tipo `Sample` in`statistics.test.cpp`:

```c++
TEST_CASE("Testing the class handling a floating point data sample") {
  pf::Sample sample;
  ...
}
```

**Formattiamo**, compiliamo ed eseguiamo i test per verificare che il programma
si comporti come atteso.

### Test della _One-Definition Rule_ ed uso di _include guard_

Cosa succede se includiamo `statistics.hpp` **due** volte? Abbiamo visto che può
capitare che lo stesso _header_ file possa essere incluso, direttamente o
indirettamente, più volte nella stessa _translation unit_ (_TU_).

Proviamo quindi a farlo:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "statistics.hpp"
#include "statistics.hpp"

#include "doctest.h"

TEST_CASE("Testing the class handling a floating point data sample") {
  ...
}
```

e compiliamo:

```bash
$ g++ -Wall -Wextra statistics.test.cpp -o statistics.t
In file included from statistics.test.cpp:4:
statistics.hpp:8:10: error: redefinition of 'struct pf::Statistics'
    8 |   struct Statistics {
      |          ^~~~~~~~~~
In file included from statistics.test.cpp:3:
statistics.hpp:8:10: note: previous definition of 'struct pf::Statistics'
    8 |   struct Statistics {
      |          ^~~~~~~~~~
statistics.hpp:15:9: error: redefinition of 'class pf::Sample'
   15 |   class Sample {
      |         ^~~~~~
statistics.hpp:15:9: note: previous definition of 'class pf::Sample'
   15 |   class Sample {
      |         ^~~~~~
...
```

Possiamo notare ridefinizioni, sia della struct `Statistics` che della classe
`Sample`. Siccome siamo all'interno della stessa _TU_, il compilatore è in grado
di diagnosticare queste violazioni della _One-Definition Rule_.

Anche in questo caso la soluzione è semplice: inseriamo un'_include guard_ in
`statistics.hpp`, in modo tale che, se l'header file viene incluso più volte
nella stessa _TU_, il suo contenuto venga considerato solo la prima volta.

Aggiungiamo quindi all'inizio e alla fine del file `statistics.hpp`:

```c++
#ifndef STATISTICS_HPP
#define STATISTICS_HPP

...

#endif
```

> :exclamation: `STATISTICS_HPP` è una _macro_ del preprocessore.
>
> Invece di `STATISTICS_HPP` avremmo potuto scegliere un nome qualsiasi, ma è
> necessario garantirne l'unicità tra tutti gli _header file_ che fanno parte del
> nostro progetto, o che appartengono ad altri progetti ma che includiamo.
>
> Scegliere un nome che richiami il nome del file, scritto tutto in lettere
> maiuscole, è una delle convenzioni maggiormente utilizzate (che quindi
> adottiamo anche noi).

A questo punto siamo in grado di compilare ed eseguire correttamente i nostri
test. Ci siamo ricondotti ad una situazione stabile:

```bash
$ g++ -Wall -Wextra statistics.test.cpp -o statistics.t
$ ./statistics.t
[doctest] doctest version is "2.4.11"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases:  1 |  1 passed | 0 failed | 0 skipped
[doctest] assertions: 29 | 29 passed | 0 failed |
[doctest] Status: SUCCESS!
```

Dopo questa verifica, provvediamo a rimuovere dal file `statistics.test.cpp` lo
_statement_ `#include "statistics.hpp"` duplicato, che abbiamo utilizzato per il
test.

### Utilizzo di `Statistics` in più di una _translation-unit_

Supponiamo ora che il nostro componente software sia parte, o venga usato, in un
progetto con molti file e che quindi `statistics.hpp` possa essere incluso in più
_TU_.

Il modo più semplice per porsi in tale situazione, e quindi verificare che stiamo
facendo le cose per bene, è di aggiungere una _TU_ formata da un file sorgente che
si limiti a includere l'header corrispondente. Questa _TU_ viene poi integrata nel
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
$ g++ -Wall -Wextra -c statistics.cpp
$ ls -t | grep statistics
statistics.o
statistics.cpp
statistics.t
statistics.hpp
statistics.test.cpp

```

> :exclamation: in questo caso si è aggiunta l'opzione `-c` al comando di
> compilazione, per dire al compilatore di non eseguire la fase di _linking_ e
> creare solamente _file oggetto_.  `statistics.o`

Verificato che sia tutto a posto, possiamo provare a compilare tutto insieme:

```bash
$ g++ -Wall -Wextra statistics.test.cpp statistics.cpp -o statistics.t
duplicate symbol '__ZN2pfplERKNS_6SampleES2_' in:
    /var/folders/t7/4c0fbk014319d6zkgm2d88g40000gn/T//ccuYN7vM.o
    /var/folders/t7/4c0fbk014319d6zkgm2d88g40000gn/T//ccf2v2Ug.o
ld: 1 duplicate symbol for architecture arm64
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
all'esterno della classe, rimanendo sempre nel file `statistics.hpp`:

```c++

namespace pf {
  class Sample {
    ...
    void add(double x);
    ...
  };
}

...

void pf::Sample::add(double x) { entries_.push_back(x); }
```

> :exclamation: ricordarsi di aggiungere il nome della classe seguito dallo
> _scope operator_ (cioè `pf::Sample::`) davanti al nome del metodo.
>
> :question: Ci serve veramente dare un nome  alla variabile argomento del
> metodo `Sample::add(double x)` durante la dichiarazione? Cosa succede se
> rimuoviamo la `x`?

Se riproviamo a compilare, oltre all'errore precedente, ne otteniamo un altro:

```bash
$ g++ -Wall -Wextra statistics.test.cpp statistics.cpp -o statistics.t 
duplicate symbol '__ZN2pf6Sample3addEd' in:
    /var/folders/t7/4c0fbk014319d6zkgm2d88g40000gn/T//ccRx6QiF.o
    /var/folders/t7/4c0fbk014319d6zkgm2d88g40000gn/T//ccrPYRAf.o
duplicate symbol '__ZN2pfplERKNS_6SampleES2_' in:
    /var/folders/t7/4c0fbk014319d6zkgm2d88g40000gn/T//ccRx6QiF.o
    /var/folders/t7/4c0fbk014319d6zkgm2d88g40000gn/T//ccrPYRAf.o
ld: 2 duplicate symbols for architecture arm64
collect2: error: ld returned 1 exit status
```

Per risolvere il problema è sufficiente aggiungere la parola chiave `inline` di
fronte alle definizioni delle funzioni definite fuori dalla classe:

```c++
  inline Sample operator+(const Sample& l, const Sample& r) {
    Sample sum{l};
    return sum += r;
  }

...

inline void pf::Sample::add(double x) { entries_.push_back(x);}

```

Verifichiamo a questo punto che il codice compili e funzioni correttamente:

```bash
$ g++ -Wall -Wextra statistics.test.cpp statistics.cpp -o statistics.t 
$ ./statistics
[doctest] doctest version is "2.4.11"
[doctest] run with "--help" for options
===============================================================================
[doctest] test cases:  1 |  1 passed | 0 failed | 0 skipped
[doctest] assertions: 29 | 29 passed | 0 failed |
[doctest] Status: SUCCESS!
```

### Definizione di funzioni in `statistics.cpp`

Come discusso a lezione, la configurazione raggiunta al punto precedente è di
per se sufficiente per ottenere la separazione necessaria per utilizzare
`Sample` all'interno di molteplici _TU_ di uno stesso programma.

Un'altra alternativa è quella di spostare la definizione delle funzioni
contenute in `statistics.hpp` all'interno di `statistics.cpp`, mantenendo in
`statistics.hpp` solo la loro dichiarazione.

**Entrambe le strategie hanno vantaggi e svantaggi**. Siccome il nostro scopo è
quello di impratichirci con tutte le possibili opzioni di suddivisione del
codice, proviamo ad implementarle entrambe.

Cominciamo spostando la definizione di `Sample::add(double x)` all'interno di
`statistics.cpp`, che diventa:

```c++
#include "statistics.hpp"

void pf::Sample::add(double x) { 
  ... 
}
```

Verifichiamo quindi che compilazione e test vadano a buon fine.

> :exclamation: Una volta spostata la definizione di `Sample::add(double x)` in
> `statistics.cpp` dobbiamo ricordarci di rimuovere la parola chiave `inline`.
>
> :question: Cosa succede se non lo facciamo?

Procediamo nello stesso modo spostando anche
`Sample operator+(const Sample& l, const Sample& r)`.

> :exclamation: Ricordatevi di anteporre lo _scope operator_ `pf::` ovunque
> risulti necessario.

Compilando possiamo notare che la funzione non è più visibile al di fuori di
`statistics.cpp`:

```bash
$ g++ -Wall -Wextra statistics.test.cpp statistics.cpp -o statistics.t
statistics.test.cpp: In function 'void DOCTEST_ANON_FUNC_14()':
statistics.test.cpp:72:23: error: no match for 'operator+' (operand types are 'pf::Sample' and 'pf::Sample')
   72 |     auto sum = sample + sample_two;
      |                ~~~~~~ ^ ~~~~~~~~~~
      |                |        |
      |                |        pf::Sample
      |                pf::Sample
statistics.cpp:58:66: error: 'pf::Sample pf::operator+(const Sample&, const Sample&)' should have been declared inside 'pf'
   58 | pf::Sample pf::operator+(const pf::Sample& l, const pf::Sample& r) {
      |                                                                  ^
```

Per renderla di nuovo accessibile, dobbiamo dichiarare (ma non definire)
`Sample operator+(const Sample&, const Sample&)` all'interno di
`statistics.hpp`, dopo che `Sample` è stata dichiarata:

```c++
...
class Sample {
  ...
};

Sample operator+(const Sample&, const Sample&);

#endif
```

Procediamo dunque spostando la definizione delle seguenti funzioni membro da
`statistics.hpp` a `statistics.cpp`:

```c++
  auto remove(double x) {
    ...
  }

  Statistics statistics() const {
    ...
  }

  Sample& operator+=(const Sample& r) {
    ...
  }
```

Ricordandoci, ad ogni passaggio, di **compilare e di eseguire i test**.

> :question: Possiamo continuare ad usare `auto` come tipo di ritorno per la
> funzione membro `auto remove(double x)` o risulta necessario specificarlo in
> maniera esplicita? Perché?
>
> :question: Per evitare di anteporre ripetutamente `pf::` nell'implementazione
> delle diverse funzioni che abbiamo spostato in `statistics.cpp`, possiamo
> racchiuderle direttamente in un `namespace`, come mostrato qui?
>
> ```c++
> namespace pf {
>
>   void Sample::add(double x) { entries_.push_back(x); }
>   ...
> }
> ```

Per completare quest'ultimo punto dell l'esercizio, andiamo a considerare
un'ultima cosa: visto quanto abbiamo detto sul preprocessore, quanti degli
`#include` all'inizio di `statistics.hpp` sono veramente necessari in quel file,
ora che le definizioni delle funzioni sono tutte in `statistics.cpp`?

Procedete quindi spostando quanti più _include statement_:

```c++
#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

...
```

in `statistics.cpp`.

> :question: Perché è buona norma effettuare questa operazione?  

## Compilazione tramite `CMake`

Prima di terminare questo laboratorio, vogliamo illustrare brevemente l'utilizzo
di [`CMake`](https://cmake.org/) per semplificare la compilazione di progetti
costituiti da più file sorgente.

Siccome il nostro scopo non è esplorare "tutte" le funzionalità di `CMake`,
partiamo da un file `CMakeLists.txt` preparato apposta apposta per questo
laboratorio.

Cominciamo quindi scaricandolo:

```bash
$ pwd
/home/fferrari/pf_labs/lab5
$ curl https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2022/main/lab5/soluzioni/CMakeLists.txt -o CMakeLists.txt
```

> :exclamation: Il file che avete appena scaricato è stato preparato per
> funzionare qualora si sia seguita l'esatta nomenclatura per i file `.cpp` e
> `.hpp` proposta in questa traccia. Qualora abbiate adottato una convenzione
> diversa per i nomi dei file, modificate opportunamente il file `CMakeLists.txt`.

Dopo averlo aperto, possiamo notare come questo, di proposito, sia stato
commentato in dettaglio in modo da rendere chiaro il funzionamento di ogni
sua parte.

Per utilizzarlo, ci basta eseguire i seguenti comandi:

```bash
$ cmake -S . -B build
-- The C compiler identification is AppleClang 14.0.0.14000029
-- The CXX compiler identification is AppleClang 14.0.0.14000029
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done (0.3s)
-- Generating done (0.0s)
-- Build files have been written to: /home/fferrari/pf_labs/lab5/build
```

> Questo comando utilizza il file `CMakeLists.txt` presente nella directory
> corrente (`.`) per creare e **configurare** un'area di compilazione all'interno
> della cartella `build`.
>
> :exclamation: In caso il file `CMakeLists.txt` non cambi, basta eseguire il
> comando appena presentato una sola volta.

```bash
$ cmake --build build         
[ 33%] Building CXX object CMakeFiles/statistics.t.dir/statistics.test.cpp.o
[ 66%] Building CXX object CMakeFiles/statistics.t.dir/statistics.cpp.o
[100%] Linking CXX executable statistics.t
[100%] Built target statistics.t
```

> Questo comando esegue la compilazione dell'area presente all'interno della
> cartella `build`
>
> :exclamation: In  caso la compilazione fallisca è sufficiente modificare i file
> sorgente (`.cpp` e `.hpp`) su cui stiamo lavorando ed eseguire di nuovo
> **solo quest'ultimo comando** senza bisogno di configurare di nuovo l'area di
> compilazione.

```bash
$ cmake --build build --target test          
Running tests...
Test project /home/fferrari/pf_labs/lab5/build
    Start 1: statistics.t
1/1 Test #1: statistics.t .....................   Passed    0.32 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.32 sec
```

> Questo comando esegue i test definiti all'interno del file `CMakeLists.txt`
> tramite il comando `add_test( ... )`.

Alla fine dei turni del quinto laboratorio, potrete trovare la soluzione
dell'esercizio [al seguente link](soluzioni/).

## Consegna facoltativa del lavoro svolto

Per chi lo desiderasse, è possibile effettuare una consegna **facoltativa** del
lavoro svolto durante il laboratorio. Questa è un'opzione che lasciamo a quegli
studenti che, incerti su alcuni punti, vogliono chiarire i loro dubbi.

Le consegne **non verranno valutate** e **NON contribuiscono al risultato
dell'esame**.

Tutti coloro che effettuano una consegna facoltativa, sono pregati di
**riportare**, **come commento** alla consegna stessa, **dubbi o domande
sull'elaborato per i quali è richiesto feedback** esplicito da parte dei
docenti.

La consegna deve avvenire, da parte dei singoli studenti, tramite
[questo link](https://virtuale.unibo.it/mod/assign/view.php?id=1204770), il
quale prevede il solo caricamento di file `.zip` o `.tgz`.

Supponendo che tutto il materiale sia nella cartella `lab5` (e supponendo di
trovarsi in tale cartella), per creare un archivio `.zip` procedere come segue:

```bash
$ pwd
/home/fferrari/pf_labs/lab5
$ cd ..
$ zip -r lab5.zip lab5
$ ls
lab4 lab5.zip
```

Per creare un archivio `.tgz` procedere invece come segue:

```bash
$ pwd
/home/fferrari/pf_labs/lab5
$ cd ..
$ tar czvf lab5.tgz lab5
$ ls
lab4 lab5.tgz
```

## Approfondimenti ed esercizi

Per chi fosse interessato a "sperimentare ulteriormente" gli argomenti
presentati in questo laboratorio, viene proposto un possibile approfondimento
**opzionale**:

Provate ad aggiungere un nuovo file `main.cpp` contenente una funzione `main`
che usa i componenti definiti sopra.

Il programma accetta dall'utente comandi:

- `a VALORE`: per aggiungere un dato al campione. Ad esempio, per
  aggiungere il valore `9.1`, scrivere `a 9.1`;
- `r VALORE`: per rimuovere un dato al campione. Ad esempio, per
  rimuovere il valore `9.1`, scrivere `r 9.1`;
  > In questo caso, ricordatevi di stampare a schermo un messaggio se la
  > rimozione **non** ha avuto successo.
- `c` per eseguire il calcolo delle statistiche e stamparle a schermo.
- `q` per uscire dal programma.

Qualora vengano immessi comandi sconosciuti, il programma deve menzionarlo,
per poi richiedere di nuovo l'inserimento di un comando valido.

Ricordatevi inoltre di gestire eventuali eccezioni e di controllare la
correttezza formale dell'input da tastiera.

Cambiate poi opportunamente il file `CMakeLists.txt` in modo da includere
la compilazione del _main program_.
