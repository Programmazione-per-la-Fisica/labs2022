# Laboratorio 6

In questo laboratorio metteremo in pratica le nozioni apprese durante il corso
introduttivo su _Git_. Inoltre, utilizzeremo come base di
partenza la soluzione dell'esercizio proposto nel
[quinto laboratorio](../lab5/README.md) per impratichirci con la generazione
di numeri (pseudo)casuali e la lettura/scrittura da/su file.

---

- [Laboratorio 6](#laboratorio-6)
  - [Area di lavoro](#area-di-lavoro)
  - [Configurazione iniziale di Git](#configurazione-iniziale-di-git)
  - [Introduzione a Git](#introduzione-a-git)
    - [Creare una _repository_](#creare-una-repository)
    - [Ispezionare lo stato della _repository_](#ispezionare-lo-stato-della-repository)
    - [Aggiungere file alla _repository_](#aggiungere-file-alla-repository)
    - [Aggiungere `.gitignore`](#aggiungere-gitignore)
    - [Controllare le differenze tra due commit](#controllare-le-differenze-tra-due-commit)
  - [Apportare modifiche al progetto](#apportare-modifiche-al-progetto)
    - [Generazione di numeri pseudocasuali](#generazione-di-numeri-pseudocasuali)
    - [Test delle prestazioni del codice](#test-delle-prestazioni-del-codice)
      - [Il comando `time`](#il-comando-time)
      - [Un tentativo di ottimizzazione](#un-tentativo-di-ottimizzazione)
    - [Lettura/scrittura da/su file](#letturascrittura-dasu-file)
  - [Consegna facoltativa del lavoro svolto](#consegna-facoltativa-del-lavoro-svolto)
  - [Approfondimenti ed esercizi](#approfondimenti-ed-esercizi)
    - [Generazione di valori tramite `std::generate_n`](#generazione-di-valori-tramite-stdgenerate_n)
    - [Un _main program_ che utilizza le nuove funzionalità](#un-main-program-che-utilizza-le-nuove-funzionalità)
    - [Stampa a schermo di un istogramma](#stampa-a-schermo-di-un-istogramma)

---

## Area di lavoro

Creiamo una nuova directory di lavoro (ad esempio `pf_labs/lab6`) e aggiungiamo
il file `.clang-format`. Possiamo copiarlo dalla cartella utilizzata per il primo
laboratorio, o generarlo ex-novo.

> :exclamation: Per esempio, potremmo procedere in modo analogo a quanto fatto
> [qui](../lab1/README.md#area-di-lavoro).

In seguito scarichiamo il file
[`doctest.h`](https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h)
tramite il seguente comando:

```bash
$ pwd
/home/fferrari/pf_labs/lab6
$ curl https://raw.githubusercontent.com/doctest/doctest/master/doctest/doctest.h -o doctest.h
```

Andiamo poi a copiare nell'area di lavoro il programma sviluppato durante il
quinto laboratorio.

Siete liberi di scegliere se effettuare una copia del codice che voi stessi
avete sviluppato, o scaricare la soluzione da noi proposta tramite i comandi:

```bash
$ pwd
/home/fferrari/pf_labs/lab6
$ curl https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2022/main/lab5/soluzioni/statistics.hpp -o statistics.hpp
$ curl https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2022/main/lab5/soluzioni/statistics.cpp -o statistics.cpp
$ curl https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2022/main/lab5/soluzioni/statistics.test.cpp -o statistics.test.cpp
$ curl https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2022/main/lab5/soluzioni/CMakeLists.txt -o CMakeLists.txt
```

> :exclamation: Per ora, all'interno del file `CMakeLists.txt`, commentate la
> riga relativa alla compilazione di `main.cpp`.

Utilizziamo quindi comandi illustrati durante
[il quinto laboratorio](../lab5/README.md#compilazione-tramite-cmake) per
verificare la compilazione ed eseguire i test.

Una volta fatto, andiamo a rimuovere la cartella `build` che abbiamo utilizzato
per questa prova.

## Configurazione iniziale di Git

Git dovrebbe essere già installato nei nostri setup di Linux/WSL e macOS, per
prima cosa verifichiamolo:

```bash
$ git --version
git version 2.40.0
```

Una volta effettuata la verifica, procediamo configurando Git per riconoscere chi
siamo quando, d'ora in poi, aggiungeremo file alle _repository_ (o _repo_) con
le quali lavoreremo.

Scegliamo in seguito di utilizzare `main` come nome del ramo (_branch_)
principale di sviluppo invece di `master`.

Infine, impostiamo `nano` (strumento _leggero_ e facile da utilizzare) come
editor di default per Git.

Le informazioni sulla configurazione di Git vengono salvate in un file chiamato
`.gitconfig` che si trova nella cartella home.
Possiamo ispezionarlo tramite il comando `cat`.

```bash
$ git config --global user.email "fabio.ferrari@cern.ch"
$ git config --global user.name "Fabio Ferrari"
$ git config --global init.defaultBranch "main"
$ git config --global core.editor "nano"
$ cat ~/.gitconfig
[user]
        email = fabio.ferrari@cern.ch
        name = Fabio Ferrari
[init]
       defaultBranch = main
[core]
        editor = nano
...
```

## Introduzione a Git

### Creare una _repository_

A questo punto, inizializziamo la _repository_ Git e verifichiamo che venga
aggiunta una cartella nascosta (`.git`) all'interno della nostra area di lavoro:

```bash
$ pwd
/home/fferrari/pf_labs/lab6/
$ git init
Initialized empty Git repository in /home/fferrari/pf_labs/lab6/.git/
$ ls -la
drwxr-xr-x  9 fferrari  staff   288 13 Apr 10:54 .git
-rw-r--r--  1 fferrari  staff  1156 13 Apr 10:31 .clang-format
-rw-r--r--  1 fferrari  staff  1156 13 Apr 10:31 CMakeLists.txt
-rw-r--r--  1 fferrari  staff  1156 13 Apr 10:31 doctest.h
-rw-r--r--  1 fferrari  staff  2454 13 Apr 10:31 statistics.cpp
-rw-r--r--  1 fferrari  staff   632 13 Apr 10:31 statistics.hpp
-rw-r--r--  1 fferrari  staff  2211 13 Apr 10:31 statistics.test.cpp
```

> :exclamation: La cartella nascosta `.git` contiene le informazioni relative
> alla _repository_ che abbiamo appena inizializzato.

### Ispezionare lo stato della _repository_

I due comandi che tipicamente vengono usati per ispezionare lo stato di una repo
sono `git status` e `git log`, spesso accompagnati da una o più delle diverse
opzioni disponibili.

Proviamoli:

```bash
$ git status
On branch main

No commits yet

Untracked files:
  (use "git add <file>..." to include in what will be committed)
        .clang-format
        CMakeLists.txt
        doctest.h
        statistics.cpp
        statistics.hpp
        statistics.test.cpp

nothing added to commit but untracked files present (use "git add" to track)
```

```bash
$ git log
fatal: your current branch 'main' does not have any commits yet
```

Come possiamo notare, il fatto che i file del nostro progetto fossero già
presenti al momento di creazione della repo, non li rende automaticamente parte
di essa. I file infatti risultano essere _Untracked_, cioè non tracciati dalla repo.

Inoltre, siccome non abbiamo ancora eseguito nessun commit, risulta inutile
ispezionare la storia della nostra repo con `git log`, che (in questo caso)
produce un errore.

### Aggiungere file alla _repository_

Aggiungiamo quindi i file relativi al progetto nella nostra repo:

```bash
$ git add .clang-format doctest.h statistics.hpp statistics.cpp statistics.test.cpp CMakeLists.txt
$ git status
On branch main

No commits yet

Changes to be committed:
  (use "git rm --cached <file>..." to unstage)
        new file:   .clang-format
        new file:   CMakeLists.txt
        new file:   doctest.h
        new file:   statistics.cpp
        new file:   statistics.hpp
        new file:   statistics.test.cpp
```

```bash
$ git commit -m "Add first project version"
[main (root-commit) cdef02d] Add first project version
 6 files changed, 7602 insertions(+)
 create mode 100644 .clang-format
 create mode 100644 CMakeLists.txt
 create mode 100644 doctest.h
 create mode 100644 statistics.cpp
 create mode 100644 statistics.hpp
 create mode 100644 statistics.test.cpp
 ```

Come possiamo notare, dopo aver eseguito il comando `git add` ci viene ricordata
la lista dei file che verranno aggiunti alla repository al commit successivo,
come il secondo comando (appunto `git commit`) conferma.

Verifichiamo nuovamente lo stato della repo:

```bash
$ git status
On branch main
nothing to commit, working tree clean
```

```bash
$ git log
commit cdef02d5b43217353c72eb393a20cdaa13007585 (HEAD -> main)
Author: Fabio Ferrari <fabio.ferrari@cern.ch>
Date:   Thu Apr 13 11:08:41 2023 +0200

    Add first project version
```

A questo punto, il comando `git status` ci informa che la nostra area di lavoro
non mostra modifiche rispetto all'ultima versione del codice nel branch `main`
della repo.

Il comando `git log`, invece evidenzia la presenza di un singolo commit (nel
branch `main`) e ne fornisce alcuni dettagli.

> :exclamation: In caso la versione di Git che risulta installata sulla vostra
> macchina sia inferiore alla `2.28`, il parametro di configurazione
> `init.defaultBranch "main"` non sortirà alcun effetto, e dovreste osservare
> che il vostro branch di lavoro si chiama `master`.
>
> Se riscontrate questo piccolo problema, rinominate il branch tramite il
> comando seguente (e verificate che il cambio sia avvenuto come atteso eseguendo
> ancora una volta il comando `git status`):
>
> ```bash
> $ git branch -m master main
> $ git status
> On branch main
> 
> No commits yet
> 
> nothing to commit (create/copy files and use "git add" to track)
> ```

### Aggiungere `.gitignore`

Procediamo quindi a compilare il progetto ed eseguire i test (per ora
utilizziamo la modalità di debug specificando: `-DCMAKE_BUILD_TYPE=Debug`):

```bash
$ cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug                                                                          
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
-- Configuring done (1.5s)
-- Generating done (0.0s)
-- Build files have been written to: /home/fferrari/pf_labs/lab6/build

```

```bash
$ cmake --build build
[ 33%] Building CXX object CMakeFiles/statistics.t.dir/statistics.test.cpp.o
[ 66%] Building CXX object CMakeFiles/statistics.t.dir/statistics.cpp.o
[100%] Linking CXX executable statistics.t
[100%] Built target statistics.t
```

```bash
$ cmake --build build --target test                                                               
Running tests...
Test project /home/fferrari/pf_labs/lab6/build
    Start 1: statistics.t
1/1 Test #1: statistics.t .....................   Passed    0.16 sec

100% tests passed, 0 tests failed out of 1

Total Test time (real) =   0.16 sec
```

Per poi verificare di nuovo lo stato della repo:

```bash
$ git status
On branch main

Untracked files:
  (use "git add <file>..." to include in what will be committed)
        build/

no changes added to commit (use "git add" and/or "git commit -a")
```

Sicuramente non vogliamo tracciare (cioè, inserire nella nostra _repository_) la
cartella `build/`, in primis perché contiene file binari, ma soprattutto perché è
generata. Ha poco senso tracciare qualcosa che può essere generato a partire da
altri file tracciati.

Il modo **per evitare che Git ci ricordi continuamente che abbiamo file non
tracciati** è quello di **indicare esplicitamente la lista dei file che non
vogliamo includere** nella nostra repo all'interno di un file chiamato
`.gitignore`. Potete inserire il `.gitignore` all'interno della directory di lavoro, in questo caso `/home/fferrari/pf_labs/lab6/`.

Andiamo quindi ad aggiungere la singola riga `build/` al file `.gitignore` e
salviamolo.

Verifichiamone poi il contenuto con il comando `cat`:

```bash
$ code .gitignore
$ cat .gitignore
build/
```

Controlliamo come cambia l'output del comando `git status`:

```bash
$ git status

git status     

On branch main
Untracked files:
  (use "git add <file>..." to include in what will be committed)
        .gitignore

no changes added to commit (use "git add" and/or "git commit -a")
```

Infine procediamo ad eseguire l'`add` ed il `commit` del file `.gitignore`:

```bash
$ git add .gitignore

$ git commit -m "Add .gitignore"
[main 0b1a157] Add .gitignore
 1 file changed, 1 insertion(+)
 create mode 100644 .gitignore
 ```

Controlliamo poi, ancora una volta, lo stato della repo:

```bash
$ git status
On branch main
nothing to commit, working tree clean
$ git log
commit 0b1a157e19bdc565e9664418cc3a78663462e2d4 (HEAD -> main)
Author: Fabio Ferrari <fabio.ferrari@cern.ch>
Date:   Thu Apr 13 11:21:29 2023 +0200

    Add .gitignore

commit cdef02d5b43217353c72eb393a20cdaa13007585
Author: Fabio Ferrari <fabio.ferrari@cern.ch>
Date:   Thu Apr 13 11:08:41 2023 +0200

    Add first project version
```

> :exclamation: `git log` mostra in alto nuove informazioni, lasciando
> inalterate quelle precedenti.
>
> :exclamation: Allo stesso tempo le etichette `HEAD` e `main` sono state
> spostate in corrispondenza dell'ultimo commit.

### Controllare le differenze tra due commit

A volte è preferibile una versione più compatta dell'output di `git log`, che
possiamo ottenere, ad esempio, aggiungendo l'opzione `--oneline`:

```bash
$ git log --oneline
0b1a157 (HEAD -> main) Add .gitignore
cdef02d Add first project version
```

Possiamo notare che ogni riga di output del comando precedente mostra gli
_hash_ dei singoli commit, che possiamo ad esempio utilizzare per verificare la
differenza tra due commit qualsiasi, usando `git diff`:

```bash
$ git diff 0b1a157 cdef02d
diff --git a/.gitignore b/.gitignore
deleted file mode 100644
index d163863..0000000
--- a/.gitignore
+++ /dev/null
@@ -1 +0,0 @@
-build/
```

## Apportare modifiche al progetto

A questo punto, andiamo ad apportare modifiche al progetto, ed utilizziamo Git
per tracciarne l'evoluzione.

Sarebbe possibile procedere con lo sviluppo eseguendo ogni commit relativo ad
eventuali cambiamenti direttamente nel branch `main`, il che ci permetterebbe
già di tenere traccia di tutte le modifiche apportate al codice, così da poter
"viaggiare nel tempo" e ripristinare eventualmente versioni precedenti del
progetto.

Nella pratica, però, quando collaboriamo a progetti che sono condivisi con altre
persone (siano essi semplici utilizzatori, o sviluppatori coi quali
lavoriamo), torna spesso utile la capacità di Git di gestire linee di sviluppo
indipendenti, che si svolgono su _branch_ separati, ed unirle
(farne il _merge_, o _mergiarle_), in un secondo tempo.

Pertanto procederemo ad effettuare le modifiche del nostro codice in un nuovo
branch, che chiameremo `add_random`.

Possiamo creare il nuovo branch e spostarci su di esso eseguendo il comando:

```bash
$ git checkout -b add_random
Switched to a new branch 'add_random'
```

> :exclamation: Il comando `git checkout -b add_random` è equivalente all
> coppia di comandi:
>
> ```bash
> $ git branch add_random
> $ git checkout add_random
> Switched to a new branch 'add_random'
> ```

Possiamo controllare il successo dell'operazione tramite i comandi:

```bash
$ git status
On branch add_random
nothing to commit, working tree clean
$ git log --oneline
0b1a157 (HEAD -> add_random, main) Add .gitignore
cdef02d Add first project version
```

Ora stiamo lavorando sul branch `add_random`, che, per il momento,
risulta essere identico al branch `main`.

### Generazione di numeri pseudocasuali

Passiamo adesso ad esercitarci sulla generazione di numeri _random_. Come
mostrato a lezione, questo può essere fatto tramite lo
`std::default_random_engine`, che è parte della
[_random number library_](https://en.cppreference.com/w/cpp/numeric/random).

Andiamo ad aggiungere al progetto il file `main.cpp` nel quale istanziamo un
oggetto di tipo `Sample` e lo `std::default_random_engine` che sarà utilizzato
per la generazione dei numeri pseudocasuali:

```c++
#include "statistics.hpp"

#include <random>

int main() {
    pf::Sample data{};
    std::default_random_engine eng;
}    
```

Supponiamo di voler generare un insieme di numeri di tipo `double` distribuito
secondo una Gaussiana con media `0.0` e deviazione standard pari a `1.0`.
 Sarà sufficiente aggiungere un oggetto `dist` di tipo
 [`std::normal_distribution<double>`](https://en.cppreference.com/w/cpp/numeric/random/normal_distribution):

```c++
...
int main() {
    pf::Sample data{};
    std::default_random_engine eng;
    std::normal_distribution<double> dist;
}    
```

Che possiamo utilizzare nel seguente modo per aggiungere dati al nostro
campione `data` (di tipo `Sample`):

```c++
...
int main() {
    pf::Sample data{};
    std::default_random_engine eng;
    std::normal_distribution<double> dist;
    data.add(dist(eng));
    data.add(dist(eng));
    data.add(dist(eng));
}    
```

> :exclamation: Il `default_random_engine` può essere utilizzato per più di una
> generazione, quindi non è necessario dichiararne uno nuovo per ogni
> generazione.

Aggiungiamo infine una stampa a schermo delle statistiche calcolate tramite il
metodo `Sample::statistics`, come ad esempio la media e la deviazione standard:

```c++
#include "statistics.hpp"

#include <random>
#include <iostream>

int main() {
    pf::Sample data{};
    std::default_random_engine eng;
    std::normal_distribution<double> dist;
    data.add(dist(eng));
    data.add(dist(eng));
    data.add(dist(eng));

    const auto stat{data.statistics()};
    std::cout << "- mean: " << stat.mean << '\n'
              << "- sigma: " << stat.sigma << '\n';
}    
```

Andiamo ad abilitare di nuovo la compilazione del _main program_ in
`CMakeLists.txt` configuriamo l'area build, compiliamo ed seguiamo il codice:

```bash
...
$ ./build/statistics
- mean: -0.0117328
- sigma: 1.46648
```

Potrete notare che, utilizzando un campione di dati così, limitato non si
ottengono esattamente i valori attesi.

> :question: Provate a eseguire il codice più volte senza cambiarlo. Ottenete
> sempre lo stesso output su schermo? Come mai?

In realtà, la sequenza di numeri generata da un qualsiasi _random number
generator_ è completamente deterministica, per cui ci attendiamo il
comportamento appena ottenuto.

È possibile variare la risposta di un _random number generator_ andando ad
utilizzare _seed_ differenti durante l'inizializzazione.

In particolare,
[`std::random_device`](https://en.cppreference.com/w/cpp/numeric/random/random_device)
permette tipicamente di accedere ad alcune risorse _hardware_ per generare
sequenze di numeri interi "veramente" casuali distribuite in maniera uniforme.

Però, **l'accesso a tali risorse risulta dispendioso**, per tanto è prassi
utilizzare `std::random_device` solo per inizializzare il _seed_ di un _random
number generator (o engine)_.

Andiamo quindi a modificare il programma nel seguente modo:

```c++
...
int main() {
    pf::Sample data{};
    std::random_device r;
    std::default_random_engine eng(r());
    ...
}
```

Quindi compiliamo ed seguiamo il codice più volte senza cambiarlo.

> :question: Come cambia il comportamento del programma rispetto a prima?

Ovviamente l'approccio illustrato sopra non è utilizzabile per generare grandi
quantità di dati. Possiamo inserire il comando `add` all'interno di un ciclo
_for_ per ovviare a questo problema.

Proviamo adesso a generare un milione di eventi, compiliamo ed eseguiamo di
nuovo il codice.

> :question: Avete ottenuto dei valori delle statistiche del campione più vicine
> alle vostre attese?

Una volta verificato che il programma si comporti correttamente, controlliamo le
modifiche che stiamo per committare tramite `git diff`:

``` bash
$ git diff
diff --git a/CMakeLists.txt b/CMakeLists.txt
index 60d819b..c66d6cc 100644
--- a/CMakeLists.txt
+++ b/CMakeLists.txt
@@ -18,7 +18,7 @@ string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")
 string(APPEND CMAKE_CXX_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")
 string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")
 
-# add_executable(statistics main.cpp statistics.cpp)
+add_executable(statistics main.cpp statistics.cpp)
 
...
```

tale comando, eseguito senza argomenti, mostra la differenza tra la nostra area
di lavoro e lo stato del branch in cui stiamo lavorando.

> :exclamation: Possiamo scorrere la lista delle differenze tramite i tasti
> freccia, qualora risulti troppo lunga per essere visualizzata nel terminale
> tutta insieme.

Procediamo poi includendo i cambiamenti nel branch `add_random`:

```bash
$ git add main.cpp CMakeLists.txt
$ git commit -m "Add main using random numbers generation"
[add_random bd314b5] Add main using random numbers generation
 2 files changed, 18 insertions(+), 1 deletion(-)
 create mode 100644 main.cpp
$ git status                 
On branch add_random
nothing to commit, working tree clean
```

Poi ritorniamo al branch `main` ed effettuiamo il merge:

```bash
$ git log --oneline   
bd314b5 (HEAD -> add_random) Add main using random numbers generation
0b1a157 (main) Add gitignore
cdef02d Add first project version
```

```bash
$ git checkout main
Switched to branch 'main'
$ git merge --no-ff add_random
Merge made by the 'ort' strategy.
 CMakeLists.txt |  2 +-
 main.cpp       | 17 +++++++++++++++++
 2 files changed, 18 insertions(+), 1 deletion(-)
 create mode 100644 main.cpp
```

> :exclamation: Il comando `git merge` andrà ad aprire una sessione di `nano`,
> tramite la quale potremo andare a gestire il messaggio relativo al merge del
> branch `add_random` in `main`.
> Una volta soddisfatti, possiamo salvare il messaggio e chiudere `nano`
> utilizzando le seguenti combinazioni di tasti: <kbd>Ctrl</kbd>+<kbd>O</kbd> e
> <kbd>Ctrl</kbd>+<kbd>X</kbd>.

```bash
$ git log --oneline --graph         
*   0b17eb6 (HEAD -> main) Merge branch 'add_random'
|\  
| * bd314b5 (add_random) Add main using random numbers generation
|/  
* 0b1a157 Add gitignore
* cdef02d Add first project version
```

Possiamo notare alcune cose:

1. è necessario eseguire il comando `git add` prima del commit anche quando si
   modificano file già inclusi nella repo;
2. `git log` inizialmente ci mostra che `add_random` si discosta di un
   commit rispetto a `main`;
3. dopo il commit, tramite `git log --oneline --graph`, risulta evidente
   l'evoluzione del progetto (incluso il merge).

> :exclamation: dopo essere ritornati sul branch `main`, ma prima di effettuare
> il merge, potete (se volete) ispezionare i file `CMakeLists.txt` per verificare
> che fosse ritornato com'erano prima di introdurre le modifiche relative alla
> compilazione del main program.
>
> :exclamation: a volte i cambiamenti effettuati in un branch separato (es:
> `add_random`) sono lineari rispetto a quelli del branch in cui viene
> mergiato (es.: `main`), ovvero non ci sono sviluppi concorrenti.
> In questi casi, Git può decidere di "dimenticarsi completamente"
> dell'esistenza del branch separato, ed aggiungere tutta la storia dei commit
> effettuati direttamente all'interno del branch in cui questo viene integrato.
> L'opzione `--no-ff`, utilizzata in questo caso durante il merge, previene
> questo comportamento. L'abbiamo usata a fini esemplificativi, per rendere
> evidente lo sviluppo in un branch separato quanto abbiamo controllato
> l'evoluzione della repo con `git log --oneline --graph`, molto spesso tale
> opzione non viene utilizzata, lasciando decidere a Git come comportarsi.

In questo caso "semplice" il merge non ha dato _conflitti_. Quando si effettuano
sviluppi concorrenti sui medesimi file, a volte, è necessario intervenire
manualmente durante il merge, nel caso Git non riesca a combinarli in modo
automatico.

A questo punto, i cambiamenti implementati nel branch `add_random` sono stati
integrati nel `main` branch.

Pertanto possiamo decidere di fare pulizia, rimuovendo il branch utilizzato per
lo sviluppo:

```bash
$ git branch
  add_random
* main
$ git branch -d add_random
Deleted branch add_random (was bd314b5).
```

```bash
$ git log --oneline --graph 
*   0b17eb6 (HEAD -> main) Merge branch 'add_random'
|\  
| * bd314b5 Add main using random numbers generation
|/  
* 0b1a157 Add gitignore
* cdef02d Add first project version
```

> :question: Notate una differenza tra questa versione dell'output del comando
> `git log --oneline --graph` e quella precedente?

### Test delle prestazioni del codice

Per valutare le prestazioni del nostro codice potrebbe essere interessante
misurarne il tempo di esecuzione. Vediamo di seguito un metodo comunemente
usato per farlo.

> :exclamation: Ricordarsi di compilare il codice in _release_ mode
>
> ```bash
> $ cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
> $ cmake --build build
> ```

#### Il comando `time`

 Un modo molto semplice per fare ciò è utilizzare il comando `time`.
 Proviamo ad eseguirlo, anteponendolo al comando di esecuzione del nostro
 eseguibile:

```bash
$ time ./build/statistics
- mean: -0.00150972
- sigma: 1.00086

real 0m0.405s
user 0m0.356s
sys 0m0.024s
```

#### Un tentativo di ottimizzazione

Come potete vedere, sono comparse tre misure di tempo al di sotto del normale
output del nostro programma: `real`, `user` e `sys`. Quella più interessante
per i nostri scopi è `real`, cioè il tempo trascorso da quando abbiamo lanciato
il comando di esecuzione del codice a quando il nostro programma è terminato.

> :question: Provate ad aumentare la quantità di dati generati di un fattore
> 10. Il tempo indicato da `real` aumenta all'incirca nel modo atteso?

La domanda che possiamo farci adesso è se il nostro codice sia ottimizzato per
maneggiare grandi quantità di dati e, in quali sue parti, il codice spenda più
tempo durante l'esecuzione.

Probabilmente, la più grossa fetta di tempo di esecuzione è
spesa dal comando `sort` all'interno della funzione `statistics`.

> :exclamation: Come potremmo cercare di evitare di ordinare un vettore
> potenzialmente molto grande? Cercate di ragionare su questa domanda prima di
> continuare l'esercizio.

Una possibile soluzione è quella di inserire i dati nel vettore di modo che
siano già ordinati. Ovviamente, la singola chiamata alla funzione `add`
diventerà più lenta, ma questo potrebbe permetterci di guadagnare comunque
tempo alla fine. Per fare ciò possiamo utilizzare la funzione
[`std::lower_bound`](https://en.cppreference.com/w/cpp/algorithm/lower_bound).

**Prima di procedere** andiamo a creare un nuovo branch di sviluppo, come
descritto poco sopra:

```bash
$ git checkout -b sorted_add
Switched to a new branch 'sorted_add'
$ git status
On branch sorted_add
nothing to commit, working tree clean
```

Modifichiamo quindi la funzione `void Sample::add(double x)` nel file
`statistics.cpp` per fare in modo che quando viene aggiunto un numero al
campione di dati, esso venga già inserito nella posizione giusta per mantenere
un ordine crescente all'interno del vettore.

```c++
void Sample::add(double x) { 
  ... 
}
```

Lasciamo a voi l'implementazione di questa modifica.

> :exclamation: Ricordatevi di eliminare a questo punto il `sort` all'interno
> del metodo `statistics` della classe `Sample`, in quanto adesso è superfluo.

Compiliamo, **verifichiamo che i test siano soddisfatti** ed eseguiamo il codice.
Misuriamo ancora il tempo di esecuzione tramite il comando `time`.

> :question: Cosa potete notare? È stato vantaggioso passare dall'ordinamento
> del vettore alla fine degli inserimenti all'ordinamento del vettore per ogni
> inserimento?
>
> :question: L'implementazione degli algoritmi della _Standard Library_ è più
> che soddisfacente in diverse circostanze, quindi, prima di provare a
> migliorare la performance dei propri programmi tramite metodi "non standard"
> è opportuno verificare quali siano i loro punti "colli di bottiglia" per
> assicurarsi che una miglioria sia "veramente necessaria" (specialmente qualora
> vada a peggiorare la leggibilità del codice).
>
> :exclamation: Se non aveste controllato il tempo di esecuzione, probabilmente
> con un numero di eventi generato basso non vi sareste accorti della differenza
> tra le due versioni del programma. Ricordatevi di riflettere sempre sulle
> vostre scelte implementative, cercando di procedere evitando operazioni
> inutili.

Una volta stabilito che il nostro ramo di sviluppo risulta fallimentare,
possiamo tornare tranquillamente al nostro branch `main` che rimane
correttamente funzionante (e con prestazioni migliori).

Come prima cosa eseguiamo il commit del codice che abbiamo sviluppato nel branch
`sorted_add`:

```bash
$ git add statistics.cpp
$ git commit -m "Implement sorted add using std::lower_bound"
...
```

```bash
$ git checkout main
Switched to branch 'main'
```

Controllate lo stato del codice, compilate, eseguite i test e misurate il tempo
di esecuzione del programma.

A questo punto, possiamo decidere se cancellare il ramo di sviluppo "morto" o
tornarci in seguito per tentare ulteriori modifiche.

Qualora optiate per la rimozione, verificate che il comando:
`git branch -d sorted_add` in questo caso fallisce.

> :question: Perché Git si comporta in modo diverso dal caso precedente? Vi
> sembra che la differenza sia giustificata?
>
> :exclamation: Leggendo con attenzione il messaggio d'errore sarete in grado di
> rispondere alle domande appena poste e di capire come procedere (se intendete
> farlo!) per completare la rimozione del branch `sorted_add`.

### Lettura/scrittura da/su file

Supponiamo adesso di avere un campione di dati salvato su file e di volerlo
leggere per processarlo tramite la classe `Sample`. Possiamo utilizzare 
`std::ifstream is{"filename"}` per leggere da file dei valori, dove `"filename"`
rappresenta il _path_ del file che vogliamo leggere.

> :exclamation: Ricordatevi di includere l'header file `fstream` per poter
> usare sia `std::ifstream` che `std::ofstream`.
>
> :question: È buona norma controllare che l'apertura del file in lettura sia
> avvenuta con successo. Come fareste?

Procedete a scaricare il file `data.txt` da questo link:

```bash
$ curl https://raw.githubusercontent.com/Programmazione-per-la-Fisica/labs2022/main/lab6/samples/data.txt -o data.txt

```

Procedete poi a leggerlo all'interno del vostro _main program_.

```c++
int main() {
    pf::Sample data{};
    std::ifstream in_file("data.txt") 
    ...
    }    
```

La lettura del file viene deliberatamente non implementata e viene lasciata a voi
come esercizio.

Salvate, compilate ed eseguite il codice.

> :exclamation: Se avete letto correttamente il file dovreste ottenere come
> valori di media, deviazione standard, errore sulla media e mediana: 3.00005,
> 2.00171, 0.00200171 e 3.00038.

Procediamo adesso a scrivere i risultati ottenuti dal metodo `statistics()` in
un file di output di nome `results.txt`. Ricordate che per scrivere su file
occorre usare `std::ofstream out_file{"filename"}`, dove `"filename"` rappresenta
il _path_ del file che vogliamo produrre.

> :question: È buona norma controllare che l'apertura del file in scrittura sia
> avvenuta con successo. Come fareste?

Alla fine dei turni del sesto laboratorio, potrete trovare la soluzione
dell'esercizio proposto, completato risolvendo anche gli approfondimenti
facoltativi [al seguente link](soluzioni/).

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

Supponendo che tutto il materiale sia nella cartella `lab6` (e supponendo di
trovarsi in tale cartella), per creare un archivio `.zip` procedere come segue:

```bash
$ pwd
/home/fferrari/pf_labs/lab6
$ cd ..
$ zip -r lab6.zip lab6
$ ls
lab5 lab6.zip
```

Per creare un archivio `.tgz` procedere invece come segue:

```bash
$ pwd
/home/fferrari/pf_labs/lab6
$ cd ..
$ tar czvf lab6.tgz lab6
$ ls
lab5 lab6.tgz
```

Alternativamente, **chi ha deciso di utilizzare [GitHub](https://github.com/)**,
può inviare ai docenti il link ad una _repository_ remota contenente il lavoro
svolto.

## Approfondimenti ed esercizi

Per chi fosse interessato a "sperimentare ulteriormente" gli argomenti
presentati in questo laboratorio, vengono proposti alcuni possibili
approfondimenti **opzionali**.

Qualora decidiate di procedere con gli approfondimenti, **vi suggeriamo** in
primo luogo **di utilizzare Git** durante tutta la fase di sviluppo.

### Generazione di valori tramite `std::generate_n`

Provate ad utilizzare l'algoritmo
[`std::generate_n`](https://en.cppreference.com/w/cpp/algorithm/generate_n) al
posto di un ciclo _for_ per effettuare la generazione di numeri pseudocasuali.

> :exclamation: Un paio di suggerimenti per l'implementazione:
>
> - potete utilizzare la funzione
>   [`std::back_inserter`](https://en.cppreference.com/w/cpp/iterator/back_inserter)
>   per aggiungere valori al campione;
> - per farlo, in primis, la classe `Sample` deve includere un metodo
>   `Sample::push_back`;
> - in secondo luogo, il tipo della variabile di cui fare `push_back` deve
>   essere definito tramite l'alias `Sample::value_type`.

### Un _main program_ che utilizza le nuove funzionalità

Provate ad aggiungere un nuovo file `main.cpp` contenente una funzione `main`
che usa i componenti definiti sopra.

Il programma accetta dall'utente comandi:

- `g N_ENTRIES`: per generare un campione random di dati contenente `N_ENTRIES`
  estrazioni;
- `r FILE_NAME`: per leggere un campione di dati da un file di nome `FILE_NAME`
  dato come input;
- `s` per eseguire il calcolo delle statistiche e stamparle a schermo.
- `o FILE_NAME` per eseguire il calcolo delle statistiche e scriverle su un file
  di output chiamato `FILE_NAME`.
- `q` per uscire dal programma.

Qualora vengano immessi comandi sconosciuti, il programma deve menzionarlo,
per poi richiedere di nuovo l'inserimento di un comando valido.

Ricordatevi inoltre di gestire eventuali eccezioni e di controllare la
correttezza formale dell'input da tastiera.

### Stampa a schermo di un istogramma

Scrivete un metodo `void Sample::histogram(int n_bins, double norm)` che stampi
su schermo un istogramma orizzontale con `n_bins` intervalli contenente i dati
generati o letti. Il parametro `norm` rappresenta un fattore di scala che vi
servirà per ridurre l'area dell'istogramma e renderlo cosi visualizzabile
comodamente su qualsiasi schermo.
