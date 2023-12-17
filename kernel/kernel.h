#ifndef kernel_h
#define kernel_h

// Declaring as a macro forces compiler to use the value directly
// and avoids complications with the linker.
#define VIDEO_PTR ((char*)0xb8000)

void user_input(char *input);

#endif
