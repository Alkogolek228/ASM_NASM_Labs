section .data
    buffer db 200          ; Буфер для хранения строки
    buffer_size equ 200    ; Размер буфера
    buffer_end db 0        ; Завершающий нулевой символ
    promt db "Enter string: ", 0xa ; Строка приглашения
    promt_len equ $ - promt ; Длина строки приглашения
    
section .bss
    input_len resb 1       ; Переменная для хранения длины ввода

section .text
    global _start

_start:
    ; Ввод строки с клавиатуры
    mov eax, 0x4           ; Системный вызов sys_write
    mov ebx, 0x1           ; Файловый дескриптор stdout
    mov ecx, promt         ; Указатель на строку
    mov edx, promt_len     ; Длина строки
    int 0x80               ; Вызов системного вызова
    mov eax, 0x3           ; Системный вызов sys_read
    mov ebx, 0             ; Файловый дескриптор stdin
    mov ecx, buffer        ; Указатель на буфер
    mov edx, buffer_size   ; Максимальное количество символов для чтения
    int 0x80               ; Вызов системного вызова

    ; Найдем длину введенной строки
    mov edi, buffer
find_length:
    mov al, [edi]           ; Загрузка символа из буфера
    cmp al, 0               ; Проверка на завершающий нулевой символ
    je reverse              ; Если символ равен нулю, то переходим к реверсу строки
    inc edi                 ; Увеличиваем счетчик символов
    jmp find_length         ; Переходим к следующему символу

reverse: 
    ; Реверс строки
    mov esi, buffer         ; Указатель на начало строки
    mov edi, edi            ; Указатель на конец строки
    dec edi                 ; Уменьшаем указатель на 1, чтобы не менять местами завершающий нулевой символ
reverse_loop:
    cmp esi, edi            ; Проверка на то, что указатели не пересеклись
    jae print_reversed      ; Если указатели пересеклись, то переходим к выводу реверсированной строки
    mov al, [esi]           ; Загрузка символа из начала строки
    mov ah, [edi]           ; Загрузка символа из конца строки
    mov [esi], ah           ; Запись символа из конца строки в начало строки
    mov [edi], al           ; Запись символа из начала строки в конец строки
    inc esi                 ; Увеличение указателя на начало строки
    dec edi                 ; Уменьшение указателя на конец строки
    jmp reverse_loop        ; Переход к следующему символу

print_reversed:
    ; Вывод реверсированной строки
    mov eax, 0x4           ; Системный вызов sys_write
    mov ebx, 0x1           ; Файловый дескриптор stdout
    mov ecx, buffer        ; Указатель на строку
    mov edx, buffer_size   ; Длина строки
    int 0x80               ; Вызов системного вызова

exit:
    ; Завершение программы
    mov eax, 0x1           ; Системный вызов sys_exit
    mov ebx, 0x0           ; Код возврата 0
    int 0x80               ; Вызов системного вызова