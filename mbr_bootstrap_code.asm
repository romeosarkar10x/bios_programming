[org 0x7C00]
[bits 16]

jmp fn_entry

fn_entry:
	xor ax, ax
	mov ds, ax
	mov si, str
	call print_str

	jmp $

; prints the character in al
print_char:
	pusha
	mov ah, 0x0e
	int 0x10
	popa
	ret

; prints a null terminated string [ds:si]
print_str:
	print_str_loop:
		lodsb
		
		cmp al, 0
		jz ret_print_str

		call print_char
ret_print_str:
	ret


str: db "hello world, this is a mbr bootstrap code!", 0