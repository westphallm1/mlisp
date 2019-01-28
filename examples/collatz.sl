if == argc() 0: 
    write "Usage: collatz <number>" endl
    exit 1
;

= n toint argv 0
while > n 1:
    write n endl
    if %(n 2) :
        = n + *(n 3) 1
    else
        /= n 2
    ;
;
write n endl
exit
