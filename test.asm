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

	.data   0x10004000
i:	.word  0x0000
fact:	.word  0x0000
	.text 	0x00001000
main:
	li   $t0,fact
	li   $t1,1
	sw   $t1,0($t0)
	li   $t0,i
	li   $t1,1
	sw   $t1,0($t0)
$L1:
	li   $t0,i
	lw   $t1,0($t0)
	nop
	li   $t2,6
	slt   $t3,$t1,$t2
	beq   $t3,$zero,$L2
	li   $t0,fact
	lw   $t1,0($t0)
	nop
	li   $t2,fact
	lw   $t3,0($t2)
	nop
	li   $t4,i
	lw   $t5,0($t4)
	nop
	mult   $t5,$t3
	mflo   $t6
	sw   $t6,0($t0)
	li   $t0,i
	li   $t1,i
	lw   $t2,0($t1)
	nop
	li   $t3,1
	add   $t4,$t3,$t2
	sw   $t4,0($t0)
	j $L1
	nop
$L2:
$EXIT: 
 	jr   $ra 
 	nop