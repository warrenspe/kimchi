import sys
import pump

#n = int(sys.argv[1])
n = -350

print "Testing: %d" % n

q = pump.deflate(n)

print repr(q)

print repr(pump.inflate(q))
