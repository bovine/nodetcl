all:
	/usr/local/bin/node-waf configure
	/usr/local/bin/node-waf build
	cp -f ./build/*/nodetcl.node .

clean:
	/usr/local/bin/node-waf clean
	rm -rf build
	rm -f nodetcl.node

install:
	/usr/local/bin/node-waf install

uninstall:
	/usr/local/bin/node-waf uninstall

test:
	cp -f ./build/*/nodetcl.node example
	node example/example1.js
	node example/example2.js
	node example/example3.js
