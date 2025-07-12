// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

// total screen registers
@8192
D=A
@R0
M=D

// current screen register index
@registerIdx
M=0

// loop through all screen registers, one at a time
(LOOP)
  // read keyboard
  // if KBD == 0, go to black
  @KBD
  D=M
  @BLACK
  D;JEQ
  // else go to white
  @WHITE
  D;JNE

// paint current screen register to black
(BLACK)
  @SCREEN
  D=A
  @registerIdx
  A=D+M
  M=0
  // go to next register
  @NEXT
  0;JMP

// paint curent scren register to white
(WHITE)
  @SCREEN
  D=A
  @registerIdx
  A=D+M
  M=-1
  // go to next register
  @NEXT
  0;JMP

// go to next screen register, 
// loop back to first register if reached en
(NEXT)
  @R0
  D=M
  @registerIdx
  D=D-M
  // reset registerIdx if (R0 - registerIdx) <= 0
  @RESET
  D;JLE
  // increment registerIdx if (R0 - registerIdx) > 0
  @INC
  D;JGT

// set registerIdx to 0
(RESET)
  @registerIdx
  M=0
  @LOOP
  0;JMP

// increment registerIdx
(INC)
  @registerIdx
  M=M+1
  @LOOP
  0;JMP

(END)
  @END
  0;JMP

