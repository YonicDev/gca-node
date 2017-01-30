var addon = require('../build/Release/BridgeTest_1.node');

console.log(`3 plus 5 equals ${addon.AddPointer(3,5)}`);
console.log('GameCube ID numbers are: ' + addon.GetGameCubeIDs());
