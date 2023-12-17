print_string:               ; print string pointed by bx
    mov     ah, 0x0e        ; Teletype output function
    mov     al, [bx]
    cmp     al, 0
    je      print_string_end
    int     0x10
    inc     bx
    jmp     print_string
print_string_end:
    ret

print_hex:                  ; print value stored in dx in hex format
    mov     ah, 0x0e        ; Teletype output function
    mov     bl, dh          ; print byte X...
    shr     bx, 4
    and     bx, 0x000F
    add     bx, TO_HEX
    mov     al, [bx]
    int     0x10

    mov     bl, dh          ; print byte .X..
    and     bx, 0x000F
    add     bx, TO_HEX
    mov     al, [bx]
    int     0x10

    mov     bl, dl          ; print byte ..X.
    shr     bx, 4
    and     bx, 0x000F
    add     bx, TO_HEX
    mov     al, [bx]
    int     0x10

    mov     bl, dl          ; print byte ...X
    and     bx, 0x000F
    add     bx, TO_HEX
    mov     al, [bx]
    int     0x10

    ret

    ; global variables
TO_HEX:
    db '0123456789ABCDEF'