section .text

global listFindKeyAsm

listFindKeyAsm:
        mov     r9,  QWORD [rdi+16] ; r9  = &list->nodes
        mov     eax, DWORD [r9+16]  ; eax = list->nodes[0].next (int current)
        xor     r10d, r10d          ; r10d = 0; (for setting edx to 0)

        vmovdqu ymm1, YWORD [rsi]   ; key is copied to ymm1 (for cmp)

        test    eax, eax
        jle     NotFound            ; if (current <= 0) return 0
        jmp     StringsCmp

    NextNode:
        mov     eax, DWORD [rcx+16] ; current = list->nodes[current].next
        test    eax, eax
        jle     NotFound            ; if (current == 0) return 0;

    StringsCmp:
        movsx   rdx, eax            ; rdx = current
        lea     rdx, [rdx+rdx*2]    ; rdx = current * 3
        lea     rcx, [r9+rdx*8]     ; rcx = &list->nodes + 24 * current 
                                    ;        list->nodes[current];
        mov     edx, r10d           ; edx = 0;
        mov     r8, QWORD [rcx]     ; r8  = list->nodes[current].data.str;

        vmovdqu ymm0, YWORD [r8]    ; list->nodes[current].data.str is copied to ymm0
        vptest  ymm0, ymm1          ; CF = (list->nodes[current].data.str == key);
        
        jnc     NextNode            ; if(!stringsEqualAsm(...)) continue;
        ret                         ; if( stringsEqualAsm(...)) return current;

    NotFound:
        xor     eax, eax
        ret