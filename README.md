Émulateur M68K
=====

implemented instruction:
```
    rts

    bra
    bsr
    bcc

    jsr

    tst
    dbcc

    clr

    cmp
    cmpa
    cmpi
    cmpm

    add
    adda
    addq
    addi

    sub
    suba
    subq
    subi

    move
    moveq
    movea
    movem

    lea

    lsd

    mulu
    muls

    or
    andi
    ori
    andi_to_ccr
    ori_to_ccr

    neg
```

implemented addressing mode:
```
    data register
    address register
    address
    address with postincrement
    address with predecrement
    address with displacement
    immediate
    absolute long
```
