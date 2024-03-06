import sys
import telnetlib

port = 20200

print("Unloading plugin in Maya...")

if len(sys.argv) > 1:
    port = sys.argv[1]

try:
    tn = telnetlib.Telnet("localhost", port)
    tn.write('file -newFile -force;'.encode())
    tn.write('catchQuiet(`unloadPlugin "center_point_cpp"`);'.encode())
    tn.close()
except:
    pass