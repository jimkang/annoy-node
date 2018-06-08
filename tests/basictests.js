/* global __dirname process */

var test = require('tape');
var Annoy = require('../index');
var fs = require('fs');
var ndjson = require('ndjson');

if (process.argv.length < 3) {
  console.log('Usage: node tests/basictests.js <test config file path>');
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
    t.ok(annoyIndex.save(annoyIndexPath), 'Saved successfully.');
    annoyIndex.unload();
    t.end();
  }
}

function usingTest(t) {
  var annoyIndex = new Annoy(dimensions, 'Euclidean');
  t.ok(annoyIndex.load(annoyIndexPath), 'Loaded successfully.');

  t.equal(
    annoyIndex.getNItems(),
    vectorCount,
    'The loaded index\'s total vector count is correct: ' + vectorCount
  );

  t.equal(
    annoyIndex.getDistance(
      indexesForWords[config.lookupWord1], indexesForWords[config.lookupWord2]
    )
    .toPrecision(7),
    config.distanceBetweenWord1And2.toString(),
    'getDistance calculates correct distance between items for ' +
      config.lookupWord1 + ' and ' + config.lookupWord2
  );

  var v1 = annoyIndex.getItem(indexesForWords[config.lookupWord1]);
  var v2 = annoyIndex.getItem(indexesForWords[config.lookupWord2]);
  checkVector(v1);
  checkVector(v2);

  var sumVector = [];
  for (var i = 0; i < v1.length; ++i) {
    sumVector.push(v1[i] + v2[i]);
  }

  // console.log('Sum:', sumVector);
  var nnResult = annoyIndex.getNNsByVector(sumVector, 100, -1, true);
  // console.log('Neighbors and distances:', nnResult);

  checkNNResult('nnResult', nnResult);

  // console.log('Third closest neighbor:', wordsForIndexes[nnResult.neighbors[2]]);

  var nnResultByItem = annoyIndex.getNNsByItem(
    indexesForWords[config.indexLookupWord], 100, -1, true
  );
  checkNNResult('nnResultByItem', nnResultByItem);

  t.end();

  function checkNNResult(resultName, result) {
    t.equal(typeof result, 'object', resultName + ' is an object.');
    t.ok(Array.isArray(result.neighbors), resultName + ' has a neighbors array.');
    t.equal(result.neighbors.length, 100, 'Correct number of neighbors is returned.');
    t.ok(
      result.neighbors.every((val) => typeof val  === 'number'),
      'Neighbors contains all numbers.'
    );

    t.ok(Array.isArray(result.distances), resultName + ' has a distances array.');
    t.equal(result.distances.length, 100, 'Correct number of distances is returned.');
    t.ok(
      result.distances.every((val) => typeof val  === 'number'),
      'Distances contains all numbers.'
    );
  }

  function checkVector(vector) {
    // console.log(vector);
    t.equal(vector.length, dimensions, 'Vector has correct number of dimensions.');
    t.ok(vector.every((val) => typeof val  === 'number'), 'Vector contains all numbers.');
  }
}
