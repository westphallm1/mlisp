"""
Brute force an effective hash for the language
"""
import sys
template = "#define {} CHR_CODE({},{},{},{}) // {}"

header = """\
#ifndef KEYWORDS_H
#define KEYWORDS_H
#define CHR_CODE(a,b,c,d) (a + (b<<{}) + (c<<{}) + (d<<{})) % {}
"""

footer = "#endif /* KEYWORDS_H */"


HASH = 97
STEP = 7


def do_hash(a,b,c,d):
    a = ord(a.strip("'"))
    b = ord(b.strip("'"))
    c = ord(c.strip("'"))
    d = ord(d.strip("'"))
    return (a + (b<<STEP) + (c<<2*STEP) + (d<<3*STEP))%HASH

def check_unique(word,set_):
    hashcode = do_hash(*word)
    if hashcode in set_:
        raise ValueError
    set_.add(hashcode)

def try_hash(print_=False):
    set_ = set()
    if print_:
        print(header.format(STEP,STEP*2,STEP*3,HASH))
    with open(sys.argv[1],'r') as kwf:
        for line in kwf.readlines():
            keywords = line.strip().split()
            keyword = keywords[0]
            if len(keywords) == 1:
                context = keyword.lower()[:4]
            else:
                context = keywords[1]
            chars = ["'%s'"%c for c in context]
            while len(chars) < 4:
                chars.append("0")
            check_unique(chars,set_)
            if print_:
                print(template.format(keyword,*chars,do_hash(*chars)))
    if print_:
        print(footer)


found_hash = False
while not found_hash:
    try:
        try_hash()
        found_hash = True
        try_hash(print_=True)
    except ValueError:
        HASH+=2

    
