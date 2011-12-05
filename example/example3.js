//
// show that the tcl event loop works by creating some after
// events and then repeatedly invoking the tcl event
// handler
//

var tcl = require('./nodetcl.node');
var interp = new tcl.NodeTcl();

interp.eval("puts 3...; after 1000 {puts 2...}; after 2000 {puts 1...}; after 3000 {puts goodbye; exit 0}")

//
// tick routine calls tcl event loop to process events -- if it comes back
// having done work, we reschedule it for the next tick of node's event loop.
// 
// if it didn't do work, we ask for a timer callback for a bit
// 
function tick() {
    if (interp.process_events(true)) {
        process.nextTick(tick);
    } else {
        setTimeout(tick, 10);
    }
}

process.nextTick(tick);

