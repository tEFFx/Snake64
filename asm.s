.export _seed, _setSeed, _rand

_seed =      0

_setSeed:   lda $D012
            sta _seed
            rts
            
_rand:      lda _seed
            beq doEor
            clc
            asl
            beq noEor
            bcc noEor
doEor:      eor #$1D
noEor:      sta _seed
            rts