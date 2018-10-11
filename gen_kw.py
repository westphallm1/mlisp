import sys
template = "#define {} CHR_CODE({},{},{},{})"
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
        print(template.format(keyword,*chars))
