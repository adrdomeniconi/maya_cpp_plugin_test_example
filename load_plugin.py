import sys
import telnetlib

port = 20200

print("Loading plugin in Maya...")

if len(sys.argv) > 1:
    port = sys.argv[1]

try:
    tn = telnetlib.Telnet("localhost", port)
    tn.write('catchQuiet(`loadPlugin "center_point_cpp"`);'.encode())
    tn.write('evalDeferred("createNode centerPoint");'.encode())
    tn.close()
except:
    pass