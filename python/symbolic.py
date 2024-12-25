import sympy
from sympy import *

P, IF, F, R, V, C, V0, a, b, c, d, g = symbols('P IF F R V,C,V0 a b c d g')

PP = V / C + F*R

CC = a / (1 - exp(b - V))
RR = F*(c-g*V) + F**2*d
PP=PP.subs(C, CC)
PP=PP.subs(R, RR)
P = PP.subs(V, V0 + IF)
diffPV0 = diff(P, V0)
diffPa = diff(P, a)
diffPb = diff(P, b)
diffPc = diff(P, c)
diffPd = diff(P, d)
diffPg = diff(P, g)
print("P=",P)
print("dP/dV0=", diffPV0)
print("dP/da=", diffPa)
print("dP/db=", diffPb)
print("dP/dc=", diffPc)
print("dP/d d=", diffPd)
print("dP/dg=", diffPg)