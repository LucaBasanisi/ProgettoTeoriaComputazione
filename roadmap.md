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

### FASE 1: Fondamenti di Memoria (Puntatori, Array e Stringhe)

Prima di toccare il KMP, facciamo piccoli esercizi mirati su questi concetti:

- [x] **Passo 1.1 — I Puntatori senza paura**
  - Operatore `&` (dammi l'indirizzo) e operatore `*` (dammi il valore a quell'indirizzo).
  - Passaggio di parametri per valore vs per riferimento (come modificare una variabile passata a una funzione).

- [x] **Passo 1.2 — Le Stringhe in C (`char*`)**
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

Ora applichiamo le nozioni di base all'algoritmo KMP, esplorando le proprietà combinatorie più affascinanti della stringologia.

- [ ] **Passo 2.1 — La Teoria della Failure Function ($\pi$) e i Bordi di una Stringa**
  - Definizione formale: cos'è un *bordo* ($u$ è sia prefisso proprio che suffisso proprio di $w$).
  - Perché la tabella $\pi$ memorizza esattamente la lunghezza del bordo massimale per ogni prefisso.
  - Esercizio su carta: calcolo manuale della tabella per `"ABAABABA"` e `"AAAAAA"`.

- [ ] **Passo 2.2 — Prima funzione algoritmica in C e Proprietà dell'Induzione**
  - Progettare la firma della funzione e il dilemma di memoria:
    ```c
    void kmp_compute_pi(const char *pattern, int *pi);
    ```
    *(Chi alloca l'array `pi`? Il chiamante o la funzione? Discuteremo i pro e contro di entrambe le scelte: gestione stack vs heap, ownership del puntatore).*
  - Implementazione riga per riga: il salto a ritroso induttivo `k = pi[k - 1]` (dimostrazione intuitiva del perché non si fanno passi a vuoto).

- [ ] **Passo 2.3 — Algoritmic Curiosity: Riconoscere Periodi e Ripetizioni con $\pi$**
  - *La magia matematica*: sapevi che la tabella $\pi$ ci rivela all'istante se una stringa è periodica (es. `"abcabcabc"`) in tempo $O(m)$?
  - Teorema del Periodo: se $m \bmod (m - \pi[m-1]) == 0$, la stringa è composta da copie ripetute di un blocco!
  - Scrivere una micro-funzione `bool is_periodic(const char *pattern, int *period_length)`.

- [ ] **Passo 2.4 — Ottimizzazione Teorica: La Tabella $\pi$ Ristretta / Ottimizzata ($\pi^*$)**
  - Il difetto nascosto del KMP classico: se facciamo mismatch su un carattere e indietreggiamo a un punto del pattern che ha *lo stesso identico carattere*, faremo un altro mismatch garantito al 100%!
  - Costruzione della versione ottimizzata $\pi^*$ (detta anche *next table* o failure function forte) che salta direttamente oltre i mismatch ridondanti.
  - Confronto empirico: quanti confronti inutili risparmia $\pi^*$ rispetto a $\pi$?

- [ ] **Passo 2.5 — Test, Asserzioni e Profiling con AddressSanitizer**
  - Scrivere una suite di micro-test con asserzioni (`assert.h`).
  - Verificare casi patologici: stringhe con un solo carattere ripetuto (`"AAAAA"`), pattern senza bordi (`"ABCDE"`), stringhe palindrome (`"ABACABA"`).
  - Verifica assenza di leak con AddressSanitizer/Valgrind.

---

### FASE 3: Knuth-Morris-Pratt (Parte 2: La Ricerca nel Testo)

- [ ] **Passo 3.1 — Creare le nostre prime `struct` in C**
  - In Java useresti una classe `SearchResult` o un `record`. In C usiamo una `struct`:
    ```c
    typedef struct {
        int *occurrences;    // Array dinamico di indici trovati
        int count;           // Quante occorrenze
        int capacity;        // Capacità allocata
        int comparisons;     // Numero di confronti eseguiti
    } SearchResult;
    ```
  - Pattern idiomatico di allocazione/deallocazione in C (il costruttore e distruttore manuale):
    ```c
    SearchResult* search_result_create(void);
    void search_result_free(SearchResult *result);
    ```

- [ ] **Passo 3.2 — Array Dinamici da Zero: Implementare un `std::vector` / `ArrayList` con `realloc`**
  - Come fa Java a fare `list.add()` sotto il cofano?
  - Gestione dinamica dell'array `occurrences`: quando `count == capacity`, raddoppiamo la memoria con `realloc`.
  - Gestione sicura di `realloc`: evitare il memory leak catastrofico se `realloc` ritorna `NULL`.

- [ ] **Passo 3.3 — Implementazione del Matching KMP in Tempo Reale ($O(n)$)**
  - Algoritmo di scansione del testo con puntatore $i$ che **non torna mai indietro** (ideale per elaborare stream di dati e file enormi senza rileggerli!).
  - Conteggio del numero esatto di confronti carattere per carattere.

- [ ] **Passo 3.4 — La Sfida Teorica: KMP vs Naive (Worst-Case Showdown)**
  - Progettare un generatore di "casi peggiori" (adversarial input) in cui l'algoritmo Naive ($O(n \cdot m)$) collassa e si inchioda, mentre KMP viaggia fluido in $O(n + m)$.
  - Esempio classico: Testo `"AAAAAAAA...A"` (100.000 'A') contro Pattern `"AAAA...B"`.
  - Misurazione dei tempi di esecuzione e del numero di comparazioni: tocca con mano perché KMP è geniale.

- [ ] **Passo 3.5 — Test sui casi limite e Match Sovrapposti**
  - Pattern non presente nel testo.
  - Pattern più lungo del testo.
  - Match multipli e parzialmente sovrapposti (es. cercare `"ABA"` in `"ABABA"` deve restituire indici 0 e 2).
  - Test finale di memoria con Valgrind: zero bytes persi.

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
