PROJECT_VERSION = 1.0.0
SOURCE_DIR = $(shell pwd)

debug:
	rm -rf build/debug
	mkdir -p build/debug
	gcc -c src/main.c -o build/debug/main.o -Iinc
	gcc -c src/menubar.c -o build/debug/menubar.o -Iinc
	windres src/res.rc -o build/debug/res.o
	gcc build/debug/main.o build/debug/menubar.o build/debug/res.o -o build/debug/notedark.exe -lkernel32 -luser32 -lgdi32 -lcomdlg32
	cd build/debug ; ./notedark

_r:
	rm -rf build/release
	mkdir -p build/release
	gcc -c src/main.c -o build/release/main.o -Iinc
	gcc -c src/menubar.c -o build/release/menubar.o -Iinc
	windres src/res.rc -o build/release/res.o
	gcc build/release/main.o build/release/menubar.o build/release/res.o -o build/release/notedark.exe -lkernel32 -luser32 -lgdi32 -lcomdlg32

release: _r
	cd build/release ; ./notedark

pkg: _r
	rm -rf "release/v${PROJECT_VERSION}"
	mkdir -p "release/v${PROJECT_VERSION}/notedark v${PROJECT_VERSION}-win64"
	cp -f "build/release/notedark.exe" "README.md" "LICENSE" "release/v${PROJECT_VERSION}/notedark v${PROJECT_VERSION}-win64"
	cd "release/v${PROJECT_VERSION}/notedark v${PROJECT_VERSION}-win64" ; zip -v9 "../notedark v${PROJECT_VERSION}-win64.zip" notedark.exe LICENSE README.md
	export APPVER=${PROJECT_VERSION} ; ./release/innosetupwrap.bat