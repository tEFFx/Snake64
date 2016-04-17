.export _init
.import _gameLoop

_init:          sei

                ldy #$7f
                sty $dc0d
                sty $dd0d
                lda $dc0d
                lda $dd0d
                
                lda #$01
                sta $d01a
                
                lda #<irq
                ldx #>irq
                sta $0314
                sta $0315
                
                lda #$00
                sta $d012
                
                lda $d011
                and #$7f
                sta $d011

                cli
                
                jmp *
                
irq:            dec $d019
                jsr _gameLoop
                jmp $ea81