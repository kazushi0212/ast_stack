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
N:	.word  0x0000 	;in DEFINE_AST 
i:	.word  0x0000 	;in DEFINE_AST 
n:	.word  0x0000 	;in DEFINE_AST 
k:	.word  0x0000 	;in DEFINE_AST 
	.text 	0x00001000 	;in Stmts_AST 
main: 	;in Stmts_AST 
	li   $t0,N  	;in IDENT_AST 
	li   $t1,10 	;in NUM_AST 
	sw   $t1,0($t0)
	li   $t0,i  	;in IDENT_AST 
	li   $t1,2 	;in NUM_AST 
	sw   $t1,0($t0)
$L0_0:
	li   $t0,i  	;in IDENT_AST 
	lw   $t1,0($t0) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	li   $t2,N  	;in IDENT_AST 
	lw   $t3,0($t2) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	li   $t4,2 	;in NUM_AST 
	div   $t3,$t4
	mflo   $t5
	slt   $t6,$t1,$t5
	beq   $t6,$zero,$L0_1 	;in WHILE_AST 
	li   $t0,n  	;in IDENT_AST 
	li   $t1,2 	;in NUM_AST 
	sw   $t1,0($t0)
$L1_0:
	li   $t0,n  	;in IDENT_AST 
	lw   $t1,0($t0) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	li   $t2,6 	;in NUM_AST 
	slt   $t3,$t1,$t2
	beq   $t3,$zero,$L1_1 	;in WHILE_AST 
	li   $t0,n  	;in IDENT_AST 
	li   $t1,n  	;in IDENT_AST 
	lw   $t2,0($t1) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	li   $t3,2 	;in NUM_AST 
	mult   $t2,$t3
	mflo   $t4
	sw   $t4,0($t0)
	li   $t0,k  	;in IDENT_AST 
	li   $t1,i  	;in IDENT_AST 
	lw   $t2,0($t1) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	li   $t3,n  	;in IDENT_AST 
	lw   $t4,0($t3) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	mult   $t2,$t4
	mflo   $t5
	sw   $t5,0($t0)
	li   $t0,n  	;in IDENT_AST 
	li   $t1,n  	;in IDENT_AST 
	lw   $t2,0($t1) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	li   $t3,1 	;in NUM_AST 
	add   $t4,$t2,$t3
	sw   $t4,0($t0)
	j $L1_0 	;in WHILE_AST 
	nop 	;in WHILE_AST 
$L1_1: 	;in WHILE_AST 
	li   $t0,i  	;in IDENT_AST 
	li   $t1,i  	;in IDENT_AST 
	lw   $t2,0($t1) 	;in IDENT_AST 
	nop 	;in IDENT_AST 
	li   $t3,1 	;in NUM_AST 
	add   $t4,$t2,$t3
	sw   $t4,0($t0)
	j $L0_0 	;in WHILE_AST 
	nop 	;in WHILE_AST 
$L0_1: 	;in WHILE_AST 
$EXIT: 
 	jr   $ra 	;in Pro_AST 
 	nop