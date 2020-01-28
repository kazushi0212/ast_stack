	INITIAL_GP = 0x10008000		# initial value of global pointer
 	INITIAL_SP = 0x7ffffffc		# initial value of stack pointer
	# system call service number
	stop_service = 99

	.text
init:
	# initialize $gp (global pointer) and $sp (stack pointer)
	la	$gp, INITIAL_GP
	la	$sp, INITIAL_SP
	jal	main			# jump to `main'
	nop				# (delay slot)
	li	$v0, stop_service	# $v0 <- 99 (stop_service)
	syscall				# stop
	nop
	# not reach here
stop:					# if syscall return
	j stop				# infinite loop...
	nop				# (delay slot)

	.text 	0x00001000
main:
	addi  $sp, $sp, -32
	sw  $ra, 28($sp)
	li  $v0, 0
	sw  $v0, 0($sp)
	nop
	li   $t0, sum
	sw   $v0, 0($t0)
	li  $v0, 1
	sw  $v0, 0($sp)
	nop
	li   $t0, i
	sw   $v0, 0($t0)
L0:
	li  $t0, i
	lw  $v0, 0($t0)
	nop
	sw  $v0, 0($sp)
	nop
	li  $v0, 11
	sw  $v0, 4($sp)
	nop
	lw $t0, 0($sp)
	nop
	lw $t1, 4($sp)
	nop
	slt  $v0, $t0, $t1
	beq  $v0, $zero, L1
	li  $t0, sum
	lw  $v0, 0($t0)
	nop
	sw  $v0, 0($sp)
	nop
	li  $t0, i
	lw  $v0, 0($t0)
	nop
	sw  $v0, 4($sp)
	nop
	lw  $t0, 0($sp)
	nop
	lw  $t1, 4($sp)
	nop
	add  $v0, $t0, $t1
	li   $t0, sum
	sw   $v0, 0($t0)
	li  $t0, i
	lw  $v0, 0($t0)
	nop
	sw  $v0, 0($sp)
	nop
	li  $v0, 1
	sw  $v0, 4($sp)
	nop
	lw  $t0, 0($sp)
	nop
	lw  $t1, 4($sp)
	nop
	add  $v0, $t0, $t1
	li   $t0, i
	sw   $v0, 0($t0)
	j  L0
	nop
L1:
end:
	lw  $ra, 28($sp)
	addi  $sp, $sp, 32
	jr $ra

	.data   0x10004000
i:	.word   0x0000
sum:	.word   0x0000
