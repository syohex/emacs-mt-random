# mt-random

C source code is taken from [here](http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/emt19937ar.html)

## Interfaces

##### `(mt-rand-random &optional max)`

Generate random number from 0 to less than `max`.

#### `(mt-rand-srand &optional seed)`

Set random seed

## License

- mt-rand.el and mt-rand-core.c are licensed by GNU Public License 3
- mt19937ar.c and mt19937ar.h are licensed by 3-BSD License
