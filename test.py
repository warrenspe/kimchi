import pump
import pickle

OBJ = -100012L
OBJ = u'\u2208\u2394\x12\xFF\x00'
OBJ = ([],)
OBJ = ((112598712587192579182759871,),)
OBJ = 11238957129385791237598173298572135L
OBJ = frozenset([1, 2, 'asdf', (1, 2, 5), (6, 7, None)])
OBJ = set([1, 2, 'asdf', (1, 2, 5), (6, 7, None)])
OBJ = -112980375918273590182735091783250987132509871320598712305987190328570192875019273590178325231245928375192735917325091732597123905710293875190237590182735981

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
