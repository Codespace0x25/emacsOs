	global Mouseirq

	extern mouse_handler 
	section .text

Mouseirq:
	cli
	pushad
	call mouse_handler
	popad
	iretd
