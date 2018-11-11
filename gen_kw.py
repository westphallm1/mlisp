"""
Brute force an effective hash for the language
"""
import sys
template = """\
#define {key} {value}
#define {key}_LONG {long_value}"""
header = """\
#ifndef KEYWORDS_H
#define KEYWORDS_H
#define HASH_VAL {}
#define LONG_HASH_VAL {}
#define CHR_CODE(a,b,c,d) (a + (b<<7) + (c<<14) + (d<<21)) % HASH_VAL
#define CHR_CODE_LONG(a,b,c,d) (a + (b<<7) + (c<<14) + (d<<21)) % LONG_HASH_VAL
int is_kw(int token);
"""
footer = "#endif /* KEYWORDS_H */"
IS_KW = """
#include "keywords.h"
int is_kw(int token){{
    switch(token){{
{}
        return 1;
    }}
    return 0;
}}
"""
STEP=7
def get_long(a,b,c,d):
    a = ord(a)
    b = ord(b)
    c = ord(c)
    d = ord(d)
    return (a + (b<<STEP) + (c<<2*STEP) + (d<<3*STEP))

def get_hash(a,b,c,d,hash_):
    return get_long(a,b,c,d)%hash_


def check_unique(word,hash_,set_):
    hashcode = get_hash(*word,hash_)
    if hashcode == 0 or hashcode in set_:
        raise ValueError
    set_.add(hashcode)

def get_chars(line):
    keywords = line.strip().split()
    keyword = keywords[0]
    if len(keywords) == 1:
        context = keyword.lower()[:4]
    else:
        context = keywords[1]
    chars = list(context)
    while len(chars) < 4:
        chars.append("\0")
    return chars,keyword

def try_hash(hash_):
    set_ = set()
    with open(sys.argv[1],'r') as kwf:
        for line in kwf.readlines():
            chars,_ = get_chars(line)
            check_unique(chars,hash_,set_)

def print_header_file(hash_,long_hash):
    long_kw = []
    with open(sys.argv[2],'w') as outf1:
        outf1.write(header.format(hash_,long_hash))
        outf1.write('\n')
        with open(sys.argv[1],'r') as kwf:
            for line in kwf.readlines():
                chars,keyword = get_chars(line)
                outf1.write(template.format(key=keyword,
                    value=get_hash(*chars,hash_),
                    long_value=get_hash(*chars,long_hash)))
                outf1.write('\n')
                long_kw.append("    case {}_LONG:".format(keyword))
            outf1.write(footer)
            outf1.write('\n')
    with open(sys.argv[3],'w') as outf2:
        outf2.write(IS_KW.format('\n'.join(long_kw)))
        outf2.write('\n')

found_hash = False
found_long_hash = False

HASH = 97
#get an 8 bit hash for keywords
while not found_hash:
    try:
        try_hash(HASH)
        found_hash = True
    except ValueError:
        HASH+=2
        assert HASH <= 255

LONG_HASH = 65521
#get a 16 bit hash for checking against ids
while not found_long_hash:
    try:
        try_hash(LONG_HASH)
        found_long_hash = True
    except ValueError:
        HASH-=2

print_header_file(HASH,LONG_HASH)
