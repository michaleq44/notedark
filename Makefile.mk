PROJECT_VERSION = 1.0.0
SOURCE_DIR = $(shell pwd)

debug:
	rm -rf build/debug
	mkdir -p build/debug
	gcc -c src/main.c -o build/debug/main.o -Iinc
	windres src/res.rc -o build/debug/res.o
	gcc build/debug/main.o build/debug/res.o -o build/debug/notedark.exe -lkernel32 -luser32 -lgdi32
	cd build/debug ; ./notedark

_r:
	rm -rf build/release
	mkdir -p build/release
	gcc -c src/main.c -o build/release/main.o -Iinc
	windres src/res.rc -o build/release/res.o
	gcc build/release/main.o build/release/res.o -o build/release/notedark.exe -lkernel32 -luser32 -lgdi32 -mwindows

release: _r
	cd build/release ; ./notedark

pkg: _r
	rm -rf "release/v${PROJECT_VERSION}"
	mkdir -p "release/v${PROJECT_VERSION}/notedark v${PROJECT_VERSION}-win64"
	cp -f "build/release/notedark.exe" "README.md" "LICENSE" "release/v${PROJECT_VERSION}/notedark v${PROJECT_VERSION}-win64"
	cd "release/v${PROJECT_VERSION}/notedark v${PROJECT_VERSION}-win64" ; zip "../notedark v${PROJECT_VERSION}-win64.zip" notedark.exe LICENSE README.md
	cmd /C "iscc release/setup.iss /Dk=${SOURCE_DIR}"