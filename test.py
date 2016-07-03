import pump
import pickle

OBJ = -100012L
OBJ = u'\u2208\u2394\x12\xFF\x00'
OBJ = (1,)

def main():
    obj = OBJ

    print("Testing: %s" % str(obj))

    print("pump test:")
    serialized = pump.deflate(obj)
    print("Serialized: %r" % serialized)
    print("Length: %d" % len(serialized))
    deserialized = pump.inflate(serialized)
    print("Deserialized: %s" % repr(deserialized))
    print("Equal: %s" % (OBJ == deserialized))

    print('\n')

    print("pickle test:")
    serialized = pickle.dumps(obj)
    print("Serialized: %r" % serialized)
    print("Length: %d" % len(serialized))
    deserialized = pickle.loads(serialized)
    print("Deserialized: %s" % repr(deserialized))
    print("Equal: %s" % (OBJ == deserialized))

if __name__ == '__main__':
    main()
