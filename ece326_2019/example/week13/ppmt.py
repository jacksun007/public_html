#!/usr/bin/python3

import random

def ppmt(rate, per, pv):
    while per > 0:
        # constant payment formula
        pmt = (pv*rate) / (1 - (1 + rate)**(-per))
        # present value = pv - payment + interest
        pv = pv - pmt + pv*rate
        per -= 1
        extra = yield pmt
        if extra is not None:
            pv += extra
    # if no more period left, repay full sum immediately
    if pv > 0:
        yield pv
     
apr = 0.05
years = 2
# e.g. monthly: 12, weekly: 52
freq = 12        
            
def main():
    g = ppmt(apr/freq, years*freq, 10000)
    for p in g:
        print("Paid $%.2f"%p)
        if random.randint(0, 9) == 0:
            s = random.randint(-1000, 1000)
            if s > 0:
                print("Borrowed $%.2f"%s)
            else:
                print("Lumpsum $%.2f"%-s)
            p = g.send(s)
            print("Paid $%.2f"%p)
            
if __name__ == "__main__":
    main()