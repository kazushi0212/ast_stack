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

	.data   0x10004000 	;in Decl_AST 
fizz:	.word  0x0000 	;in DEFINE_AST 
buzz:	.word  0x0000 	;in DEFINE_AST 
fizzbuzz:	.word  0x0000 	;in DEFINE_AST 
others:	.word  0x0000 	;in DEFINE_AST 
i:	.word  0x0000 	;in DEFINE_AST 
	.text 	0x00001000 	;in Stmts_AST 
main: 	;in Stmts_AST 
	li   $t0,fizz  	;in IDENT_AST 
	li   $t1,0 	;in NUM_AST 
	sw   $t1,0($t0)
	li   $t0,buzz  	;in IDENT_AST 
	li   $t1,0 	;in NUM_AST 
	sw   $t1,0($t0)
	li   $t0,fizzbuzz  	;in IDENT_AST 
	li   $t1,0 	;in NUM_AST 
	sw   $t1,0($t0)
	li   $t0,others  	;in IDENT_AST 
	li   $t1,0 	;in NUM_AST 
	sw   $t1,0($t0)
	li   $t0,i  	;in IDENT_AST 
	li   $t1,1 	;in NUM_AST 
	sw   $t1,0($t0)
$L1:
	li   $t0,i  	;in IDENT_AST 
	lw   $t1,0($t0) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	li   $t2,31 	;in NUM_AST 
	slt   $t3,$t1,$t2
	beq   $t3,$zero,$L2 	;in WHILE_AST 
;!!!!!!!!!!IF!!!!!!!!!!!
	li   $t0,i  	;in IDENT_AST 
	lw   $t1,0($t0) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	li   $t2,15 	;in NUM_AST 
	div   $t1,$t2
	mflo   $t3
	li   $t4,15 	;in NUM_AST 
	mult   $t3,$t4
	mflo   $t5
	li   $t6,i  	;in IDENT_AST 
	lw   $t7,0($t6) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	bne   $t5,$t7,$D1 	;in IF_AST 
	li   $t0,fizzbuzz  	;in IDENT_AST 
	li   $t1,fizzbuzz  	;in IDENT_AST 
	lw   $t2,0($t1) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	li   $t3,1 	;in NUM_AST 
	add   $t4,$t2,$t3
	sw   $t4,0($t0)
;!!!!!!!!!!ELSEIF!!!!!!!!!!!
	j   $D0 	;ELSEIF_AST 
 	nop 
$D1: 	;in ELSEIF_AST 
	li   $t0,i  	;in IDENT_AST 
	lw   $t1,0($t0) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	li   $t2,3 	;in NUM_AST 
	div   $t1,$t2
	mflo   $t3
	li   $t4,3 	;in NUM_AST 
	mult   $t3,$t4
	mflo   $t5
	li   $t6,i  	;in IDENT_AST 
	lw   $t7,0($t6) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	bne   $t5,$t7,$D2 	;in ELSEIF_AST
 	li   $t0,fizz  	;in IDENT_AST 
	li   $t1,fizz  	;in IDENT_AST 
	lw   $t2,0($t1) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	li   $t3,1 	;in NUM_AST 
	add   $t4,$t2,$t3
	sw   $t4,0($t0)
;!!!!!!!!!!ELSEIF!!!!!!!!!!!
	j   $D0 	;ELSEIF_AST 
 	nop 
$D2: 	;in ELSEIF_AST 
	li   $t0,i  	;in IDENT_AST 
	lw   $t1,0($t0) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	li   $t2,5 	;in NUM_AST 
	div   $t1,$t2
	mflo   $t3
	li   $t4,5 	;in NUM_AST 
	mult   $t3,$t4
	mflo   $t5
	li   $t6,i  	;in IDENT_AST 
	lw   $t7,0($t6) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	bne   $t5,$t7,$D3 	;in ELSEIF_AST
 	li   $t0,buzz  	;in IDENT_AST 
	li   $t1,buzz  	;in IDENT_AST 
	lw   $t2,0($t1) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	li   $t3,1 	;in NUM_AST 
	add   $t4,$t2,$t3
	sw   $t4,0($t0)
;!!!!!!!!!!ELSE!!!!!!!!!!!
	j   $D0 	;in ELSE_AST 
	nop 	;in ELSE_AST 
$D3: 	;in ELSE_AST 
	li   $t0,others  	;in IDENT_AST 
	li   $t1,others  	;in IDENT_AST 
	lw   $t2,0($t1) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	li   $t3,1 	;in NUM_AST 
	add   $t4,$t2,$t3
	sw   $t4,0($t0)
	nop 	;in IF_AST
$D0: 	;in IF_AST
$D4:
	li   $t0,i  	;in IDENT_AST 
	li   $t1,i  	;in IDENT_AST 
	lw   $t2,0($t1) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	li   $t3,1 	;in NUM_AST 
	add   $t4,$t2,$t3
	sw   $t4,0($t0)
	j $L1 	;in WHILE_AST 
	nop 	;in WHILE_AST 
$L2: 	;in WHILE_AST 
$EXIT: 
 	jr   $ra 	;in Pro_AST 
 	nop