;
; teste_numero_de_ciclos.asm
;
; Created: 15/07/2022 17:41:35
; Author : Matheus
;


LDI R17,20     ; 1 ciclo
L1:            ; ATRASO
    LDI R16,50 ; 1 ciclo  
L2:            ; VOLTA
    NOP        ; 1 ciclo
    NOP        ; 1 ciclo
    DEC R16    ; 1 ciclo
    BRNE L2    ; 2/1 ciclos
               ; 1 + 5 * R16 - 1 -> 1 + 5 * 50 - 1 = 250
    DEC R17    ; 1 ciclo
    BRNE L1    ; 2/1 ciclos
               ; (3 + ncd_R16) * R17 - 1 =
			   ; (3 + 1 + 5 * 50 - 1) * 20 - 1 = 5059

               ; 1  + 5059 = 5309

NOP

