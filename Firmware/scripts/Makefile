unit_tests:
	mkdir -p ../../unix/x86
	cd ../../unix/x86; cmake ../../Firmware -DTARGET_PLATFORM:STRING=x86
	make -C ../../unix/x86 -j5
	../../unix/x86/build/x86/bin/unit_tests
