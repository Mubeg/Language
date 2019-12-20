jmp main_end

main:
pop dx
push 1
push bp
add
pop ax
push [ax]
push 2
push bp
add
pop ax
push [ax]
push 3
push bp
add
pop ax
push [ax]
push 0
push 6000
fmul
push 4
push bp
add
pop ax
push [ax]
push 0
push 5
push bp
add
pop ax
push [ax]
metka_1104975904_1:
push 0
push 5
push bp
add
pop ax
pop [ax]
ja metka_1104976000_1
jmp metka_1104976000_2
metka_1104976000_1:
push 1
jmp metka_1104976000_3
metka_1104976000_2:
push 0
metka_1104976000_3:
push 0
pop
push 0
je metka_1104975904_2
push 0
fout
push 10
cout
push 2000
push 4
push bp
add
pop ax
pop [ax]
je metka_1104977328_1
jmp metka_1104977328_2
metka_1104977328_1:
push 1
jmp metka_1104977328_3
metka_1104977328_2:
push 0
metka_1104977328_3:
push 0
pop
push 0
je metka_1104977840_1
push 4
push bp
add
pop ax
pop [ax]
push 10
push bp
sub
pop bp
pop ax
push dx
return
end
jmp metka_1104977840_2
metka_1104977840_1:
push 0
pop
push 4
push bp
add
pop ax
pop [ax]
push 10
push bp
sub
pop bp
pop ax
push dx
return
end
metka_1104977840_2:
push 0
pop
jmp metka_1104975904_1
metka_1104975904_2:
push 0
push 0
push bp
add
pop ax
pop [ax]
je metka_1104980912_1
jmp metka_1104980912_2
metka_1104980912_1:
push 1
jmp metka_1104980912_3
metka_1104980912_2:
push 0
metka_1104980912_3:
push 0
pop
push 0
je metka_1104981424_1
push 0
push bp
add
pop ax
pop [ax]
fout
push 10
cout
jmp metka_1104981424_2
metka_1104981424_1:
push 0
pop
push 2000
push 10
push bp
sub
pop bp
pop ax
push dx
return
end
metka_1104981424_2:
push 0
pop
push dx
push 0
push bp
add
pop ax
pop [ax]
push 0
push bp
add
pop ax
pop [ax]
push 0
push bp
add
pop ax
pop [ax]
push 10
push bp
add
pop bp
call main
pop dx
push ax
push 6
push bp
add
pop ax
push [ax]
push dx
return
end
main_end:
call main
end
