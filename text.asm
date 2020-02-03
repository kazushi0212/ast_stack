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

	.text      0x00001000
main:

	addi  $sp, $sp, -32
	sw  $ra, 28($sp)
	li  $v0, 0
	sw  $v0, 0($sp)
	li   $t0, a
	sw   $v0, 0($t0)
	li  $v0, 1
	sw  $v0, 0($sp)
	li   $t0, b
	sw   $v0, 0($t0)
	li  $v0, 3
	sw  $v0, 0($sp)
	li   $t0, n
	sw   $v0, 0($t0)
	li  $v0, 5
	sw  $v0, 0($sp)
	li   $t0, n
	sw   $v0, 4($t0)
	li  $v0, 7
	sw  $v0, 0($sp)
	li   $t0, n
	sw   $v0, 8($t0)
	li  $t0, a
	lw  $v0, 0($t0)
	nop
	sw  $v0, 0($sp)
	li  $v0, 2
	sw  $v0, 4($sp)
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	add  $v0, $t0, $t1
	sw  $v0, 0($sp)
	li   $t0, a
	sw   $v0, 0($t0)
	li  $t0, a
	lw  $v0, 0($t0)
	nop
	sw  $v0, 0($sp)
	la  $t0,n
	lw  $v0, 0($t0)
	nop
	sw  $v0,4($sp)
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	mult  $t0, $t1
	mflo  $v0
	sw  $v0, 0($sp)
	li  $t0, b
	lw  $v0, 0($t0)
	nop
	sw  $v0, 4($sp)
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	sub  $v0, $t0, $t1
	nop
	sw  $v0, 0($sp)
	li   $t0, b
	sw   $v0, 0($t0)
	la  $t0,n
	lw  $v0, 0($t0)
	nop
	sw  $v0,0($sp)
	la  $t0,n
	lw  $v0, 4($t0)
	nop
	sw  $v0,4($sp)
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	add  $v0, $t0, $t1
	sw  $v0, 0($sp)
	li  $v0, 2
	sw  $v0, 4($sp)
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	div  $t0, $t1
	mflo  $v0
	sw  $v0, 0($sp)
	li   $t0, n
	sw   $v0, 0($t0)
	la  $t0,n
	lw  $v0, 8($t0)
	nop
	sw  $v0,0($sp)
	la  $t0,n
	lw  $v0, 4($t0)
	nop
	sw  $v0,4($sp)
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	div  $t0, $t1
	mfhi  $v0
	sw  $v0, 0($sp)
	li   $t0, n
	sw   $v0, 4($t0)
end:
	lw  $ra, 28($sp)
	addi  $sp, $sp, 32
$EXIT: 
 	jr   $ra 	;in Pro_AST 
 	nop 
