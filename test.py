import pump
import pickle

OBJ = -100012L
OBJ = u'\u2208\u2394\x12\xFF\x00'
OBJ = ([],)
OBJ = ((112598712587192579182759871,),)
OBJ = 11238957129385791237598173298572135L
OBJ = frozenset([1, 2, 'asdf', (1, 2, 5), (6, 7, None)])
OBJ = set([1, 2, 'asdf', (1, 2, 5), (6, 7, None)])
OBJ = 11298037591827359018273509178325098713250987132059871230598719032857019287501927359017832523.1245928375192735917325091732597123905710293875190237590182735981

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
