# 🗺️ Roadmap di Progetto: StringLab (Imparare il C da Java/Python)

> **Obiettivo**: Costruire un'applicazione CLI in C che implementa gli algoritmi classici di string matching (KMP, Automa a Stati Finiti, Suffix Array), utilizzandola come **palestra per imparare il C moderno**, la gestione della memoria, i puntatori e il debugging di basso livello.

---

## 🎓 Metodo Didattico: Da Java/Python a C

In Java e Python la JVM o l'interprete proteggono lo sviluppatore. In C parliamo direttamente con la memoria e la CPU. 

### I 3 Pilastri di questo percorso:
1. **Un solo concetto nuovo per volta**: non introduciamo puntatori a funzione o architetture complesse finché non maneggiamo con sicurezza `malloc`, puntatori e stringhe terminate da `\0`.
2. **Tu scrivi il codice riga per riga**: niente blocchi copia-incolla già pronti. Io ti fornirò schemi mentali, spiegazioni teoriche e scheletri minimi; tu digiterai il codice per prendere confidenza con sintassi ed errori del compilatore.
3. **Safety first (La tua rete di protezione)**: impareremo fin dal giorno 1 a usare **AddressSanitizer** e **Valgrind** per beccare memory leak e segmentation fault all'istante, invece di impazzire a cercarli dopo.

---

## 🧭 Mappa Mentale: Da Java a C

| Concetto in Java | Equivalente in C | A cosa fare attenzione |
| :--- | :--- | :--- |
| `String s = "ciao";` | `char s[] = "ciao";` oppure `const char *s = "ciao";` | È solo un array di byte con `\0` alla fine. Non ha un metodo `.length()`. |
| `new Oggetto()` | `malloc(sizeof(Oggetto))` | L'heap non si pulisce da solo: serve `free(ptr)`. |
| Riferimenti ad oggetti | **Puntatori** (`int*`, `struct Oggetto*`) | Un puntatore è solo un intero che contiene un indirizzo di memoria RAM. |
| `throw new Exception()` | Valori di ritorno (`NULL`, `-1`, codici errore) | Non ci sono eccezioni; ogni chiamata va verificata manualmente. |
| Package e `import` | File `.h` (dichiarazioni) e `#include` | Il C separa nettamente la promessa (header) dall'implementazione (`.c`). |
| Gradle / Maven | **CMake** | Descrive come compilare i sorgenti e creare gli eseguibili/librerie. |

---

## 📋 Fasi di Apprendimento e Sviluppo

---

### FASE 0: Primi Passi con l'Ambiente C (Zero Algoritmi, Solo Strumenti)

- [ ] **Passo 0.1 — Come pensa un compilatore C**
  - Differenza tra *Preprocessore* (`#include`, `#define`), *Compilatore* (`.c` $\rightarrow$ `.o`), e *Linker*.
  - Compilare a mano un semplice `main.c` con `gcc -Wall -Wextra -std=c11`.

- [ ] **Passo 0.2 — Setup di CMake e CLion**
  - Creare un `CMakeLists.txt` pulito ed essenziale.
  - Abilitare i warning massimi (`-Wall -Wextra -Wpedantic`) per farci avvisare dal compilatore al minimo errore.
  - Attivare **AddressSanitizer** (`-fsanitize=address,undefined`): lo strumento magico che ti dice esattamente a che riga hai sbagliato l'accesso alla memoria.

- [ ] **Passo 0.3 — Header (.h) vs Sorgente (.c)**
  - Perché non si scrive tutto in un file solo.
  - Capire cosa sono gli *include guards* (`#ifndef`, `#define`, `#endif`).
  - Creare una prima funzioncina divisa tra `.h` e `.c` per verificare che il linking funzioni.

---

### FASE 1: Fondamenti di Memoria (Puntatori, Array e Stringhe)

Prima di toccare il KMP, facciamo piccoli esercizi mirati su questi concetti:

- [ ] **Passo 1.1 — I Puntatori senza paura**
  - Operatore `&` (dammi l'indirizzo) e operatore `*` (dammi il valore a quell'indirizzo).
  - Passaggio di parametri per valore vs per riferimento (come modificare una variabile passata a una funzione).

- [ ] **Passo 1.2 — Le Stringhe in C (`char*`)**
  - Cos'è il carattere nullo `'\0'` e perché se lo dimentichi il programma legge memoria a caso.
  - Funzioni standard essenziali da `<string.h>`: `strlen`, `strcmp`, `strcpy`.
  - Differenza tra string literal (in sola lettura) e array di caratteri modificabili.

- [ ] **Passo 1.3 — Allocazione Dinamica (Stack vs Heap)**
  - Quando una variabile vive solo nella funzione (stack) e quando deve sopravvivere (heap).
  - Usare `malloc` per allocare un array di interi.
  - La regola d'oro: **a ogni `malloc` DEVE corrispondere una `free`**.
  - Prima prova pratica con Valgrind: provocare volutamente un memory leak e vedere come Valgrind lo segnala.

---

### FASE 2: Knuth-Morris-Pratt (Parte 1: Il Preprocessing e la Tabella π)

Ora applichiamo le nozioni di base all'algoritmo KMP.

- [ ] **Passo 2.1 — La Teoria della Failure Function ($\pi$)**
  - Ripasso concettuale: perché calcoliamo il prefisso più lungo che è anche suffisso proprio.
  - Calcolo manuale su carta della tabella per `"ABAABABA"`.

- [ ] **Passo 2.2 — Prima funzione algoritmica in C**
  - Progettare la firma della funzione:
    ```c
    void kmp_compute_pi(const char *pattern, int *pi);
    ```
    *(Nota: chi alloca l'array `pi`? Il chiamante o la funzione? Discuteremo i pro e contro di entrambe le scelte).*
  - Implementazione riga per riga di `kmp_compute_pi`.

- [ ] **Passo 2.3 — Testare la tabella $\pi$**
  - Scrivere un piccolo test in un `main` o con un framework leggero (Unity).
  - Verificare che su `"ABAABABA"` produca `[0, 0, 1, 1, 2, 3, 2, 3]`.
  - Verifica assenza di leak con AddressSanitizer/Valgrind.

---

### FASE 3: Knuth-Morris-Pratt (Parte 2: La Ricerca nel Testo)

- [ ] **Passo 3.1 — Creare le nostre prime `struct` in C**
  - In Java useresti una classe `SearchResult` o un `record`. In C usiamo una `struct`:
    ```c
    typedef struct {
        int *occurrences;    // Array dinamico di indici trovati
        int count;           // Quante occorrenze
        int comparisons;     // Numero di confronti eseguiti
    } SearchResult;
    ```
  - Scrivere la funzione per creare e soprattutto **distruggere** la struct:
    ```c
    void search_result_free(SearchResult *result);
    ```

- [ ] **Passo 3.2 — Implementazione di `kmp_search`**
  - Algoritmo di scansione del testo in $O(n)$ usando la tabella $\pi$.
  - Gestione dinamica delle occorrenze trovate (introduzione a `realloc`).
  - Conteggio del numero di confronti carattere per carattere.

- [ ] **Passo 3.3 — Test sui casi limite**
  - Pattern non presente.
  - Pattern più lungo del testo.
  - Match multipli e sovrapposti (es. `"AA"` in `"AAAA"`).

---

### FASE 4: Una CLI Semplice e Interattiva

- [ ] **Passo 4.1 — Input da tastiera in C**
  - Perché `scanf` è pericoloso per le stringhe (rischio buffer overflow).
  - Usare `fgets` per leggere in sicurezza testo e pattern da tastiera.

- [ ] **Passo 4.2 — Visualizzazione a terminale**
  - Formattare l'output con `printf`: mostrare indici, caratteri, tabella $\pi$ e occorrenze.
  - Aggiunta facoltativa: visualizzazione passo-passo del confronto con spiegazione degli shift.

---

### FASE 5: Automa a Stati Finiti (DFA per String Matching)

- [ ] **Passo 5.1 — Teoria dell'Automa a Stati Finiti**
  - Stati come lunghezza del massimo prefisso del pattern matchato finora.
  - Matrice di transizione $\delta(\text{stato}, \text{carattere})$.

- [ ] **Passo 5.2 — Matrici 2D e Puntatori a Funzione**
  - Come gestire array bidimensionali in C (`int delta[NUM_STATES][ALPHABET_SIZE]`).
  - (Facoltativo ma istruttivo) Simulare un'interfaccia Java usando **puntatori a funzione** per rendere intercambiabili KMP e Automa.

- [ ] **Passo 5.3 — Confronto empirico: KMP vs Automa**
  - Confrontare il numero di operazioni e la fase di preprocessing tra i due algoritmi sullo stesso input.

---

### FASE 6: Suffix Array (Introduzione alle Strutture Dati Avanzate)

- [ ] **Passo 6.1 — Cos'è un Suffix Array**
  - Rappresentazione compatta di tutti i suffissi di una stringa tramite un array di indici.

- [ ] **Passo 6.2 — Ordinamento con `qsort` di C**
  - Come funziona la funzione di libreria standard `qsort`.
  - Scrivere una funzione di comparazione (`int compare(const void *a, const void *b)`).

- [ ] **Passo 6.3 — Ricerca Binaria sul Suffix Array**
  - Ricerca del pattern tramite bisezione sui suffissi ordinati in $O(m \log n)$.

---

## 🛠️ Regola Fondamentale per Ogni Sessione

Prima di passare da un passo all'altro:
1. Il codice deve compilare con **zero warning**.
2. Il programma deve girare senza errori su Valgrind / AddressSanitizer (**zero memory leaks, zero accessi invalidi**).
3. Devi sentirti a tuo agio a spiegare cosa fa ogni riga di memoria che abbiamo scritto.
