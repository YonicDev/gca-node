gca_node = require("./build/Release/gca-node.node");

var loop;

loop = gca_node.Setup()
if(loop>0) {
    var loop = gca_node.Load();
    var count = 0;
    console.log(loop);
    while(loop===0&&count<10000) {
        console.log(gca_node.Process()[0]);
        count+=1;
    }
    console.log(gca_node.Stop());
} else {
    console.log("Aaah " + loop)
}
