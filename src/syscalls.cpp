/**
 * @file syscalls.cpp
 * @brief Implementări și stub-uri pentru apeluri de sistem POSIX pe bare-metal.
 *
 * Librăria standard a toolkitului ARM necesită aceste apeluri de sistem.
 * Deoarece aplicația rulează pe hardware bare-metal (fără sistem de operare),
 * implementările sunt fie minimale, fie stub-uri fără funcționalitate reală.
 */

#include "utils/lifesource.hpp"

extern "C" {

/**
 * @brief Implementare a funcției de ieșire — bucla infinită pe bare-metal.
 * @param status Codul de ieșire (ignorat).
 */
void _exit(int status) {
    while (1) { }
}

/**
 * @brief Extinde heap-ul programului (necesar pentru malloc/new).
 *
 * Utilizează simbolul @c __heap_start__ definit în linker script pentru
 * a gestiona manual memoria heap.
 *
 * @param incr Numărul de octeți cu care se extinde heap-ul.
 * @return Pointer la începutul zonei nou alocate.
 */
char* _sbrk(int incr) {
    extern char __heap_start__;
    static char* heap_end = &__heap_start__;
    char* prev_heap = heap_end;
    heap_end += incr;
    return prev_heap;
}

/**
 * @brief Stub pentru scrierea la un descriptor de fișier (printf/cout).
 *
 * Poate fi implementat pentru a transmite date prin UART.
 *
 * @param file Descriptorul de fișier (ignorat).
 * @param ptr Pointer la datele de scris.
 * @param len Numărul de octeți de scris.
 * @return Numărul de octeți "scrisi" (egal cu @p len).
 */
int _write(int file, char* ptr, int len) {
    return len;
}

/**
 * @brief Stub pentru citirea de la un descriptor de fișier (scanf/getchar).
 *
 * Nu citește nimic în implementarea curentă.
 *
 * @param file Descriptorul de fișier (ignorat).
 * @param ptr Bufferul de destinație.
 * @param len Numărul maxim de octeți de citit.
 * @return 0 (nicio dată citită).
 */
int _read(int file, char* ptr, int len) {
    return 0;
}

/**
 * @brief Stub pentru închiderea unui descriptor de fișier.
 * @param file Descriptorul de fișier de închis.
 * @return -1 (operațiune nesuportată).
 */
int _close(int file) { return -1; }

/**
 * @brief Stub pentru obținerea stării unui descriptor de fișier.
 * @param file Descriptorul de fișier.
 * @param st Pointer la structura de stare.
 * @return 0.
 */
int _fstat(int file, void* st) { return 0; }

/**
 * @brief Stub pentru verificarea dacă un descriptor de fișier este un terminal.
 * @param file Descriptorul de fișier.
 * @return 1 (considerat terminal).
 */
int _isatty(int file) { return 1; }

/**
 * @brief Stub pentru deplasarea poziției curente într-un fișier.
 * @param file Descriptorul de fișier.
 * @param offset Deplasamentul față de origine.
 * @param whence Originea deplasamentului.
 * @return 0.
 */
int _lseek(int file, int offset, int whence) { return 0; }

/**
 * @brief Stub pentru trimiterea unui semnal unui proces.
 * @param pid ID-ul procesului.
 * @param sig Semnalul de trimis.
 * @return -1 (operațiune nesuportată pe bare-metal).
 */
int _kill(int pid, int sig) { return -1; }

/**
 * @brief Stub pentru obținerea ID-ului procesului curent.
 * @return 1 (ID fix pe bare-metal).
 */
int _getpid() { return 1; }

}
