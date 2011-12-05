//
// Demonstrates how to register an anonymous JavaScript function into
// the namespace of a Tcl interpreter.  Then invoke it from Tcl with a
// value returned back to JavaScript.
//

var tcl = require('./nodetcl.node');
var interp = new tcl.NodeTcl();

interp.proc("simpleCallback", function(arg1, arg2) {
    console.log("I got the callback!");
    console.log("Our arguments were: " + arg1 + ", " + arg2);
    return "arbitrary result";
});

var result = interp.eval("simpleCallback alpha bravo");
console.log(result);

