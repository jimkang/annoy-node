/* global process */

var fs = require('fs');
var ndjson = require('ndjson');
var level = require('level');
var Sublevel = require('level-sublevel');
var through2 = require('through2');
var test = require('tape');
var assertNoError = require('assert-no-error');
var Annoy = require('../index');

if (process.argv.length < 5) {
  console.log('Usage: node tools/test-word-index-db.js <line-delimited JSON file path> <db path> <Annoy path>');
  process.exit();
}

const vectorJSONPath = process.argv[2];
const dbPath = process.argv[3];
const annoyPath = process.argv[4];

var db = Sublevel(level(dbPath));
var indexesForWords = db.sublevel('indexes');
var wordsForIndexes = db.sublevel('words');
var annoyIndex = new Annoy(300, 'Euclidean');
annoyIndex.load(annoyPath);

var vectorCount = 0;

fs.createReadStream(vectorJSONPath)
  .pipe(ndjson.parse({strict: false}))
  .pipe(through2({objectMode: true}, runTestOnPair))
  .on('end', closeDb);

function runTestOnPair(wordVectorPair, enc, done) {
  test(wordVectorPair.word + '/' + vectorCount, testPair);

  function testPair(t) {
    indexesForWords.get(wordVectorPair.word, checkIndex);

    function checkIndex(error, index) {
      assertNoError(t.ok, error, 'No error while getting index for word.');
      t.equal(parseInt(index, 10), vectorCount, 'Index matches index in JSON.');
      var vector = annoyIndex.getItem(index);
      t.deepEqual(
        vector.map(toFixedDecimalString),
        wordVectorPair.vector,
        'Vector for index matches vector for same index in JSON.'
      );

      wordsForIndexes.get(index, checkWord);
    }

    function checkWord(error, word) {
      assertNoError(t.ok, error, 'No error while getting word for index.');
      t.equal(word, wordVectorPair.word, 'Word matches word in JSON.');

      vectorCount += 1;

      t.end();
      done();
    }
  }
}

function closeDb() {
  db.close(logDone);

  function logDone(error) {
    if (error) {
      console.error(error);
    }
    else {
      console.log('Done testing db.');
    }
  }
}

function toFixedDecimalString(num) {
  return num.toFixed(6);
}
