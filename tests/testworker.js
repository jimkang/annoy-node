const { parentPort } = require('worker_threads');
var Annoy = require('../index');

var annoyPath = __dirname + '/data/test.annoy';

const annoyIndex = new Annoy(10, 'Angular')

if(!annoyIndex.load(annoyPath)) {
    throw new Error('Loading annoy index failed!')
} else {
    console.log('Successfully loaded annoy index.')
}

parentPort.on('message', async (id) => {
    // return the result to the main thread
    const result = annoyIndex.getNNsByItem(id, 10, -1, false)
    parentPort.postMessage(result);
});