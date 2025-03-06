if defined SOURCE_DIR (
iscc "%SOURCE_DIR%\release\setup.iss" /DSrcDir="%SOURCE_DIR%" /DMyAppVersion="%APPVER%"
)else (
echo "Set environment variable SOURCE_DIR to the source directory (i.e. where the Makefile is)"
exit
)