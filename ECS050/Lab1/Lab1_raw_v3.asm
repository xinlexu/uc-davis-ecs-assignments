    // Standard constants
    $RESULT_W 0
    $RESULT_F 1

    // store lfsr random number used by lfsr_galois
    @lfsr   0x0A

    // store the current rolled number
    @current_num 0x0B

    // pointer of circular buffer header 
    // buffer is located in 0x10~0x1F
    @buf_ptr    0x0C
    
    // load buffer pointer
    movlw   0x10
    movwf   buf_ptr

    // Load seed value into lfsr
    movlw   0x79 // any non-zero value will work
    movwf   lfsr

:origin
    // Setup io ports
    movlw 	00011000b
    tris 	6
    call    clear_led


// -------------------- main loop --------------------

:loop
    // check GP3 button, if it is pressed then go on, 
    // else display the actual value of the current dice rolls 
    btfsc   GPIO, GP3
    goto    loop

    // now GP3 is pressed, we check GP4 status, 
    // if it is clear (not pressed) then go on (2d8z), 
    // else (pressed) goto roll_1d16
    btfss   GPIO, GP4
    call    roll_1d16
    btfsc   GPIO, GP4
    call    roll_2d8

:show_result
    // stop rolling and stop flash LED
    call    clear_led
    // display the current rolled number on LED

    btfsc   current_num, 0
    bsf     GPIO, GP0
    btfsc   current_num, 1
    bsf     GPIO, GP1
    btfsc   current_num, 2
    bsf     GPIO, GP2
    btfsc   current_num, 3
    bsf     GPIO, GP5

    // record the current result in buffer
    movf    buf_ptr, 0      // W <-- buf_ptr
    movwf   FSR             // FSR <-- W
    movf    current_num, 0  // W <-- current_num
    movwf   INDF            // *buf_ptr <-- W
    
    incf    buf_ptr, 0      // W <-- buf_ptr + 1
    andlw   0x0F            // W <-- W & 0x0F
    iorlw   0x10            // W <-- W | 0x10
    movwf   buf_ptr         // buf_ptr <-- W
    
    goto    loop


// -------------------- functions --------------------

:roll_1d16

    call    lfsr_galois

    // as result
    movf    lfsr, 0
    andlw   0x0f    // mod 16 
    movwf   current_num

    // CAUTION: blink_wait: set LED randomly 
    // btfsc   lfsr, 0
    // bsf     GPIO, GP0
    // btfsc   lfsr, 1
    // bsf     GPIO, GP1
    // btfsc   lfsr, 2
    // bsf     GPIO, GP2
    // btfsc   lfsr, 3
    // bsf     GPIO, GP5

    // check GP3 button, if still pressed then goto roll_1d16, else show_result
    btfss   GPIO, GP3
    goto    roll_1d16
    retlw   0
        
:roll_2d8
    // call lfsr_galois twice to get two numbers of 1d8
    call    lfsr_galois
    movf    lfsr, 0     // load and store for RISC architecture
    andlw   0x07        // mod 8
    movwf   current_num
    call    lfsr_galois
    movf    lfsr, 0
    andlw   0x07        // mod 8
    addwf   current_num, 1

    // CAUTION: blink_wait: set LED randomly 
    // btfsc   lfsr, 0
    // bsf     GPIO, GP0
    // btfsc   lfsr, 1
    // bsf     GPIO, GP1
    // btfsc   lfsr, 2
    // bsf     GPIO, GP2
    // btfsc   lfsr, 3
    // bsf     GPIO, GP5

    // check GP3 button, if still pressed then goto roll_2d8, else show_result
    btfss   GPIO, GP3
    goto    roll_2d8
    retlw   0

:lfsr_galois
    // First clear the carry so that we know that it is zero. Now, shift the lfsr right
    // putting the LSB in carry and copying the lfsr into the working register for more
    // processing
    bcf     STATUS,C
    rrf     lfsr, RESULT_W
    // If the carry (LSB) is set we want to
    // xor the lfsr with our xor taps 8,6,5,4 (0,2,3,4)
    //
    btfsc   STATUS, C
    xorlw   10111000b
    movwf   lfsr // save the new lfsr
    retlw   0

:clear_led
    bcf	    GPIO, GP0
    bcf	    GPIO, GP1
    bcf	    GPIO, GP2
    bcf	    GPIO, GP5
    retlw   0

