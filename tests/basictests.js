/* global __dirname */

var test = require('tape');
var Annoy = require('../index');
var fs = require('fs');
var ndjson = require('ndjson');

if (process.argv.length < 3) {
  console.log('Usage: node tests/basictests.js <test config file path>')
  process.exit();
}

var config = require('./' + process.argv[2]);

const vectorJSONPath = __dirname + '/data/' + config.vectorJSONFile;
const annoyIndexPath = __dirname + '/data/' + config.annoyFile;
const dimensions = config.dimensions;

var indexesForWords = {};
var wordsForIndexes = {};
var vectorCount = 0;

test('Adding vectors to Annoy', addTest);
test('Using vectors from Annoy', usingTest);

function addTest(t) {
  var annoyIndex = new Annoy(dimensions, 'Euclidean');

  fs.createReadStream(vectorJSONPath)
    .pipe(ndjson.parse({strict: false}))
    .on('data', addToAnnoy)
    .on('end', checkAdded);

  function addToAnnoy(wordVectorPair) {
    indexesForWords[wordVectorPair.word] = vectorCount;
    wordsForIndexes[vectorCount] = wordVectorPair.word;
    annoyIndex.addItem(vectorCount, wordVectorPair.vector);
    // process.stdout.write('+');
    vectorCount += 1;
    // console.log(wordVectorPair.word, wordVectorPair.vector);
  }

  function checkAdded() {
    t.ok(vectorCount > 0, 'More than one vector was added to the index.');
    t.equal(annoyIndex.getNItems(), vectorCount, 'The index\'s total vector count is correct.');
    annoyIndex.build();
    annoyIndex.save(annoyIndexPath);
    t.end();
  }
}

function usingTest(t) {
  var annoyIndex = new Annoy(dimensions, 'Euclidean');
  annoyIndex.load(annoyIndexPath);

  t.equal(
    annoyIndex.getNItems(),
    vectorCount,
    'The loaded index\'s total vector count is correct: ' + vectorCount
  );

  var v1 = annoyIndex.getItem(indexesForWords[config.lookupWord1]);
  var v2 = annoyIndex.getItem(indexesForWords[config.lookupWord2]);
  checkVector(v1);
  checkVector(v2);

  var sumVector = [];
  for (var i = 0; i < v1.length; ++i) {
    sumVector.push(v1[i] + v2[i]);
  }

  console.log('Sum:', sumVector);
  var nnResult = annoyIndex.getNNsByVector(sumVector, 100, -1, true);
  console.log('Neighbors and distances:', nnResult);

  t.equal(typeof nnResult, 'object', 'NN result is an object.');

  t.ok(Array.isArray(nnResult.neighbors), 'NN result has a neighbors array.');
  t.equal(nnResult.neighbors.length, 100, 'Correct number of neighbors is returned.');
  t.ok(
    nnResult.neighbors.every((val) => typeof val  === 'number'),
    'Neighbors contains all numbers.'
  );

  t.ok(Array.isArray(nnResult.distances), 'NN result has a distances array.');
  t.equal(nnResult.distances.length, 100, 'Correct number of distances is returned.');
  t.ok(
    nnResult.distances.every((val) => typeof val  === 'number'),
    'Distances contains all numbers.'
  );

  console.log('Third closest neighbor:', wordsForIndexes[nnResult.neighbors[2]]);

  t.end();

  function checkVector(vector) {
    console.log(vector);
    t.equal(vector.length, dimensions, 'Vector has correct number of dimensions.');
    t.ok(vector.every((val) => typeof val  === 'number'), 'Vector contains all numbers.');
  }
}
