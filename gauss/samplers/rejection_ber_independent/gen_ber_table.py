'''
Created on 12.03.2014

@author: thomas
'''

import math
from decimal import *
import pprint
import sys

def get_binary_expansion_fraction(x):
        val = 0
        res =[]
        for i in range(1, getcontext().prec):
            tmp = Decimal(2**(-i))
            if (val+tmp) < x:
                val = val + tmp
                res.append(1)
            else:
                res.append(0)
        return res    
    
def generate_table(max_val,sigma):
        '''generate the table for the exp functions'''
        val = []
        
        #print(max_val, "dadasdas")       
        for i in range(0, max_val):
            #val.append(   list(bin(int(str(((Decimal(2) ** ((Decimal((-i)/(2*sigma*sigma)))))/S_sigma).quantize(Decimal(10)**(Decimal(-max_round))))[2:]))[2:]))
            #val.append(   float(((Decimal(2) ** ((Decimal((-i)/(2*sigma*sigma)))))/S_sigma.quantize(Decimal(10)**(Decimal(-max_round))) ) ) )
            #val.append(   str(((((Decimal((-Decimal(2)**(i))/(Decimal(2*sigma*sigma)))))).exp()).quantize(Decimal(10)**(Decimal(-50)))))
            
            tmp = (Decimal(((-Decimal(2)**(i))/(Decimal(2*sigma*sigma))).exp()))
            #print(tmp)
            val.append(get_binary_expansion_fraction(tmp))
            #
        return val
    
def chunks(l, n):
    return [l[i:i+n] for i in range(0, len(l), n)]

def conv(bin_list):
    val = 0;
    for i in range(0,len(bin_list)):
        if bin_list[i] == 1:
            val |= (1<<(len(bin_list)-i-1))
    return val
        
def print_table_bytes(table):
    #print the table as byte array
    val_table = []
    
    for i in range(0,len(table)):
        val_table.append(chunks(table[i], 8))
    
    byte_table = []
    for i in range(0,len(val_table)):
        byte_table.append([])
        for j in range(len(val_table[0])):
            byte_table[i].append(conv(val_table[i][j]))
            #byte_table[i].append(val_table[i][j])
    
    #pprint.pprint(byte_table)
    

    print("#define MAX_BER_ENTRIES "+str(len(byte_table)))
    print("#define MAX_BER_BYTES "+str(len(byte_table[0])))
    print("")
    print("static uint8_t ber_table[MAX_BER_ENTRIES][MAX_BER_BYTES]={")
    for i in range(0,len(byte_table)):
        print("{", end="")
        for j in range(len(byte_table[0])-1):
            print(byte_table[i][j], end="")
            print(",", end="")
        print(byte_table[i][len(byte_table[0])-1], end="")
        print("},")
    
    print("};")
     
     
def print_table_int64(table):
    #Print the table as int64 in columns
    #print the table as byte array
    val_table = []
    
    for i in range(0,len(table)):
        val_table.append(chunks(table[i], 8))
    
    byte_table = []
    for i in range(0,len(val_table)):
        byte_table.append([])
        for j in range(len(val_table[0])):
            byte_table[i].append(conv(val_table[i][j]))
            #byte_table[i].append(val_table[i][j])
    
    #pprint.pprint(byte_table)
    
    print("#define MAX_BER64_ENTRIES "+str(len(byte_table)))
    print("#define MAX_BER64_BYTES "+str(len(byte_table[0])))
    print("#define MAX_BER64_TABLE "+str(len(len(byte_table)*byte_table[0])))
    print("")
    print("static uint64_t ber64_table[]={")
    
    
    val = 0
    cnt = 0
    for j in range(0, len(byte_table[0])-1):
        for i in range(0, len(byte_table)):
            val = (val * 256) + (int(byte_table[i][j]))            
            cnt = cnt +1
            
            if cnt == 8:
                print(val, end="ull, ")
                val = 0;
                cnt = 0    
    print("};")
    
    
def list_to_int(list):
    val = 0
    for i in range(0,len(list)):
        val = val*2 + list[i]
    return val
    
    
    
if __name__ == '__main__':
    #Parameter 1: sigma
    #Parameter 2: tailcut
    #parameter 3: precision



    sigma = int(sys.argv[1])
    tail = int(sys.argv[2])
    precision = int(sys.argv[3])

    print("//Usage: Sigma, tailcut, precision. Eg., 215 13 8")

    print("//Sigma: ",sigma ,"Tailcut: ",tail,"Precision: ",precision)
    print("#define MAX_GAUSS_VAL "+str(int(math.ceil(tail*sigma))))
    print("#define MAX_GAUSS_LOG "+str(int(math.log(int(math.ceil(tail*sigma)),2)))) 
    print("#define SIGMA "+str(sigma)) 

    #Computation
    max_val = math.ceil(math.log(tail*tail*sigma*sigma,2))
    getcontext().prec = math.ceil(precision/8.0)*8 +1 #For byte packing increase precision  
    table = generate_table(max_val,sigma)
    #print(table)
    #print_table_int64(table)
    print_table_bytes(table)
