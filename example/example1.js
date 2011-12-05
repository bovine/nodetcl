//
// Demonstrates how to create a Tcl interpreter and then invoke a
// simple Tcl command from JavaScript.
//

var tcl = require('./nodetcl.node');
var interp = new tcl.NodeTcl();
console.log(interp.eval("expr 6*7")); 


