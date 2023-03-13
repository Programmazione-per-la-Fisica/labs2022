# Laboratorio 4

In questa lezione riprenderemo il programma sviluppato durante il
[terzo laboratorio](../lab3/README.md) ed andremo ad aggiungervi nuove
funzionalità.
Dove possibile, faremo uso degli strumenti forniti dalla _standard library_ del
_C++_.

---

- [Laboratorio 4](#laboratorio-4)
  - [Area di lavoro](#area-di-lavoro)
  - [Aggiunta di nuove funzionalità alla classe `Sample`](#aggiunta-di-nuove-funzionalità-alla-classe-sample)
    - [Rimozione di un punto già inserito](#rimozione-di-un-punto-già-inserito)
    - [Aggiungere il calcolo della mediana](#aggiungere-il-calcolo-della-mediana)
  - [Overloading dell'operatore somma](#overloading-delloperatore-somma)
    - [Versione 1: operatore somma come funzione libera](#versione-1-operatore-somma-come-funzione-libera)
    - [Versione 2: operatore somma a partire dall'operatore membro `+=`](#versione-2-operatore-somma-a-partire-dalloperatore-membro-)
  - [Consegna facoltativa del lavoro svolto](#consegna-facoltativa-del-lavoro-svolto)

---

L'obiettivo principale di questo laboratorio è imparare i rudimenti dell'uso di
`std::vector<>`, implementare la somma di diversi campioni di dati (`Sample`)
tramite l'_overload_ dell'operatore `operator+` ed abituarci a consultare la
documentazione della `standard library` del `C++` alla ricerca di strumenti (es:
_container_ ed _algoritmi_) che semplifichino lo sviluppo dei nostri programmi.

Durante il laboratorio vi invitiamo a tenere aperte le
[slide](https://github.com/Programmazione-per-la-Fisica/pf2022/releases/latest)
presentate a lezione, così come la _webpage_
[cppreference.com](https://en.cppreference.com/w/).

## Area di lavoro

Creiamo una nuova directory di lavoro (ad esempio `pf_labs/lab4`) e aggiungiamo
il file `.clang-format`. Possiamo copiarlo dalla cartella utilizzata per il primo
laboratorio, o generarlo ex-novo.

> :exclamation: Per esempio, potete procedere in modo analogo a quanto fatto
> [qui](../lab1/README.md#area-di-lavoro).

In seguito scarichiamo il file
[`doctest.h`](https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h)
tramite il seguente comando:

```bash
$ pwd
/home/battilan/pf_labs/lab4
$ curl https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h -o doctest.h
```

Andiamo infine a copiare nell'area di lavoro il programma sviluppato durante il
terzo laboratorio.

Siete liberi di scegliere se effettuare una copia del codice che voi stessi
avete sviluppato, o scaricare la soluzione da noi proposta tramite il comando:

```bash
$ curl https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2022/main/lab3/soluzioni/statistics.cpp -o statistics.cpp
```

Fatto ciò, l'area di lavoro deve trovarsi in questo stato:

```bash
$ pwd
/home/battilan/pf_labs/lab4
$ ls -A
.clang-format  doctest.h statistics.cpp
```

## Aggiunta di nuove funzionalità alla classe `Sample`

Pur svolgendo il compito che ci eravamo inizialmente prefissi, questa prima
versione di `Sample` risulta migliorabile sotto vari aspetti, ad esempio:

- un eventuale utilizzatore della classe potrebbe desiderare
  rimuovere alcuni dei valori relativi ai dati introdotti in precedenza
  (es: perché ottenuti tramite un processo di misura effettuato in modo
  sbagliato, etc.);
- potrebbe essere utile estendere l'analisi di un campione aggiungendo il
  calcolo di ulteriori statistiche descrittive come la
  [mediana](https://it.wikipedia.org/wiki/Mediana_(statistica)) (o altri
  percentili).

In entrambi i casi risulta necessario "ricordare" l'intero insieme dei valori che
sono stati aggiunti tramite il metodo `add`.

Per fare ciò, andiamo a modificare la classe `Sample` per aggiungendo uno
`std::vector<double>` nella sua parte privata:

```c++
#include<vector>
...
class Sample {
  double sum_x_{};
  double sum_x2_{};
  int N_{};
  std::vector<double> entries_{};

  ...
};
```

Quando aggiungiamo una variabile membro privata, la prima cosa che dobbiamo
chiederci è quale sia il suo impatto sull'**invariante di classe**, cioè la
relazione esistente tra i dati membri di una classe.

Mantenendo tutte le variabili membro finora introdotte in `Sample`,
**in ogni momento**, si dovrebbe garantire che:

- `N` sia _sempre_ uguale a `entries_.size()`;
- i valori delle variabili corrispondenti alle somme parziali siano _sempre_
  allineati con la serie di valori contenuti in `entries_`.

Pertanto, preservare tutti questi dati membro renderebbe l'informazione nella
parte privata di `Sample` altamente ridondante, generando una classe
_difficile da mantenere_.

D'altro canto, rimuovere tutte variabili relative alle somme parziali ci
imporrebbe di calcolarle a partire da
`entries_`, direttamente dentro al metodo `statistics()`, il che avrebbe
verosimilmente impatto sulla sua performance.

Decidiamo di optare per **la rimozione dei dati membro ridondanti,
concettualmente più semplice, quindi più facile da implementare e mantenere**.

> :question: Seguendo questo approccio, non abbiamo in realtà alcuna
> invariante di classe da verificare, potremmo quindi decidere di utilizzare
> una `struct` al posto di una `class`?
>
> Discutetene brevemente tra voi, ed eventualmente parlatene a docenti e tutor.
>
> Qualunque sia la risposta, allo scopo di illustrare alcune funzionalità del
> linguaggio, oggi procederemo mantenendo `Sample` come `class`.

Rimuoviamo quindi tutte le variabili membro di `Sample` ad eccezione di
`entries_` e procediamo con le modifiche necessarie a `statistics.cpp`.

Ad esempio, `add` diventa semplicemente:

```c++
...
  void add(double x) {
    entries_.push_back(x);
  }
...
```

mentre è necessario ricalcolare le somme all'interno di `statistics`:

```c++
Statistics statistics() const {
  ...
  double sum_x{};
  double sum_x2{};

  for (auto const& e : entries_) {
    sum_x += e;
    sum_x2 += e * e;
  }
    ...
}
```

L'implementazione dei cambiamenti è volutamente lasciata in uno stato parziale.

**Completate voi le modifiche** e verificate che il programma compili
e funzioni come deve prima di procedere.

> :exclamation: In questo caso particolare non abbiamo modificato per nulla
> l'interfaccia della classe `Sample` e non abbiamo aggiunto nessuna nuova
> funzionalità, pertanto non è stato necessario aggiungere né modificare
> nessun test in `statistics.cpp`.

### Rimozione di un punto già inserito

Come discusso poco sopra, la prima funzionalità aggiuntiva che vogliamo
implementare è permettere di rimuovere un valore, qualora questo risulti
già presente nel nostro campione.

> :exclamation: Notate che, qualora un valore risultasse inserito più volte
> (es.: se fossero state inserite due misure il cui valore è `2.4`) quello
> vogliamo fare è rimuovere una singola misura relativa a quel valore.

Seguiamo pertanto l'approccio utilizzato nel laboratorio precedente, definendo
l'_interfaccia_ tramite la quale vogliamo rendere possibile la rimozione, ed
andiamo ad **implementare i test prima di scrivere il codice** che la effettui.

Aggiungiamo quindi alla fine della nostra lista di test il seguente
_code snippet_:

```c++
TEST_CASE("Testing the class handling a floating point data sample") {
  Sample sample;

  REQUIRE(sample.size() == 0);

  ...

  SUBCASE("Removing an existing point") {
    sample.add(1.5);
    sample.add(2.0);
    CHECK(sample.remove(2.0) == true);
    CHECK(sample.size() == 1);
  }

  SUBCASE("Removing a non existing point") {
    sample.add(1.5);
    sample.add(2.0);
    CHECK(sample.remove(1.9) == false);
    CHECK(sample.size() == 2);
  }
}
```

Implicitamente, l'implementazione proposta suggerisce di aggiungere alla classe
`Sample` una funzione membro `remove` che accetta come argomento il valore che
si vuole rimuovere e restituisce un booleano che indica se la rimozione è
avvenuta:

```c++
...
    bool remove(double x) { }
...
```

Controllando la documentazione (fatelo ogni volta che avete un dubbio!), potete
verificare che `std::vector<>` fornisce un metodo `erase`, il quale accetta come
argomento il `const_iterator` relativo all'elemento del vettore di cui si
vuole effettuare la rimozione.

Pertanto vi suggeriamo di iniziare l'implementazione della funzione partendo da
un loop realizzato tramite iteratori:

```c++
...
    bool remove(double x) {
      for (auto it = entries_.begin(), end = entries_.end(); it != end; ++it) {
        ...
      }
    ...
    }
...
```

Completate la modifica procedendo fintanto che il codice non compila ed i test
risultano tutti eseguiti con successo.

> :exclamation: Ricordate che, dopo una chiamata al metodo `erase`, l'iteratore
> `it` viene invalidato, pertanto non sarà possibile utilizzarlo nemmeno per
> l'operazione di incremento. Questo va considerato durante l'implementazione
> di `remove`.

### Aggiungere il calcolo della mediana

Dato un campione di valori ordinabili, la [mediana](https://it.wikipedia.org/wiki/Mediana_(statistica))
(o valore mediano) si definisce come il valore per il quale la frequenza relativa
cumulata vale (o supera) 0.5.

All'atto pratico, la mediana di un campione costituito da $N$ valori è
calcolabile nel seguente modo:

1. si ordinano gli $N$ valori in ordine crescente;
1. se $N$ è dispari la mediana corrisponde al valore centrale, che occupa la
   posizione $(N+1)/2$ del campione ordinato;
1. se $N$ è pari la mediana è definita come la media aritmetica dei
   valori che occupano le posizioni $(N)/2$ ed $(N)/2 + 1$.

Al fine di implementare il calcolo della mediana procediamo innanzitutto
aggiungendo una ulteriore variabile di tipo `double` alla struct `Statistics`:

```c++
...
struct Statistics {
  double mean{};
  double sigma{};
  double mean_err{};
  double median{};
};
...
```

Andiamo poi a modificare **tutti** i `TEST_CASE` (e gli eventuali `SUBCASE`) in
cui viene verificata la correttezza dei calcoli effettuati dal metodo
`Sample::statistics()`:

```c++
...
  SUBCASE("Calling statistics() with two points") {
    sample.add(1.0);
    sample.add(2.0);
    auto result = sample.statistics();
    CHECK(result.mean == doctest::Approx(1.5));
    CHECK(result.sigma == doctest::Approx(0.70710678));
    CHECK(result.mean_err == doctest::Approx(0.5));
    CHECK(result.median == doctest::Approx(1.5));
  }
  ...
```

Infine modifichiamo il metodo `Sample::statistics()` in modo che il codice
risulti sintatticamente corretto:

```c++
...
Statistics statistics() const {

    ...

    const double mean = sum_x / N;
    const double sigma = std::sqrt((sum_x2 - N * mean * mean) / (N - 1));
    const double mean_err = sigma / std::sqrt(N);

    const double median{};

    return {mean, sigma, mean_err, median};
  }
...
```

Proviamo quindi a compilarlo e ad eseguirlo:

```bash
$ g++ -Wall -Wextra statistics.cpp -o statistics 
$ ./statistics 
[doctest] doctest version is "2.4.7"
[doctest] run with "--help" for options
===============================================================================
statistics.cpp:53:
TEST CASE:  Testing the class handling a floating point data sample
  Calling statistics() with two points

statistics.cpp:82: ERROR: CHECK( result.median == doctest::Approx(1.5) ) is NOT correct!
  values: CHECK( 0.0 == Approx( 1.5 ) )

===============================================================================
statistics.cpp:53:
TEST CASE:  Testing the class handling a floating point data sample
  Calling statistics() with two identical points

statistics.cpp:92: ERROR: CHECK( result.median == doctest::Approx(10.0) ) is NOT correct!
  values: CHECK( 0.0 == Approx( 10.0 ) )

===============================================================================
statistics.cpp:53:
TEST CASE:  Testing the class handling a floating point data sample
  Calling statistics() with five points

statistics.cpp:105: ERROR: CHECK( result.median == doctest::Approx(2.5) ) is NOT correct!
  values: CHECK( 0.0 == Approx( 2.5 ) )

===============================================================================
[doctest] test cases:  1 |  0 passed | 1 failed | 0 skipped
[doctest] assertions: 23 | 20 passed | 3 failed |
[doctest] Status: FAILURE!
```

Ovviamente i test sul calcolo della mediana falliscono, ma abbiamo escluso (o
corretto), eventuali errori di sintassi prima di addentrarci
nell'implementazione dell'algoritmo descritto poc'anzi.

Il primo passo del calcolo consiste nell'ordinare i valori contenuti nello
`std::vector<double>` relativo al nostro campione di dati.

> :exclamation: Sebbene sia dispendioso, vi suggeriamo di effettuare una copia
> del vettore `entries_` (es.: `auto entries{entries_}`) e ordinare quella.
>
> :question: Riuscite ad intuire il perché?
>
> :warning: Discuteremo brevemente il costo, in termini di
> prestazioni, di scelte come questa in uno dei prossimi laboratori.

Prima di procedere è opportuno porsi la seguente domanda:

> :question: Siccome l'implementazione di un algoritmo di ordinamento non è
> completamente banale, esiste _qualcosa_ all'interno della _standard library_
> del _C++_ che potrebbe aiutarci?

La risposta si trova consultando la documentazione: l'algoritmo
[`std::sort`](https://en.cppreference.com/w/cpp/algorithm/sort) fa proprio
quello che vogliamo.

Partendo da queste considerazioni, procedete quindi ad implementare voi il
calcolo della mediana (e a testarlo).

> :exclamation: In generale, quando ci serve effettuare una operazione "comune"
> è **sempre** buona norma verificare se esista già qualche funzionalità della
> `standard library` (o di qualsiasi libreria che stiamo utilizzando nel
> nostro progetto) che assolva tale compito.

## Overloading dell'operatore somma

In alcuni casi potrebbe risultare utile combinare (o "sommare") tra loro diverse
istanze di `Sample`.

Ad esempio, si potrebbe decidere che diverse campagne di raccolta dati di un
medesimo esperimento vadano analizzate assieme (es: per ridurre le incertezze
statistiche), oppure si potrebbero generare separatamente diversi processi fisici
(es: un segnale ed una distribuzione di rumore di fondo), per poi combinarli al
fine di simulare la risposta "prevista" per un determinato strumento di misura.

### Versione 1: operatore somma come funzione libera

Iniziamo implementando l'operatore somma come funzione libera.

Come sempre, la prima cosa da fare è implementare tutti i test che riteniamo
necessari, ad esempio:

```c++
...
  SUBCASE("Testing sum operator") {
    sample.add(1);
    Sample sample_two;
    sample_two.add(2);
    auto sum = sample + sample_two;
    CHECK(sum.size() == 2);
    if (sum.size() == 2) {
      CHECK(sum.entries()[0] == doctest::Approx(1.0));
      CHECK(sum.entries()[1] == doctest::Approx(2.0));
    }
  }
...
```

Nella nostra implementazione della classe `Sample` il vettore `entries_` è
dichiarato come variabile membro `private`, pertanto non possiamo accedervi
direttamente.

Scrivendo il test abbiamo quindi notato che, oltre alla funzione libera
`operator+`, ci serve aggiungere un metodo che ci permetta di leggere i valori
contenuti all'interno di un'istanza di `Sample`, ad esempio:

```c++
class Sample {
...
 public:
...
  const auto& entries() const { return entries_; }
...
}
```

Nell'implementazione appena proposta possiamo notare che:

1. il metodo `entries` restituisce una _const reference_ al vettore `entries_`;
2. il metodo `entries` è dichiarato come _const_.

Questa scelta ci permette di evitare di effettuare una copia di `entries_` ogni
volta che viene effettuata una chiamata a `entries` ma, al contempo, permette
di garantire che attraverso questa funzione il membro privato `entries_` non
venga mai modificato.

> :exclamation: Notate come, in questo caso, entrambi i `const` vadano aggiunti
> assieme.
> Provate a rimuoverne un `const` alla volta e compilate per verificare cosa
> succede.

Procediamo quindi implementando una versione preliminare di `operator+`, che
permetta di compilare con successo:

```c++
Sample operator+(const Sample& l, const Sample& r) { return Sample{}; }
```

Come potrete notare, rimangono comunque un paio di `warning`, ed il test appena
aggiunto fallisce:

```bash
$ g++ -Wall -Wextra statistics.cpp -o statistics 
statistics.cpp: In function 'Sample operator+(const Sample&, const Sample&)':
statistics.cpp:66:32: warning: unused parameter 'l' [-Wunused-parameter]
   66 | Sample operator+(const Sample& l, const Sample& r) { return Sample{}; }
      |                  ~~~~~~~~~~~~~~^
statistics.cpp:66:49: warning: unused parameter 'r' [-Wunused-parameter]
   66 | Sample operator+(const Sample& l, const Sample& r) { return Sample{}; }
      |                                   ~~~~~~~~~~~~~~^
$ ./statistics                                                         
[doctest] doctest version is "2.4.7"
[doctest] run with "--help" for options
===============================================================================
statistics.cpp:68:
TEST CASE:  Testing the class handling a floating point data sample
  Testing sum operator

statistics.cpp:134: ERROR: CHECK( sum.size() == 2 ) is NOT correct!
  values: CHECK( 0 == 2 )

===============================================================================
[doctest] test cases:  1 |  0 passed | 1 failed | 0 skipped
[doctest] assertions: 27 | 26 passed | 1 failed |
[doctest] Status: FAILURE!
```

In teoria, i metodi `add` e `entries` sarebbero sufficienti per implementare
una versione correttamente funzionante di `operator +`.

> :exclamation: Potete provare a farlo, se arrivate a questo punto
> dell'esercizio abbastanza presto.

Però, rispetto ad aggiungere uno ad uno tutti gli elementi sfruttando il metodo
`Sample::add`,
risulterebbe più efficiente e chiaro costruire il vettore di `entries_` relativo
al risultato di `operator+` utilizzando il metodo `std::vector<>::insert`.

Purtroppo, se proviamo a farlo:

```c++
Sample operator+(const Sample& l, const Sample& r) {
  Sample sum{l};

  std::vector<double> & sum_v{sum.entries()};
  sum_v.insert(sum_v.end(), r.entries().begin(), r.entries().end());

  return sum;
}
```

incontriamo un errore di compilazione:

```bash
$ g++-12 -Wall -Wextra statistics.cpp -o statistics 
statistics.cpp: In function 'Sample operator+(const Sample&, const Sample&)':
statistics.cpp:69:44: error: binding reference of type 'std::vector<double>&' to 'const std::vector<double>' discards qualifiers
   69 |   std::vector<double> & sum_v{sum.entries()};
      |                                            ^
```

Tale errore è dovuto al fatto che `sum.entries()` restituisce una referenza
`const` al vettore `entries_` di `sum`, ma a noi servirebbe una referenza non
constante, visto che vogliamo aggiungere valori.

Come possiamo risolvere questo problema?

La soluzione sta nell'aggiungere un secondo metodo `Sample::entries`, che però
restituisce una referenza non costante.

Seguendo il suggerimento, risolvete il problema e verificate la correttezza del
risultato relativo alla somma eseguendo i test.

> :question: Perché serve scrivere due versioni di `Sample::entries`?
> Non sarebbe bastato scriverne una che non utilizza referenze a `entries_`?
> Se non intuite la cause di questa scelta provate a modificare il codice e
> controllate il risultato dei test.

### Versione 2: operatore somma a partire dall'operatore membro `+=`

Come discusso a lezione, è possibile implementare un generico operatore binario
simmetrico `operator@` (definito come funzione libera) in termini di un operatore
`@=` che sia metodo della classe per la quale cui vogliamo implementare
`operator@`.

Nel nostro caso, si  tratta di aggiungere a `Sample`:

```c++
class Sample {
  ...
  Sample& operator+=(const Sample& r) {
      ...
  }
};
```

E di semplificare `operator+` in modo che faccia uso di `operator+=`.

Lasciamo a voi l'onere dell'implementazione, ma ricordate che:

- `operator+=` è un metodo di `Sample`, pertanto può accedere alle sue variabili
  membro private;
- `operator+=` intende modificare l'istanza della classe su cui opera, pertanto
  potete avvalervi di `this` quando restituite il risultato della funzione.

Dopo le modifiche compilate il codice ed eseguite i test fino a quanto il
programma funziona correttamente.

Alla fine dei turni del quarto laboratorio, potrete trovare la soluzione
dell'esercizio [al seguente link](soluzioni/statistics.cpp).

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
[questo link](), il
quale prevede il solo caricamento di file `.zip` o `.tgz`.

Supponendo che tutto il materiale sia nella cartella `lab4` (e supponendo di
trovarsi in tale cartella), per creare un archivio `.zip` procedere come segue:

```bash
$ pwd
/home/battilan/pf_labs/lab4
$ cd ..
$ zip -r lab4.zip lab4
$ ls
lab4 lab4.zip
```

Per creare un archivio `.tgz` procedere invece come segue:

```bash
$ pwd
/home/battilan/pf_labs/lab4
$ cd ..
$ tar czvf lab4.tgz lab4
$ ls
lab4 lab4.tgz
```
