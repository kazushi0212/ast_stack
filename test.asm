    INITIAL_GP = 0x10008000     # initial value of global pointer 
    INITIAL_SP = 0x7ffffffc     # initial value of stack pointer 
    # system call service number 
    stop_service = 99 

    .text 
init: 
    # initialize $gp (global pointer) and $sp (stack pointer) 
    la	$gp, INITIAL_GP    # (下の2行に置き換えられる) 
#   lui	$gp, 0x1000     # $gp ← 0x10008000 (INITIAL_GP) 
#   ori	$gp, $gp, 0x8000 
    la	$sp, INITIAL_SP     # (下の2行に置き換えられる) 
#   lui	$sp, 0x7fff     # $sp ← 0x7ffffffc (INITIAL_SP) 
#   ori	$sp, $sp, 0xfffc 
    jal	main            # jump to `main' 
    nop             # (delay slot) 
    li	$v0, stop_service   # $v0 ← 99 (stop_service) 
    syscall             # stop 
    nop 
    # not reach here 
stop:                   # if syscall return  
    j stop              # infinite loop... 
    nop             # (delay slot) 
 
    .text 	0x00001000
    .data   0x00004000
    li    $a0,0
    li    $a1,1
$LOOP:
    bltz   $t0,11,$EXIT
    lw    $t0,0(第1要素のアドレス) 
    lw    $t1,0(第2要素のアドレス) 
    add   $t3,$t0,$t1 
    sw    $t3,0($sp)

    lw    $t0,0(第1要素のアドレス) 
    lw    $t1,0(第2要素のアドレス) 
    add   $t3,$t0,$t1 
    sw    $t3,4($sp)
1
    j   $LOOP 
    nop
$EXIT: 
    jr   $ra 
    nop
