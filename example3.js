//
// show that the tcl event loop works by creating some after
// events and then repeatedly invoking the tcl event
// handler
//

var tcl = require('./nodetcl.node');
var interp = new tcl.NodeTcl();

interp.eval("puts 3...; after 1000 {puts 2...}; after 2000 {puts 1...}; after 3000 {puts goodbye; exit 0}")

// we need setInterval here to invoke interp.process_events, not
// this tight while loop
while (true) {
    interp.process_events(true);
}

