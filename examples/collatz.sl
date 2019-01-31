if !argc: 
    write "Usage: collatz <number>" endl
    exit 1
;

= n toint argv 0
while > n 1:
    write n endl
    if %n 2 :
        = n + 1 * n 3
    else
        /= n 2
    ;
;
write n endl
exit
