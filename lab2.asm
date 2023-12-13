DATA	EQU	$0000	RAM
PROGRAM	EQU	$C000	ROM
RESET	EQU	$FFFE	RESET VECTOR


	ORG	PROGRAM
START	LDAA	#1 ; load a as 1
	LDAB	#2 ; load b as 2
	LDX	#$01fa ; load x as 01fa
	LDY	#$02aa ; load y as 02aa
	ABA ; add a to b
	PSHX ; push x to stack
	PULB ; pull to b
	LDX	#$0000 ; make x null
	ABX ; add a to x
	PULB ; pull to b
	ABX ; add a to x
	PSHY ; push y
	PULB ; pull b
	ABX ; add a to x
	PULB ; pull b
	ABX ; add a to x
	TAB ; tranfer a to b
	ABX ; add a to x
	XGDX ; swap d and x
LOOP	BRA	LOOP	nice way to stop without running wild

	ORG	RESET
	FDB	START	initialise reset vector