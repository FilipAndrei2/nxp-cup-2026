/* ==============================================
   Linker script pentru S32K bare-metal C++
   ============================================== */

/* Definirea memoriei MCU */
MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 512K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

/* ==============================================
   Secțiuni program
   ============================================== */
SECTIONS
{
    /* Cod și date read-only în Flash */
    .text : 
    {
        KEEP(*(.isr_vector))   /* vectori ISR */
        *(.text*)              /* cod C/C++ */
        *(.rodata*)            /* date read-only */
        *(.rodata*)            /* mai multe date read-only */
    } > FLASH

    /* Variabile inițializate în RAM, încărcate din Flash */
    .data : AT(ADDR(.text) + SIZEOF(.text))
    {
        *(.data*)
    } > RAM

    /* Variabile neinițializate */
    .bss :
    {
        *(.bss*)
        *(COMMON)
    } > RAM

    /* ==========================================
       Heap și Stack
       ========================================== */
    /* Stack-ul crește în jos din sfârșitul RAM-ului */
    __stack_start__ = ORIGIN(RAM) + LENGTH(RAM);

    /* Heap-ul crește în sus din începutul RAM-ului */
    __heap_start__  = ORIGIN(RAM);
}
