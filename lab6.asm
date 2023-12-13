section .rodata

EnterMsg:                                                  
        db "Enter the file name: ", 0                   
formatInput:                                                  
        db "%s", 0                             
fileFlag:                                              
        db "r", 0                                    
ErrorMsg:                                                 
        db "Error opening the file", 0                   
term:                                                  
        db 10, 0                                    
ResultMsg:                                                
        db "Number of empty strings %d", 10, 0                      

default rel

global main: function

extern fclose, fgets, strcmp, exit, puts, fopen, scanf, printf

section .text   align=1 exec                            ; section number 1, code

main:   ; Function begin                                      
        push    rbp        ; save old base pointer                             
        mov     rbp, rsp   ; set new base pointer                        
        sub     rsp, 240   ; allocate 240 bytes for local variables       

        mov     rax, qword [fs:abs 40] ; stack canary
        mov     qword [rbp-8], rax     ; save stack canary                    
        xor     eax, eax                ; set eax to 0                
        lea     rax, EnterMsg     ; load address of EnterMsg to rax                   
        mov     rdi, rax                ; set rax to first argument of printf                
        mov     eax, 0                  ; set eax to 0                

        call    printf                  ; call printf with 1 argument (EnterMsg)                
        lea     rax, [rbp-224]         ; load address of file name to rax                
        mov     rsi, rax                ; set rax to second argument of scanf               
        lea     rax, formatInput  ; load address of formatInput to rax                      
        mov     rdi, rax                ; set rax to first argument of scanf                
        mov     eax, 0                  ; set eax to 0                
        call    scanf          ; call scanf with 2 arguments (formatInput, file name)    

        lea     rax, [rbp-224]         ; load address of file name to rax                
        lea     rdx, fileFlag     ; load address of fileFlag to rdx                   
        mov     rsi, rdx                ; set rdx to second argument of fopen
        mov     rdi, rax                ; set rax to first argument of fopen
        call    fopen                   ; call fopen with 2 arguments (file name, fileFlag)
        mov     qword [rbp-232], rax   ; save file pointer to local variable               
        cmp     qword [rbp-232], 0     ; compare file pointer to 0              
        jnz     counterInit             ; if file pointer is not 0, jump to counterInit                     
        lea     rax, ErrorMsg     ; load address of ErrorMsg to rax                   
        mov     rdi, rax                ; set rax to first argument of puts                
        call    puts                    ; call puts with 1 argument (ErrorMsg)                
        mov     edi, 1                  ; set edi to 1                
        call    exit                    ; call exit with 1 argument (1)

counterInit:  
        mov     dword [rbp-236], 0   ; set counter to 0       
        jmp     fileRead              ; jump to fileRead   

cmpLoop:  
        lea     rax, [rbp-112]        ; load address of buffer to rax                  
        lea     rdx, term       ; load address of term to rdx
        mov     rsi, rdx                ; set rdx to second argument of strtok                
        mov     rdi, rax               ; set rax to first argument of strtok                 
        call    strcmp                 ; call strcmp with 2 arguments (buffer, term)                 
        test    eax, eax               ; test eax with eax                 
        jnz     fileRead               ; if eax is not 0, jump to fileRead
        add     dword [rbp-236], 1    
            ; increment counter by 1             
fileRead:  mov     rdx, qword [rbp-232]  ; load file pointer to rdx               
        lea     rax, [rbp-112]            ; load address of buffer to rax              
        mov     esi, 100                  ; set esi to 100              
        mov     rdi, rax                  ; set rax to first argument of fgets               
        call    fgets                     ; call fgets with 3 arguments (buffer, 100, file pointer)              
        test    rax, rax                  ; test rax with rax              
        jnz     cmpLoop                   ; if rax is not 0, jump to cmpLoop               
        mov     eax, dword [rbp-236]        
        mov     esi, eax                  ; set eax to second argument of printf          
        lea     rax, ResultMsg      ; load address of ResultMsg to rax      
        mov     rdi, rax                               
        mov     eax, 0                              
        call    printf                    ; call printf with 2 arguments (ResultMsg, counter)        
        mov     rax, qword [rbp-232]               
        mov     rdi, rax                             
        call    fclose                    ; call fclose with 1 argument (file pointer)         
        mov     eax, 0                               
        mov     rdx, qword [rbp-8]       ; load stack canary to rdx             

        sub     rdx, qword [fs:abs 40]   ; compare stack canary with stack canary from before              
        jz      endProg                                                       
endProg: 
        leave                                         
        ret                                            
; main End of function
