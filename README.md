annoy-node
==================

Node bindings for Annoy, an efficient Approximate Nearest Neighbors implementation written in C++.

Note: Still in early development.

Installation
------------

    npm install annoy-node

Usage
-----

    var Annoy = require('annoy-node');
    var annoyIndex1 = new Annoy(10, 'Angular');

    annoyIndex1.addItem(0, [-5.0, -4.5, -3.2, -2.8, -2.1, -1.5, -0.34, 0, 3.7, 6]);
    annoyIndex1.addItem(1, [5.0, 4.5, 3.2, 2.8, 2.1, 1.5, 0.34, 0, -3.7, -6]);
    annoyIndex1.addItem(2, [0, 0, 0, 0, 0, -1, -1, -0.2, 0.1, 0.8]);
    annoyIndex1.build();
    annoyIndex1.save(annoyPath);

    read();

    function read() {
      var annoyIndex2 = new Annoy(10, 'Angular');

      if (annoyIndex2.load(annoyPath)) {
        var v1 = annoyIndex2.getItem(0);
        var v2 = annoyIndex2.getItem(1);
        console.log('Gotten vectors:', v1, v2);

          for (var i = 0; i < v1.length; ++i) {
            sum.push(v1[i] + v2[i]);
          }

          var neighbors = obj2.getNNsByVector(sum, 10, -1, false);
          console.log('Nearest neighbors to sum', neighbors);

          var neighborsAndDistances = obj2.getNNsByVector(sum, 10, -1, true);
          console.log('Nearest neighbors to sum with distances', neighborsAndDistances);
      }
    }

Development
------------

    npm install -g node-gyp
    node-gyp rebuild

Tests
-----

Run tests with `make test`.

License
-------

The MIT License (MIT)

Copyright (c) 2016 Jim Kang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
