[org 0x7C00]
[bits 16]

jmp fn_entry

fn_entry:
	xor ax, ax
	mov ds, ax
	mov si, str
	call print_str

	hlt

; prints the character in al
print_char:
	mov ah, 0x0e
	mov bh, 0x00
	int 0x10
	ret

; prints a null terminated string [ds:si]
print_str:
	l1:
		lodsb
		
		cmp al, 0
		jz l2

		call print_char
		jmp l1

	l2:

	ret


str: db "hello world, this is a mbr bootstrap code!", 0