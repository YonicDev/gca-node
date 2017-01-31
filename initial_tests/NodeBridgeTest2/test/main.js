var gca_node = require('../build/Release/BridgeTest_2.node')
var proceed = false;
if(gca_node.Setup()>0) {
    console.log("I found an available GameCube adapter!");
    proceed = true;
} else {
    console.error("I haven't found an available GameCube adapter. I can't tell the reason just yet.");
}
if(proceed) {
    console.log("I'm going to try and connect to the adapter.");
    var code = gca_node.Load();
    if(code===0) {
        console.log("I did it! Hooray!");
    } else {
        console.log("Oops! Something happened: " + code);
    }
}
