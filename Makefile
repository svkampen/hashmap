all:
	cd build; cmake ..
	+$(MAKE) -C build
