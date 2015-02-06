#!/usr/bin/env python
# encoding=utf-8

from SearchManager import *
import time
import demjson

params = demjson.decode(raw_input())

query_data = ['' for i in range(len(params))]

if __name__=='__main__':

    sm = []
    tnum = range(len(params))

    for i in tnum:
        sm.append(SearchManager(i, query_data))
        sm[i].setParams(params[i])
        sm[i].start()
        
    for i in tnum:
        sm[i].join()
    
    print demjson.encode(query_data, {'encoding': 'utf-8', 'escape_unicode': True})

