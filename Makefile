all:
	node-waf configure
	node-waf build
	cp -f ./build/default/nodetcl.node .

clean:
	rm -rf build
	rm -f nodetcl.node
