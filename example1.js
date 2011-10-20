var tcl = require('./nodetcl.node');
var interp = new tcl.NodeTcl();
console.log(interp.eval("expr 6*7")); 


