# If you do not have node-gyp installed, then you need to do this first:
#    pkg install www/npm python
#    npm install -g node-gyp

all:
	node-gyp configure
	env CXX=c++ node-gyp build
	cp -f ./build/*/nodetcl.node .

clean:
	node-gyp clean
	rm -rf build
	rm -f nodetcl.node

install:
	node-gyp install

uninstall:
	node-gyp uninstall

test:
	cp -f ./build/*/nodetcl.node example
	node example/example1.js
	node example/example2.js
	node example/example3.js
