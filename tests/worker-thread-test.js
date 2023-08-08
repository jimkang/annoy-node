var test = require('tape');
const { StaticPool } = require('node-worker-threads-pool')

var workerPath = __dirname + '/testworker.js';

test('Worker thread test', workerThreadTest);

async function workerThreadTest(t) {
    const workerPool = new StaticPool({
        size: 2,
        task: workerPath
    });

    const idsToLookUp = [0, 1]
    const indexLookups = idsToLookUp.map(id => workerPool.exec(id))
    console.log("Lookup results: ", (await Promise.all(indexLookups)))
    workerPool.destroy()
    t.end()
}
