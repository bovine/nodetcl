all:
	node-waf configure
	node-waf build
	cp -f ./build/*/nodetcl.node .

clean:
	node-waf clean
	rm -rf build
	rm -f nodetcl.node

install:
	node-waf install

uninstall:
	node-waf uninstall

test:
	cp -f ./build/*/nodetcl.node example
	node example/example1.js
	node example/example2.js
	node example/example3.js
