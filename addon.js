var addon = require('bindings')('addon');

var annoyPath = __dirname + '/test.annoy';
var obj = new addon.Annoy(10, 'Angular');
// console.log( obj.plusOne() ); // 11
// console.log( obj.plusOne() ); // 12
// console.log( obj.plusOne() ); // 13

// console.log( obj.multiply().value() ); // 13
// console.log( obj.multiply(10).value() ); // 130

// var newobj = obj.multiply(-1);
// console.log( newobj.value() ); // -13
// console.log( obj === newobj ); // false
obj.addItem(0, [-5.0, -4.5, -3.2, -2.8, -2.1, -1.5, -0.34, 0, 3.7, 6]);
obj.addItem(1, [5.0, 4.5, 3.2, 2.8, 2.1, 1.5, 0.34, 0, -3.7, -6]);
obj.build();
obj.save(__dirname + '/test.annoy');

var obj2 = new addon.Annoy(10, 'Angular');
if (obj2.load(annoyPath)) {
  console.log('Loaded annoy index!');
}
