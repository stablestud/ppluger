compile: builddir
	meson compile -C builddir
builddir: meson.build
	meson setup builddir
install: builddir
	meson install -C builddir
reconfig: reconfigure
reconfigure: meson.build
	meson --reconfigure builddir
config:
	meson configure builddir
clean:
	rm -rf builddir
dist: builddir
	meson dist -C builddir
	

.PHONY: compile config clean reconfigure install
