    ; Ensures that we jump straight into the kernel's entry function.
    [bits 32]       ; We're in protected mode by now, so use 32-bit instructions.
    [extern _start] ; Declare that we will be referencing the external symbol '_start',
                    ; so the linker can substitute the final address
    call    _start  ; invoke _start() in our C kernel
    jmp     $       ; Hang forever when we return from the kernel to prevent the CPU from executing random data as if it was code.

;     // Start of the video memory.
; // The screen is a 80x25 grid of characters. 2 bytes per character.
; volatile char* VIDEO_PTR = (char*)0xb8000;
    ; global VIDEO_PTR
    ; VIDEO_PTR equ 0xb8000
