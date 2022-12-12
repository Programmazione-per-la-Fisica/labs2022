# Laboratorio 3

In questa lezione inizieremo ad apprendere come utilizzare i meccanismi di
astrazione del _C++_ costruendo semplici classi e strutture.

---

- [Laboratorio 3](#laboratorio-3)
  - [Area di lavoro](#area-di-lavoro)
  - [Calcolo di grandezze statistiche relative ad un campione di dati](#calcolo-di-grandezze-statistiche-relative-ad-un-campione-di-dati)
    - [Definizione della classe `Sample`](#definizione-della-classe-sample)
    - [Implementazione della classe `Sample`](#implementazione-della-classe-sample)
    - [Identificazione dei metodi _const_](#identificazione-dei-metodi-const)
    - [Aggiungere altri test e funzionalità](#aggiungere-altri-test-e-funzionalità)
    - [Migliorare il calcolo della deviazione standard](#migliorare-il-calcolo-della-deviazione-standard)
    - [Utilizzo di `SUBCASE`](#utilizzo-di-subcase)
  - [Consegna facoltativa del lavoro svolto](#consegna-facoltativa-del-lavoro-svolto)
  - [Approfondimenti ed esercizi](#approfondimenti-ed-esercizi)

---

L'obiettivo principale di questo laboratorio è imparare i rudimenti dell'uso di
`class` e le `struct`.

Per farlo, realizzeremo una `class` (`Sample`) che, una volta inseriti i dati
relativi ad un determinato campione, permette di ricavarne alcune grandezze
statistiche significative, le quali verranno a loro volta raccolte in una
`struct` (`Statistics`).

Durante il laboratorio vi invitiamo a tenere aperte le
[slide](https://github.com/Programmazione-per-la-Fisica/pf2022/releases/latest)
presentate a lezione.

## Area di lavoro

Creiamo una nuova directory di lavoro (ad esempio `pf_labs/lab3`) e aggiungiamo
il file `.clang-format`. Possiamo copiarlo dalla cartella utilizzata per il primo
laboratorio, o generarlo ex-novo.

> :exclamation: Per esempio, potete procedere in modo analogo a quanto fatto
> [qui](../lab1/README.md#area-di-lavoro).

In seguito scarichiamo il file
[`doctest.h`](https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h)
tramite il seguente comando:

```bash
$ pwd
/home/fferrari/pf_labs/lab3
$ curl https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h -o doctest.h
```

Fatto ciò, l'area di lavoro deve trovarsi in questo stato:

```bash
$ pwd
/home/fferrari/pf_labs/lab3
$ ls -A
.clang-format  doctest.h
```

## Calcolo di grandezze statistiche relative ad un campione di dati

Dato un adeguato campione di dati, costituito da un insieme di $N$ numeri
reali non correlati tra loro, _media_, _deviazione standard_ ed _errore sulla
media_ possono essere definiti come:

$$ \mu = \frac{1}{N} \sum_{i=1}^{N} x_i $$

$$ \sigma = \sqrt{ \frac{1}{N} \sum_{i=1}^{N} \left( \mu - x_i \right)^2} = \sqrt{\frac{\sum x_i^2}{N} - \mu^2} $$

$$ \sigma_{\mu} = \frac{\sigma}{\sqrt{N}} $$

Quello che vogliamo fare è implementare in _C++_ uno strumento che calcoli
questi valori, costruendo una classe `Sample` che permetta la gestione del
campione e restituisca le grandezze statistiche $\mu$, $\sigma$ e $\sigma_\mu$,
nascondendo i dettagli implementativi relativi alle formule utilizzate.

> :warning: Ad esempio, per il calcolo di $\sigma$, potremmo implementare una
> qualsiasi delle due formule presentate sopra, ed eventualmente decidere di
> cambiare l'implementazione in un secondo momento, senza che un ipotetico
> utilizzatore di `Sample` si accorga di nulla.

Inoltre vogliamo raggruppare tali grandezze statistiche in un'unica _struttura
dati_: `Statistics`.

### Definizione della classe `Sample`

Per risolvere il problema tramite l'uso di una classe, è opportuno cominciare
identificando le funzionalità _minime_ che questa deve garantire, così come
_l'interfaccia_ che questa espone ad un eventuale utilizzatore.

Nel nostro caso serviranno:

- un metodo `add` per inserire nuovi valori (uno alla volta per ora);
- un metodo `statistics` che ritorni i valori di $\mu$ , $\sigma$ e $\sigma_\mu$
  (calcolati utilizzando i valori inseriti fino ad un dato momento).
  
Anche in questo caso, una buona prassi è quella di cominciare implementando
alcuni test:

```c++
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("Testing the class handling a floating point data sample") {

  Sample sample;

  sample.add(1.0);
  sample.add(2.0);

  auto result = sample.statistics();
  CHECK(result.mean == doctest::Approx(1.5));
  CHECK(result.sigma == doctest::Approx(0.5));
  CHECK(result.mean_err == doctest::Approx(0.35355339));
};
```

Apriamo quindi VSCode nell'area di lavoro, creiamo un nuovo file (es:
`statistics.cpp`) ed inseriamo il frammento di codice riportato sopra:

```bash
$ pwd
/home/fferrari/pf_labs/lab3
$ code .
```

> :exclamation: se non lo state già facendo vi consigliamo di utilizzare la
> combinazione di tasti di scelta rapida associata alla creazione di un file
> vuoto in VSCode e, d'ora in poi, abituarvi a farlo con regolarità.

Proviamo a compilare il programma, e verifichiamo attentamente i messaggi
d'errore riportati.

> ```bash
> g++ statistics.cpp -Wall -Wextra -o statistics
> ```
>
> o, su macOS:
>
> ```bash
> g++-12 statistics.cpp -Wall -Wextra -o statistics
> ```

Quello che notiamo è che il compilatore si lamenta dell'assenza della classe
`Sample`.

### Implementazione della classe `Sample`

Procediamo quindi a creare tale classe, aggiungendo quanto segue al file
`statistics.cpp` **prima** del `TEST_CASE`:

```c++
...

class Sample {
};

TEST_CASE("Testing the class handling a floating point data sample") {
...
}
```

Ripetendo la compilazione (verificate!) otteniamo errori (diversi da prima)
legati alla mancanza dei metodi `add` e `statistics`.

Ovviare alla mancanza di `add` è facile, basta aggiungerlo alla classe:

```c++
..
class Sample
{
 public:
  void add(double x) {}
};
...
```

per poi compilare.

> :exclamation: Notiamo che, per `add`:
>
> - il _return type_ è `void`, in quanto il metodo non deve restituire nulla;
> - l'argomento in input è di tipo `double`;
> - l'implementazione per ora non fa nulla; il comportamento è naturalmente
>   sbagliato, ma ci permette di iniziare a definire l'interfaccia e risolve
>   l'errore di compilazione;
> - il fatto che non stiamo ancora utilizzando la variabile `x`
>   nell'implementazione di `add` genera un _warning_ (avvertimento) in fase di
>   compilazione (verificatelo).

Aggiungere il metodo `statistics` richiede maggiore attenzione, perché dobbiamo
prima definire il tipo ritornato.

Dall'uso che viene fatto di `result`, vediamo che deve avere tre dati membri
pubblici, chiamati `mean` `sigma` e `mean_err`, tutti di tipo `double`.
Procediamo quindi definendo la `struct` `Statistics`, in modo che includa quei
campi.

In _C++_ il tutto si traduce in:

```c++
...
struct Statistics {
  double mean{};
  double sigma{};
  double mean_err{};
};

class Sample {
 public:
  void add(double x) {}
  Statistics statistics() { return Statistics{}; }
};
...
```

> :exclamation: Notiamo che, `statistics`:
>
> - non ha argomenti;
> - per ora la sua implementazione restituisce una `struct` `Statistics` con i
>   campi semplicemente azzerati (ci preoccuperemo di implementare
>   correttamente le formule tra poco).

Con queste modifiche il programma compila correttamente (provate!), **ma fallisce
tutti i test**.

Pensiamo quindi a come implementare i due metodi, così che **facciano
quello che vogliamo**.

Nel caso dell'implementazione di una classe come questa, la cosa
spesso implica anche chiedersi: _cosa abbiamo bisogno di memorizzare, in termini
di variabili membro (private) della classe, per raggiungere i nostri scopi?_

Nel nostro caso, la domanda sopra si traduce in questa: _ci serve
davvero mantenere in memoria tutti i punti che inseriti tramite il metodo `add`,
per processarli quando viene chiamato il metodo `statistics`, o ci sono
alternative?_

Se si osservano attentamente le formule indicate sopra, si può notare che non
c'è bisogno di farlo, ma è sufficiente mantenere alcuni **accumulatori** di
valori:

- $N$
- $\sum x_i$
- $\sum x_i^2$

che possiamo aggiornare man mano che viene chiamato il metodo `add`. Questi
valori verranno poi opportunamente combinati nel momento in cui viene chiamato
il metodo `statistics` per calcolare media, deviazione standard ed errore sulla
media.

Come si implementa questa idea in _C++_?

```c++
...

class Sample {
  double sum_x_{};
  double sum_x2_{};
  int N_{};

 public:
  void add(double x) {
    sum_x_ += x;
    sum_x2_ += x * x;
    ++N_;
  }

  Statistics statistics() {
    double mean = ...
    double sigma = ...
    double mean_err = ...

    return {mean, sigma, mean_err};
  }
};

... 
```

> :warning: Tipicamente, si utilizzando criteri per distinguere le variabili
> membro di una classe da quelle definite nello _scope_ di un determinato
> metodo. Nel caso sopra, abbiamo introdotto un "_" alla fine del nome della
> variabile (es.: `N_`).
>
> :warning: Notiamo che le variabili membro sono costituite da tipi primitivi,
> quindi devono essere inizializzate, altrimenti il loro valore iniziale risulta
> indeterminato. Qui proponiamo di inizializzarle direttamente dove sono
> dichiarate, usando la sintassi `{}`. In questo modo sono _value-initialized_,
> che per gli `int` e i `double` significa "inizializzati a zero". In
> alternativa si potrebbe:
>
> - usare la sintassi più esplicita `int n = 0;`, `double sum_x = 0.;`, etc ...;
> - definire esplicitamente un costruttore di default, che nella sua
>   _initialization-list_ inizializzi tutti i valori a zero.
>
> Se vi rimane tempo potete provare a sperimentare tutte le alternative.

Se l'implementazione è corretta, tutti i test devono essere eseguiti con
successo.

### Identificazione dei metodi _const_

Facciamo un'ultima modifica al codice della classe, dichiarando `statistics` come
`const` in quanto **non modifica** i dati membri.

> :exclamation: In generale, è importante identificare che tutti i metodi
> della nostra classe che ci aspettiamo debbano essere `const` e specificarli
> come tali.

```c++
...
  Statistics statistics() const {
  ...
  }
...
```

Per verificare l'effetto dell'uso dell'identificatore `const` provate ad
aggiungere la seguente funzione libera prima dei `TEST_CASE`:

```c++
auto statistics(Sample const &sample) {
  return sample.statistics();
}
```

ed a compilare il programma includendo e rimuovendo l'identificatore `const`
nella dichiarazione di `statistics` all'interno della classe `Sample`.

### Aggiungere altri test e funzionalità

Ora che la classe `Sample` è implementata e soddisfa il nostro test
minimale, è opportuno aggiungere altri test e/o funzionalità.

Ricordiamo che l'obiettivo dei test non è di confermare che il programma
funzioni; al contrario, l'obiettivo è di **rompere** il codice. Cercare
attivamente potenziali _punti deboli_ e _casi particolari_ dal principio ci
permette di evidenziare e risolvere problemi sin da subito.

> :exclamation: I test definiti in questo momento vanno **ripetutamente eseguiti
> dopo ogni nuova modifica**, ciò permette di verificare che nuovi problemi non
> appaiano in futuro.

A titolo esemplificativo, cosa vorremmo che succeda se:

1. chiamiamo `statistics` senza aggiungere alcun punto?
2. chiamiamo `statistics` dopo aver aggiunto un solo punto?
3. chiamiamo `statistics` dopo aver aggiunto due punti uguali?

>:warning: Per gestire in modo appropriato il caso 1., possiamo avvalerci di
> una exception.
> Per capire come implementare test in questi casi, potete consultare la
> sezione [Exceptions](https://github.com/doctest/doctest/blob/master/doc/markdown/assertions.md#exceptions)
> della documentazione `doctest`.

Inoltre si possono aggiungere alcuni test in cui il campione contiene tre o più
punti. I calcoli si possono fare con carta e penna, si può usare
[WolframAlpha](https://www.wolframalpha.com), oppure si possono utilizzare i
dati raccolti durante le prove pratiche del corso di laboratorio di Meccanica e
Termodinamica.

### Migliorare il calcolo della deviazione standard

Come discusso durante il corso di laboratorio di Meccanica e Termodinamica, una
versione _più corretta_ per il calcolo di $\sigma$ per un campione di dati finito
è:

$$ \sigma = \sqrt{ \frac{1}{N - 1} \sum_{i=1}^{N} \left( \mu - x_i \right)^2}$$

Proviamo pertanto a modificare il calcolo di questa grandezza.

In ordine, per effettuare la modifica:

- con carta e penna, correggiamo la formula che abbiamo utilizzato, in modo da
  preservare l'approccio del calcolo basato su somme parziali;
- pensiamo a quali parti dei vari `TEST_CASE` vanno modificate (es: per quali
  vanno modificati i risultati del calcolo, e quali nuovi casi dovrebbero
  sollevare un'eccezione) e provvediamo a cambiarle;
- solo ora implementiamo la nuova forma all'interno di `statistics`.

Verifichiamo poi che il codice **compili e superi tutti i test**.

Alla fine dei turni del terzo laboratorio, potrete trovare la soluzione
dell'esercizio 3 [al seguente link](soluzioni/statistics.cpp).

### Utilizzo di `SUBCASE`

Quando si implementano più test, specialmente se sono relativi ad un'unica
classe, o insieme di funzionalità tra loro collegate, è possibile esprimerli
utilizzando i `SUBCASE`, così da partire sempre dalle stesse condizioni
iniziali.

Provate ad utilizzare i `SUBCASE` nel caso di quanto implementato oggi.

> :exclamation: Per una comprensione più dettagliata dei `SUBCASE`, potete
> consultare la [documentazione](https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md#test-cases-and-subcases).

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
[questo link](https://virtuale.unibo.it/mod/assign/view.php?id=1109468), il
quale prevede il solo caricamento di file `.zip` o `.tgz`.

Supponendo che tutto il materiale sia nella cartella `lab3` (e supponendo di
trovarsi in tale cartella), per creare un archivio `.zip` procedere come segue:

```bash
$ pwd
/home/fferrari/pf_labs/lab3
$ cd ..
$ zip -r lab3.zip lab3
$ ls
lab3 lab3.zip
```

Per creare un archivio `.tgz` procedere invece come segue:

```bash
$ pwd
/home/fferrari/pf_labs/lab3
$ cd ..
$ tar czvf lab3.tgz lab3
$ ls
lab2 lab2.tgz
```

## Approfondimenti ed esercizi

Riportiamo qui un possibile esercizio alternativo che potete risolvere
utilizzando `class` e `struct` ed applicando un approccio simile a quello che
abbiamo imparato oggi:

Create una classe `Regression` che, dato un insieme arbitrario di _N_ punti
$(x_i,y_i)$ , calcoli i coefficienti $A$ e $B$ della retta di regressione
$y = A + B\cdot x$, utilizzando le seguenti formule:

$$A = \frac {\sum y_{i} \sum x_{i}^{2} - \sum x_{i} \sum x_{i} y_{i}}{N \sum x_{i}^{2} - (\sum x_{i})^{2}}$$

$$B = \frac {N \sum x_{i}y_{i} - \sum x_{i} \sum y_{i}}{N \sum x_{i}^{2} - (\sum x_{i})^{2}}$$
