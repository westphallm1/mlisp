=n toint argv 0
=q 2
=prime 1

while & prime < q n: 
  if !%n q: =prime 0;
  +=q 1
;

if ==prime 1:
  swrite n "is prime!" endl
else
  swrite n "is not prime..." endl
;
exit
