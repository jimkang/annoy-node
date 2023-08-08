annoy-node
==================

[![Build Status](https://travis-ci.org/jimkang/annoy-node.svg?branch=master)](https://travis-ci.org/jimkang/annoy-node)

Node bindings for [Annoy](https://github.com/spotify/annoy), an efficient Approximate Nearest Neighbors implementation written in C++.

Version 4.0.0 requires Node 14 or Node 16 and does not yet work on 18.

Status: Tests pass, including one that loads 3 million vectors, but API coverage is not complete. Run on OS X and Linux with Node 8, 10, and 12. Not tried on Windows yet. Support for Node 6.3 and 4.6 ended at version 2.0.1 of this package.

All of the [Python API](https://github.com/spotify/annoy#full-python-api) methods are implemented. The names are camel cased, JavaScript-style.

  - `addItem`
  - `build`
  - `save`
  - `load`
  - `unload`
  - `getItem`
  - `getNNsByVector`
  - `getNNsByItem`
  - `getNItems`
  - `getDistance`

There are a few minor differences in behavior:

- If you set the "include distances" param (the fourth param) when calling `getNNsByVector` and `getNNsByItem`, rather than returning a 2D array containing the neighbors and distances, it will return an object with the properties `neighbors` and `distances`, each of which is an array.
- `get_item_vector` in with the Python API is just called `getItem` here.

Installation
------------

On Linux, if you don't already have Python 2.7 and g++ 4.8, you need to install them. Here's how you do it on Ubuntu:

    (sudo) apt-get install python2.7
    (sudo) apt-get install g++-4.8
    npm config set python /path/to/executable/python2.7

Then, symlink g++ somewhere it can be found:

    ln -s /usr/bin/g++-4.8 /usr/local/bin/g++

On OS X, they should already be there.

Then:

    npm install annoy

Usage
-----

    var Annoy = require('annoy');
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

          var neighbors = annoyIndex2.getNNsByVector(sum, 10, -1, false);
          console.log('Nearest neighbors to sum', neighbors);

          var neighborsAndDistances = annoyIndex2.getNNsByVector(sum, 10, -1, true);
          console.log('Nearest neighbors to sum with distances', neighborsAndDistances);
      }
    }

Development
------------

    npm install -g node-gyp
    node-gyp rebuild

Run `eslint .` and `make prettier` before committing.

Tests
-----

Run tests with `make test`.

You can also run tests individually:

- This is a short baseline test: `node tests/smalltest.js`
- This is a test that uses 70K 200-dimension vectors: `node tests/basictests.js`

There is also a `big-test` target that is not a dependency of the `test` target. It loads about 3 million 300-dimension vectors. It takes about six minutes to run on good-for-2016 hardware. Before you can run it, you need to download [GoogleNews-vectors-negative300.bin](https://drive.google.com/file/d/0B7XkCwpI5KDYNlNUTTlSS21pQmM/edit?usp=sharing) to `tests/data`.

Then, you can run `make tests/data/GoogleNews-vectors-negative300.json`, which takes a while, and gets the test data ready for the big test. (See comment about running that in the Makefile.) Then, `make big-test`.

Contributors
------------

Thanks to:

- [mbuszka](https://github.com/mbuszka) for [updating the wrapper to the latest Annoy (with Manhattan distance) and updating the random number generator](https://github.com/jimkang/annoy-node/pull/4).
- [aaaton](https://github.com/aaaton) for [updating the example code so that it works](https://github.com/jimkang/annoy-node/pull/1).
- [kornesh](https://github.com/kornesh) for [updating annoylib.h](https://github.com/jimkang/annoy-node/pull/10) to match the [Annoy of 2020-07-19](https://github.com/spotify/annoy/commit/7f2562add33eeb217dcdc755520c201aefc1b021).
- [Benjaminrivard](https://github.com/Benjaminrivard) for [updating the wrapper for Node 14](https://github.com/jimkang/annoy-node/pull/13) and testing the thread support.
- [S4N0I](https://github.com/S4N0I) for adding context-awareness so that it can be used in threads.

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
