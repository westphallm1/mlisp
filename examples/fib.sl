if argc():
  = n toint(argv 0)
else
  swrite "Usage: fib.l <max term>" endl
  exit 1
;

= a 0
= b 1
while > n 0:
  swrite b endl
  -= n 1
  += b a
  = a - b a
;
swrite b endl
exit
