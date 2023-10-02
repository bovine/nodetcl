ACLOCAL_AMFLAGS = -I autotools_m4 --install

CLEANFILES = *~ ./*.o build/*.o src/*.o
DISTCLEANFILES = *~ .DS_Store autoscan.log npm-debug.log .lock-wscript

AM_CPPFLAGS = -I$(srcdir)
AM_CFLAGS = -Wall
AM_LIBTOOL_LINKFLAGS = -module -no-undefined
AM_LDFLAGS = @AM_LDFLAGS@

NODE_WAF = @NODE_WAF@
NODE_BIN = @NODE_BIN@

all: nodetcl.node

configure_with_node_waf: wscript
	$(NODE_WAF) configure
.PHONY: configure_with_node_waf

build_with_node_waf: wscript configure_with_node_waf
	$(NODE_WAF) build
.PHONY: build_with_node_waf

all_with_node_waf: configure_with_node_waf build_with_node_waf
	cp -f ./build/*/nodetcl.node .
.PHONY: all_with_node_waf

build/Release/nodetcl.node: all_with_node_waf
	@if test ! -d build/Release; then \
		$(MKDIR_P) build/Release; \
	fi
	@if test -e ./build/Debug/nodetcl.node -a ! -e ./build/Release/nodetcl.node; then \
		cp -f ./build/Debug/nodetcl.node ./build/Release; \
	fi

clean_with_node_waf: wscript configure_with_node_waf
	$(NODE_WAF) clean
	rm -rf build || rmdir build
	rm -f nodetcl.node
	test -e .lock-wscript && rm -f .lock-wscript
.PHONY: clean_with_node_waf

clean-local:
	rm -rf autom4te.cache || rmdir autom4te.cache
	rm -rf .waf-1.*.*-* || rmdir .waf-1.*.*-*
	rm -rf node_modules || rmdir node_modules
.PHONY: clean-local

install_with_node_waf: wscript configure_with_node_waf
	$(NODE_WAF) install
.PHONY: install_with_node_waf

uninstall_with_node_waf: wscript configure_with_node_waf
	$(NODE_WAF) uninstall
.PHONY: uninstall_with_node_waf

test_with_node_waf: all_with_node_waf
	cp -f ./build/*/nodetcl.node example
	node example/example1.js
	node example/example2.js
	node example/example3.js
.PHONY: test_with_node_waf

src/nodetcl_1.o: src/nodetcl.cc
	$(CXX) $(CXXFLAGS) $(DEFS) $(CPPFLAGS) $(AM_CPPFLAGS) $(AM_CFLAGS) -c src/nodetcl.cc -o src/nodetcl_1.o

src/nodetcl.node: src/nodetcl_1.o
	$(LIBTOOL) --mode=link $(CXX) $(CXXFLAGS) $(LDFLAGS) $(AM_LDFLAGS) $(AM_LIBTOOL_LINKFLAGS) src/nodetcl_1.o -o src/nodetcl.node $(LIBS)

nodetcl.node: src/nodetcl.node
	test -e src/nodetcl.node && cp -f src/nodetcl.node .

example/nodetcl.node: src/nodetcl.node
	test -e src/nodetcl.node && cp -f src/nodetcl.node example

check-local: example/nodetcl.node
	$(NODE_BIN) example/example1.js
	$(NODE_BIN) example/example2.js
	$(NODE_BIN) example/example3.js
.PHONY: check-local

compare: src/nodetcl.node build/Release/nodetcl.node
	@echo "running cmp..."
	@if cmp -b src/nodetcl.node build/Release/nodetcl.node; then \
		echo "waf-built nodetcl.node and autotools-built nodetcl.node are identical"; \
	else \
		echo "waf-built nodetcl.node and autotools-build nodetcl.node came out different"; \
		exit 1; \
	fi
.PHONY: compare
