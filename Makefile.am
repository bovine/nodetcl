ACLOCAL_AMFLAGS = -I autotools_m4 --install

CLEANFILES = *~ ./*.o build/*.o src/*.o
DISTCLEANFILES = *~ .DS_Store autoscan.log npm-debug.log

AM_CPPFLAGS = -I$(srcdir)
AM_CFLAGS = -Wall

NODE_WAF = @NODE_WAF@

all_with_node_waf:
	$(NODE_WAF) configure
	$(NODE_WAF) build
	cp -f ./build/*/nodetcl.node .
.PHONY: all_with_node_waf

clean_with_node_waf:
	$(NODE_WAF) clean
	rm -rf build || rmdir build
	rm -f nodetcl.node
.PHONY: clean_with_node_waf

clean-local:
	rm -rf autom4te.cache || rmdir autom4te.cache
	rm -rf .waf-1.*.*-* || rmdir .waf-1.*.*-*
	rm -rf node_modules || rmdir node_modules
.PHONY: clean-local

install_with_node_waf:
	$(NODE_WAF) install
.PHONY: install_with_node_waf

uninstall_with_node_waf:
	$(NODE_WAF) uninstall
.PHONY: uninstall_with_node_waf

test: all_with_node_waf
	cp -f ./build/*/nodetcl.node example
	node example/example1.js
	node example/example2.js
	node example/example3.js
.PHONY: test

src/nodetcl_1.o: src/nodetcl.cc
	$(CXX) $(CXXFLAGS) $(DEFS) $(CPPFLAGS) $(AM_CPPFLAGS) $(AM_CFLAGS) -c src/nodetcl.cc -o src/nodetcl_1.o

src/nodetcl.node: src/nodetcl_1.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) src/nodetcl_1.o -o src/nodetcl.node $(LIBS)

nodetcl.node: src/nodetcl.node
	test -e src/nodetcl.node && cp -f src/nodetcl.node .
