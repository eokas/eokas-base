
rm ./_build
mkdir _build
cd _build

cmake ..

cmake --build .
cmake --install . --prefix ../_install
