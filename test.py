import pump
import pickle

#OBJ = u'\u2207'
#OBJ = [1]
OBJ = 123985712935791827359081732905712983571239857129357918273590817329057129835712398571293579182735908173290571298357123985712935791827359081732905712983571239857129357918273590817329057129835712398571293579182735908173290571298357
OBJ = -100012L
OBJ = u'\u2208\u2394\x12\xFF\x00'

def main():
    obj = OBJ

    print("Testing: %s" % obj)

    print("pump test:")
    serialized = pump.deflate(obj)
    print("Serialized: %r" % serialized)
    print("Length: %d" % len(serialized))
    deserialized = pump.inflate(serialized)
    print("Deserialized: %r" % deserialized)
    print("Equal: %s" % (OBJ == deserialized))

    print('\n')

    print("pickle test:")
    serialized = pickle.dumps(obj)
    print("Serialized: %r" % serialized)
    print("Length: %d" % len(serialized))
    deserialized = pickle.loads(serialized)
    print("Deserialized: %r" % deserialized)
    print("Equal: %s" % (OBJ == deserialized))

if __name__ == '__main__':
    main()
