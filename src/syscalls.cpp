/**
*   Implementari si stuburi pentru apeluri de sistem POSIX.
*   Libraria standard a toolkitului are nevoie de ele, am incercat sa le caut prin folder structure-ul toolkitului, n-am gasit nimic, atp e mai rapid sa le implementez eu
*/

#include "utils/lifesource.hpp"

extern "C" {

// Exit infinit
void _exit(int status) {
    while (1) { }
}


// Mai jos sunt stuburi si syscalluri generate cu chatgpt, asa ca bear with me.

// =======================
// Heap (malloc/new)
// =======================
char* _sbrk(int incr) {
    extern char __heap_start__;  // simbol definit în linker script // nu stiu ce e un linker script
    static char* heap_end = &__heap_start__;
    char* prev_heap = heap_end;
    heap_end += incr;
    return prev_heap;
}

// =======================
// Output (printf/cout)
// =======================
int _write(int file, char* ptr, int len) {
    // Poți implementa UART transmit aici
    // Deocamdată stub minimal:
    return len;
}

// =======================
// Input (scanf/getchar)
// =======================
int _read(int file, char* ptr, int len) {
    // Stub minimal: nu citește nimic
    return 0;
}

// =======================
// File operations stubs
// =======================
int _close(int file) { return -1; }
int _fstat(int file, void* st) { return 0; }
int _isatty(int file) { return 1; }
int _lseek(int file, int offset, int whence) { return 0; }

// =======================
// Process stubs (bare-metal)
// =======================
int _kill(int pid, int sig) { return -1; }
int _getpid() { return 1; }

}
