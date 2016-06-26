import pump
import pickle

OBJ = u'\u2207'

def main():
    obj = OBJ

    print "Testing: %s" % obj

    print "pump test:"
    serialized = pump.deflate(obj)
    print "Serialized: %r" % serialized
    print "Length: %d" % len(serialized)
    deserialized = pump.inflate(serialized)
    print "Deserialized: %r" % deserialized

    print "pickle test:"
    serialized = pickle.dumps(obj)
    print "Serialized: %r" % serialized
    print "Length: %d" % len(serialized)
    deserialized = pickle.loads(serialized)
    print "Deserialized: %r" % deserialized

if __name__ == '__main__':
    main()
