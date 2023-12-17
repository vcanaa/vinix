    ; Example of setting up the PIT for 1000 Hz frequency
    mov al, 0x36         ; Command to set PIT to repeating mode
    out 0x43, al         ; Send command to PIT control port
    mov ax, 1193         ; Frequency divisor (1193180 Hz / 1000 Hz)
    out 0x40, al         ; Send low byte of frequency divisor
    mov al, ah
    out 0x40, al         ; Send high byte of frequency divisor
; WIP