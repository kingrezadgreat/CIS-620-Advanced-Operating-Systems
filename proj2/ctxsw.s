        .file   "ctxsw.s"
        .version        "01.01"
gcc2_compiled.:
.text
        .align 4
.globl ctxsw
        .type    ctxsw,@function
ctxsw:
        pushl %ebp
        movl  %esp,%ebp          # frame pointer
        pushl %esi 
        pushl %edi               # preserve registers
        movl  8(%ebp),%ebx      
        movl  %esp,(%ebx)      
        movl  12(%ebp),%ebx     
        movl  (%ebx),%esp       
        popl  %edi
        popl  %esi
.L1:
        popl %ebp 
        ret
.Lfe1:
        .size    ctxsw,.Lfe1-ctxsw
        .ident  "GCC: (GNU) 2.7.2.3"

