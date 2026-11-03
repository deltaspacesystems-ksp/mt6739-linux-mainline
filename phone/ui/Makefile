CC ?= cc
CXX ?= c++
CFLAGS ?= -O2 -g
CFLAGS += -pthread -std=c11 -Wall -Wextra -Werror
PKGS = cairo pangocairo wayland-client
CPPFLAGS += -Isrc -Ibuild $(shell pkg-config --cflags $(PKGS))
LDLIBS += -pthread $(shell pkg-config --libs $(PKGS)) -lm
XDG_XML ?= /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml
DECOR_XML ?= /usr/share/wayland-protocols/unstable/xdg-decoration/xdg-decoration-unstable-v1.xml
SRC = src/main.c src/motion.c src/model.c src/sampler.c src/render.c src/wayland.c src/desktop.c src/gestures.c build/icons.c build/xdg-shell-protocol.c build/xdg-decoration-protocol.c build/wlr-layer-shell-protocol.c build/wlr-foreign-toplevel-management-protocol.c

.PHONY: all previews clean test test-desktop
all: build/microfw-shell
build:
	mkdir -p build
build/icons.c: tools/compile_icons.py $(wildcard assets/icons/*.svg) | build
	python3 tools/compile_icons.py $@
build/xdg-shell-client-protocol.h: $(XDG_XML) | build
	wayland-scanner client-header $< $@
build/xdg-shell-protocol.c: $(XDG_XML) | build
	wayland-scanner private-code $< $@
build/xdg-decoration-client-protocol.h: $(DECOR_XML) | build
	wayland-scanner client-header $< $@
build/xdg-decoration-protocol.c: $(DECOR_XML) | build
	wayland-scanner private-code $< $@
build/wlr-layer-shell-client-protocol.h: protocols/wlr-layer-shell-unstable-v1.xml | build
	wayland-scanner client-header $< $@
build/wlr-layer-shell-protocol.c: protocols/wlr-layer-shell-unstable-v1.xml | build
	wayland-scanner private-code $< $@
build/wlr-foreign-toplevel-management-client-protocol.h: protocols/wlr-foreign-toplevel-management-unstable-v1.xml | build
	wayland-scanner client-header $< $@
build/wlr-foreign-toplevel-management-protocol.c: protocols/wlr-foreign-toplevel-management-unstable-v1.xml | build
	wayland-scanner private-code $< $@
build/microfw-shell: $(SRC) src/ui.h src/motion.h src/gestures.h build/xdg-shell-client-protocol.h build/xdg-decoration-client-protocol.h build/wlr-layer-shell-client-protocol.h build/wlr-foreign-toplevel-management-client-protocol.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS) $(LDLIBS)
previews: all
	./build/microfw-shell --demo --preview build/desktop.png
	./build/microfw-shell --demo --page switcher --preview build/switcher.png
	./build/microfw-shell --demo --page control --preview build/control.png
	./build/microfw-shell --demo --page settings --preview build/settings.png
build/test-gestures: src/gestures.c src/gestures.h tools/test_gestures.c | build
	$(CC) -Isrc -std=c11 -Wall -Wextra -Werror -fsanitize=undefined src/gestures.c tools/test_gestures.c -o $@ -lm
build/abi-gestures.o: src/gestures.c src/gestures.h | build
	$(CC) $(CPPFLAGS) -std=c11 -Wall -Wextra -Werror -c $< -o $@
build/abi-model.o: src/model.c src/ui.h | build
	$(CC) $(CPPFLAGS) -std=c11 -Wall -Wextra -Werror -c $< -o $@
build/test-cpp: tools/test_cpp.cpp src/ui.h src/gestures.h build/abi-gestures.o build/abi-model.o
	$(CXX) $(CPPFLAGS) -std=c++17 -Wall -Wextra -Werror $< build/abi-gestures.o build/abi-model.o -o $@ $(LDLIBS)
test: build/test-gestures build/test-cpp all
	./build/test-gestures
	./build/test-cpp
	bash tools/test_wayland.sh
test-desktop: all
	bash tools/test_desktop.sh
clean:
	rm -f build/microfw-shell build/test-gestures build/test-cpp build/abi-*.o build/icons.c build/*-protocol.c build/*-client-protocol.h build/*.png

.PHONY: test-render-cache bench-render
build/render-reference.o: tools/render_reference_before_cache.c src/ui.h | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -Dmfw_render=mfw_render_reference -c $< -o $@
build/test-render-cache: tools/test_render_cache.c src/render.c build/icons.c build/render-reference.o
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)
test-render-cache: build/test-render-cache
	./build/test-render-cache
build/bench-render: tools/bench_render.c src/render.c build/icons.c
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)
bench-render: build/bench-render
	./build/bench-render

.PHONY: test-sampler
build/test-sampler: tools/test_sampler.c src/sampler.c src/sampler.h src/ui.h | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -fsanitize=undefined tools/test_sampler.c src/sampler.c -o $@ $(LDLIBS)
test-sampler: build/test-sampler
	./build/test-sampler

.PHONY: test-motion
build/test-motion: src/motion.c src/motion.h src/ui.h tools/test_motion.c | build
	$(CC) $(CPPFLAGS) $(CFLAGS) -fsanitize=address,undefined src/motion.c tools/test_motion.c -o $@ $(LDLIBS)
test-motion: build/test-motion
	./build/test-motion
