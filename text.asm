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
	li   $t0, fizz
	sw   $v0, 0($t0)
	li  $v0, 0
	sw  $v0, 0($sp)
	li   $t0, buzz
	sw   $v0, 0($t0)
	li  $v0, 0
	sw  $v0, 0($sp)
	li   $t0, fizzbuzz
	sw   $v0, 0($t0)
	li  $v0, 0
	sw  $v0, 0($sp)
	li   $t0, others
	sw   $v0, 0($t0)
	li  $v0, 1
	sw  $v0, 0($sp)
	li   $t0, i
	sw   $v0, 0($t0)
$L0_0:
;LT_AST1
	li  $t0, i
	lw  $v0, 0($t0)
	nop
	sw  $v0, 0($sp)
	li  $v0, 31
	sw  $v0, 4($sp)
;LT_AST2
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	slt   $v0,$t0,$t1
	beq   $v0,$zero,$L0_1 	;in WHILE_AST 
	li  $t0, i
	lw  $v0, 0($t0)
	nop
	sw  $v0, 0($sp)
	li  $v0, 15
	sw  $v0, 4($sp)
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	div  $t0, $t1
	mfhi  $v0
	sw  $v0, 0($sp)
	li  $v0, 0
	sw  $v0, 4($sp)
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	bne   $t0,$t1,$D0 	;in IF_AST 
;!!!ADD
	li  $t0, fizzbuzz
	lw  $v0, 0($t0)
	nop
	sw  $v0, 8($sp)
	li  $v0, 1
	sw  $v0, 12($sp)
;ADD!!!
	lw  $t0, 8($sp)
	lw  $t1, 12($sp)
	nop
	add  $v0, $t0, $t1
	nop
	sw  $v0, 8($sp)
	li   $t0, fizzbuzz
	sw   $v0, 0($t0)
	j   $D0_0 	;ELSEIF_AST 
 	nop 
$D0: 	;in ELSEIF_AST 
	li  $t0, i
	lw  $v0, 0($t0)
	nop
	sw  $v0, 0($sp)
	li  $v0, 3
	sw  $v0, 4($sp)
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	div  $t0, $t1
	mfhi  $v0
	sw  $v0, 0($sp)
	li  $v0, 0
	sw  $v0, 4($sp)
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	bne   $t0,$t1,$D1 	;in ELSEIF_AST
 ;!!!ADD
	li  $t0, fizz
	lw  $v0, 0($t0)
	nop
	sw  $v0, 8($sp)
	li  $v0, 1
	sw  $v0, 12($sp)
;ADD!!!
	lw  $t0, 8($sp)
	lw  $t1, 12($sp)
	nop
	add  $v0, $t0, $t1
	nop
	sw  $v0, 8($sp)
	li   $t0, fizz
	sw   $v0, 0($t0)
	j   $D0_0 	;ELSEIF_AST 
 	nop 
$D1: 	;in ELSEIF_AST 
	li  $t0, i
	lw  $v0, 0($t0)
	nop
	sw  $v0, 0($sp)
	li  $v0, 5
	sw  $v0, 4($sp)
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	div  $t0, $t1
	mfhi  $v0
	sw  $v0, 0($sp)
	li  $v0, 0
	sw  $v0, 4($sp)
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	bne   $t0,$t1,$D2 	;in ELSEIF_AST
 ;!!!ADD
	li  $t0, buzz
	lw  $v0, 0($t0)
	nop
	sw  $v0, 8($sp)
	li  $v0, 1
	sw  $v0, 12($sp)
;ADD!!!
	lw  $t0, 8($sp)
	lw  $t1, 12($sp)
	nop
	add  $v0, $t0, $t1
	nop
	sw  $v0, 8($sp)
	li   $t0, buzz
	sw   $v0, 0($t0)
	j   $D0_0 	;in ELSE_AST 
	nop 	;in ELSE_AST 
$D2: 	;in ELSE_AST 
;!!!ADD
	li  $t0, others
	lw  $v0, 0($t0)
	nop
	sw  $v0, 0($sp)
	li  $v0, 1
	sw  $v0, 4($sp)
;ADD!!!
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	add  $v0, $t0, $t1
	nop
	sw  $v0, 0($sp)
	li   $t0, others
	sw   $v0, 0($t0)
	j   $D0_0 	;ELSEIF_AST 
 	nop 
$D3: 	;in ELSEIF_AST 
	li  $t0, i
	lw  $v0, 0($t0)
	nop
	sw  $v0, 0($sp)
	li  $v0, 3
	sw  $v0, 4($sp)
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	div  $t0, $t1
	mfhi  $v0
	sw  $v0, 0($sp)
	li  $v0, 0
	sw  $v0, 4($sp)
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	bne   $t0,$t1,$D4 	;in ELSEIF_AST
 ;!!!ADD
	li  $t0, fizz
	lw  $v0, 0($t0)
	nop
	sw  $v0, 8($sp)
	li  $v0, 1
	sw  $v0, 12($sp)
;ADD!!!
	lw  $t0, 8($sp)
	lw  $t1, 12($sp)
	nop
	add  $v0, $t0, $t1
	nop
	sw  $v0, 8($sp)
	li   $t0, fizz
	sw   $v0, 0($t0)
	j   $D0_0 	;ELSEIF_AST 
 	nop 
$D4: 	;in ELSEIF_AST 
	li  $t0, i
	lw  $v0, 0($t0)
	nop
	sw  $v0, 0($sp)
	li  $v0, 5
	sw  $v0, 4($sp)
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	div  $t0, $t1
	mfhi  $v0
	sw  $v0, 0($sp)
	li  $v0, 0
	sw  $v0, 4($sp)
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	bne   $t0,$t1,$D5 	;in ELSEIF_AST
 ;!!!ADD
	li  $t0, buzz
	lw  $v0, 0($t0)
	nop
	sw  $v0, 8($sp)
	li  $v0, 1
	sw  $v0, 12($sp)
;ADD!!!
	lw  $t0, 8($sp)
	lw  $t1, 12($sp)
	nop
	add  $v0, $t0, $t1
	nop
	sw  $v0, 8($sp)
	li   $t0, buzz
	sw   $v0, 0($t0)
	j   $D0_0 	;in ELSE_AST 
	nop 	;in ELSE_AST 
$D5: 	;in ELSE_AST 
;!!!ADD
	li  $t0, others
	lw  $v0, 0($t0)
	nop
	sw  $v0, 0($sp)
	li  $v0, 1
	sw  $v0, 4($sp)
;ADD!!!
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	add  $v0, $t0, $t1
	nop
	sw  $v0, 0($sp)
	li   $t0, others
	sw   $v0, 0($t0)
	nop 	;in IF_AST
$D0_0: 	;in IF_AST
$D6:
;!!!ADD
	li  $t0, i
	lw  $v0, 0($t0)
	nop
	sw  $v0, 0($sp)
	li  $v0, 1
	sw  $v0, 4($sp)
;ADD!!!
	lw  $t0, 0($sp)
	lw  $t1, 4($sp)
	nop
	add  $v0, $t0, $t1
	nop
	sw  $v0, 0($sp)
	li   $t0, i
	sw   $v0, 0($t0)
	j $L0_0 	;in WHILE_AST 
	nop 	;in WHILE_AST 
$L0_1: 	;in WHILE_AST 
end:
	lw  $ra, 28($sp)
	addi  $sp, $sp, 32
$EXIT: 
 	jr   $ra 	;in Pro_AST 
 	nop 
