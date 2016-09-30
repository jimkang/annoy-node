var addon = require('bindings')('addon');

var annoyPath = __dirname + '/test.annoy';

var obj = new addon.Annoy(10, 'Angular');

obj.addItem(0, [-5.0, -4.5, -3.2, -2.8, -2.1, -1.5, -0.34, 0, 3.7, 6]);
obj.addItem(1, [5.0, 4.5, 3.2, 2.8, 2.1, 1.5, 0.34, 0, -3.7, -6]);
obj.addItem(2, [0, 0, 0, 0, 0, -1, -1, -0.2, 0.1, 0.8]);
obj.build();
obj.save(annoyPath);

read();

function read() {
  var obj2 = new addon.Annoy(10, 'Angular');

  if (obj2.load(annoyPath)) {
    console.log('Loaded annoy index!');
    var v1 = obj2.getItem(0);
    var v2 = obj2.getItem(1);
    console.log('Gotten vectors:', v1, v2);
    var sum = [];
    for (var i = 0; i < v1.length; ++i) {
      sum.push(v1[i] + v2[i]);
    }
    console.log('Sum:', sum);
    var neighbors = obj2.getNNsByVector(sum, 10, -1, false);
    console.log('Nearest neighbors to sum', neighbors);
    var neighborsAndDistances = obj2.getNNsByVector(sum, 10, -1, true);
    console.log('Nearest neighbors to sum with distances', neighborsAndDistances);
  }
}

