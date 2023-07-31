import io


def readnext(f:io.TextIOWrapper):
    s=""
    while(f.readable()):
        s = f.read(1)
        if (s == " ") or (s == "\n"):
            continue
        else:
            break
    while(f.readable()):
        t = f.read(1)
        if (t == " ") or (t == "\n") or (t == ''):
            break
        s += t
    return s
