# 🗺️ Roadmap di Progetto: StringLab (C Moderno con String_View da Java/Python)

> **Obiettivo**: Costruire un'applicazione CLI e una libreria in C che implementa gli algoritmi classici di string matching (KMP, Automa a Stati Finiti, Suffix Array), basata sull'idioma moderno delle **`String_View`**. Una vera **palestra per imparare il C moderno**, la gestione della memoria, i puntatori, le struct e l'architettura del software.

---

## 🎓 Metodo Didattico: Da Java/Python a C Moderno

In Java e Python ci sono le classi `String`, i metodi `.substring()`, la garbage collection e la gestione automatica delle dimensioni. In C tradizionale si usavano puntatori `char*` e stringhe terminate da `\0`, spesso fonte di bug e memory leak.

Con l'approccio **C Moderno (alla Tsoding / Andrei Alexandrescu / C++20 `std::string_view`)**, introduciamo fin da subito la struttura:
```c
typedef struct {
    const char *data;  // Puntatore ai byte (senza possesso / non-owning)
    size_t count;      // Lunghezza esatta in O(1)
} String_View;
```
Questo ci permette di:
- Fare slicing e sottostringhe in tempo $O(1)$ e **con 0 allocazioni**.
- Non dover chiamare continuamente `strlen()` nei nostri algoritmi.
- Rendere le firme delle funzioni eleganti, sicure e compatte.

---

## 🧭 Mappa Mentale: Da Java a C Moderno

| Concetto in Java | Equivalente in C Tradizionale | Equivalente in C Moderno (`String_View`) |
| :--- | :--- | :--- |
| `String s = "ciao";` | `char s[] = "ciao";` (`\0` alla fine) | `String_View sv = sv_from_cstr("ciao");` (dati + count) |
| `s.length()` | `strlen(s)` ($O(n)$, scorre fino a `\0`) | `sv.count` ($O(1)$, lettura immediata) |
| `s.substring(from, to)` | `malloc` + `strncpy` ($O(k)$ e memory leak risk) | `sv_sub(sv, from, count)` ($O(1)$, zero copie) |
| `new Oggetto()` | `malloc(sizeof(Oggetto))` | `malloc(sizeof(Oggetto))` (con ownership esplicita) |
| `throw new Exception()` | Codici di errore (`NULL`, `-1`) | Valori di ritorno con struct / booleani |
| Package / Classi | `.h` (interfaccia) e `.c` (implementazione) | Moduli chiari: `string_view.h`, `kmp.h`, ecc. |

---

### FASE 1: Fondamenti di Memoria e la Prima Struttura (`String_View`)

- [x] **Passo 1.1 — I Puntatori senza paura**
  - Operatore `&` (indirizzo) e operatore `*` (dereferenziazione).
  - Passaggio di parametri per riferimento (la funzione `swap`).

- [x] **Passo 1.2 — Le Stringhe C (`char*`) e il carattere `\0`**
  - Perché il C tradizionale ha bisogno di `\0`.
  - Implementazione manuale di `my_strlen`.

- [x] **Passo 1.3 — Allocazione Dinamica (Stack vs Heap)**
  - Stack (automatico, locale) vs Heap (`malloc` / `free`).
  - La regola d'oro: ownership della memoria e annullamento dangling pointer (`ptr = NULL`).

- [x] **Passo 1.4 — La prima Struct Moderna: `String_View`**
  - Definizione della struct `String_View` (`data` e `count`).
  - Funzioni base:
    - `sv_from_cstr(const char *cstr)`
    - `sv_from_parts(const char *data, size_t count)`
    - `sv_sub(String_View sv, size_t start, size_t count)`
    - `sv_equals(String_View a, String_View b)`
  - Stampa formattata con `printf("%.*s", (int)sv.count, sv.data)`.

---

### FASE 2: Knuth-Morris-Pratt (Parte 1: Preprocessing e Tabella π con `String_View`)

- [x] **Passo 2.1 — Teoria della Failure Function ($\pi$) e i Bordi di una Stringa**
  - Definizione formale di *bordo* (prefisso e suffisso proprio comune).
  - Perché $\pi$ traccia la lunghezza del massimo bordo per ogni prefisso.
  - Esercizio su carta: calcolo manuale per `"ABABAC"` e `"AAAAAA"`.

- [ ] **Passo 2.2 — Calcolo della Tabella $\pi$ su `String_View`**
  - Firma della funzione:
    ```c
    void kmp_compute_pi(String_View pattern, int *pi);
    ```
  - Accesso diretto: `pattern.count` evita qualsiasi `strlen`, caratteri accessibili con `pattern.data[i]`.
  - Implementazione riga per riga del salto a ritroso: `k = pi[k - 1]`.

- [ ] **Passo 2.3 — Proprietà Combinatorie: Riconoscere Periodi con $\pi$**
  - Teorema del Periodo con `String_View`:
    ```c
    bool sv_is_periodic(String_View sv, size_t *period_len);
    ```
  - Se `m % (m - pi[m - 1]) == 0`, estrarre la radice periodica come `String_View` in $O(1)$!

- [ ] **Passo 2.4 — Tabella Ottimizzata $\pi^*$ (Strong Failure Function)**
  - Evitare confronti inutili su caratteri duplicati al mismatch.
  - Costruzione di $\pi^*$ e misurazione del numero di salti risparmiati.

- [ ] **Passo 2.5 — Test di Robustezza e Modularità**
  - Separazione del codice: creazione di `string_view.h`, `kmp.h` e relativi `.c`.
  - Test con asserzioni (`assert.h`) su casi limite (stringhe vuote, mono-carattere, palindrome).

---

### FASE 3: Knuth-Morris-Pratt (Parte 2: La Ricerca nel Testo)

- [ ] **Passo 3.1 — Struct di Risultato con `String_View`**
  - Progettazione del risultato di ricerca:
    ```c
    typedef struct {
        size_t *matches;      // Array dinamico di offset nel testo
        size_t count;
        size_t capacity;
        size_t comparisons;   // Numero di confronti eseguiti
    } MatchResult;
    ```
  - Costruttore e distruttore con gestione memoria: `match_result_create`, `match_result_free`.

- [ ] **Passo 3.2 — Array Dinamico Idiomatico (stile `std::vector` / `da_append`)**
  - Implementazione della crescita dinamica con `realloc` e fattore di scala $\times 2$.

- [ ] **Passo 3.3 — Algoritmo KMP Matcher completo**
  - Firma:
    ```c
    MatchResult kmp_search(String_View text, String_View pattern);
    ```
  - Scansione a sola andata del testo senza backtrack.
  - Slicing istantaneo: per ogni match all'indice `idx`, estrarre la finestra corrispondente con `sv_sub(text, idx, pattern.count)`.

- [ ] **Passo 3.4 — Benchmark: KMP vs Naive**
  - Generazione di casi adversarial (peggiori): Testo `"AAAA...A"`, Pattern `"AAA...B"`.
  - Misurazione del tempo e confronto del numero di comparazioni.

---

### FASE 4: Visualizzatore CLI Interattivo

- [ ] **Passo 4.1 — Lettura da tastiera con `String_View`**
  - Lettura sicura con `fgets` e conversione immediata del buffer in `String_View` (rimuovendo il newline `\n` in $O(1)$).

- [ ] **Passo 4.2 — Render grafico del matching a terminale**
  - Visualizzare il cursore nel testo e il pattern allineato sotto.
  - Evidenziare con colori ANSI o caratteri ASCII la finestra matchata tramite `%.*s`.

---

### FASE 5: Automa a Stati Finiti (DFA) su `String_View`

- [ ] **Passo 5.1 — Tabella di Transizione $\delta$**
  - Modello dell'automa: stati da $0$ a $m$, con $m = pattern.count$.
  - Costruzione della matrice di transizione per l'alfabeto.

- [ ] **Passo 5.2 — Esecuzione DFA sul Testo**
  - Firma:
    ```c
    MatchResult dfa_search(String_View text, DFA *dfa);
    ```
  - Zero confronti al mismatch durante la scansione: una sola lookup nella matrice per ogni carattere di `text`.

- [ ] **Passo 5.3 — Confronto: Preprocessing vs Throughput**
  - KMP (preprocessing $O(m)$ leggerissimo) vs DFA (preprocessing $O(m \cdot |\Sigma|)$, ma scansione pura a stati).

---

### FASE 6: Suffix Array (Il trionfo delle `String_View`)

- [ ] **Passo 6.1 — Suffissi come Array di `String_View`**
  - Perché le `String_View` sono perfette per il Suffix Array: ogni suffisso di un testo è solo uno spostamento di puntatore!
  - Nessuna duplicazione di testo, zero byte di memoria sprecati per i suffissi.

- [ ] **Passo 6.2 — Ordinamento con `qsort` e comparazione di `String_View`**
  - Funzione di comparazione lessicografica `sv_compare(String_View a, String_View b)`.
  - Ordinamento dell'array di indici/views con `qsort`.

- [ ] **Passo 6.3 — Ricerca Binaria del Pattern nel Suffix Array**
  - Trovare il pattern in $O(m \log n)$ confrontando la `String_View` del pattern con i suffissi ordinati.

---

## 🛠️ Regola Fondamentale per Ogni Sessione

1. Zero compiler warning con `-Wall -Wextra`.
2. Zero memory leak o accessi fuori limite.
3. Ownership della memoria chiarissima: chi alloca deve deallocare; le `String_View` non possiedono memoria, guardano solo!
