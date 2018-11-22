if: ==(argc() 0) {
    swrite("Usage: collatz <number>")
    exit(1)
}
=(n toint(argv(0)))


while: >(n 1) {
    swrite(n endl)
    if: %(n 2) {
        *=(n 3)
        +=(n 1)
    else
        /=(n 2)
    }
}
swrite(n endl)
