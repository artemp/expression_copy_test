####Sample output 
```bash
Parsing expression ...
([name]='mapnik')
copying expr 1000000 times.. 
([name]='mapnik')
 0.228176s wall, 0.230000s user + 0.000000s system = 0.230000s CPU (100.8%)
parsing expr 1000000 times.. 
([name]='mapnik')
 18.105859s wall, 18.090000s user + 0.010000s system = 18.100000s CPU (100.0%)
parsing expr (reusing grammar)1000000 times.. 
([name]='mapnik')
 6.132049s wall, 6.130000s user + 0.010000s system = 6.140000s CPU (100.1%)
Done
```

As expected, parsing expressions (even when re-using grammar) is about 30 times slower than copying
