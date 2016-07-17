# pump
Python2 & 3 C Extension for serializing certain built-in Python types

`pump` offers functionality very similar to the default `pickle` module.  Unlike `pickle` though, `pump` cannot serialize generic Python objects and classes.  `pump` can only serialize built in Python objects - in particular:
* Integers (only in Python2)
* Floats
* Longs
* Strings (Bytes in Python3)
* Unicodes (Strings in Python3)
* Lists
* Tuples
* Sets
* Frozensets
* Dictionaries
* None objects

However, for most objects `pump` performs serialization faster than the built-in `pickle` library, and more importantly, produces more compact objects.

## Usage
Pump has two single argument functions, `inflate` and `deflate`.  `deflate` is used to create serializations of objects, and `inflate` is used to recreate the original object given its serialization.

## Examples
```
>>> import pump
>>> serialization = pump.deflate(123)
>>> serialization
'\x01\x81{'
>>> pump.inflate(serialization)
123

>>> serialization = pump.deflate('123')
>>> serialization
'\x05\x83123'
>>> pump.inflate(serialization)
'123'

>>> serialization = pump.deflate([1, 2, 3, ('4', '5', '6')])
>>> serialization
'\x08\x96\x84\x01\x81\x01\x01\x81\x02\x01\x81\x03\t\x8a\x83\x05\x814\x05\x815\x05\x816'
>>> pump.inflate(serialization)
[1, 2, 3, ('4', '5', '6')]
```

## Speed and size comparisons to the built-in pickle
Note that all tests were done in Python2, comparing to its cPickle module
