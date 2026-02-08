build (Win32)

```bash
cp /path/to/twilight-princess.ciso orig/GZ2E01/.
python3 ./configure.py                                                                                          
ninja
cmake -B build/dusk -G "Visual Studio 17 2022" -A Win32
```

build (Linux/macOS)
```bash
cp /path/to/twilight-princess.ciso orig/GZ2E01/.
python3 ./configure.py                                                                                          
ninja
cmake -B build/dusk -GNinja
ninja -C build/dusk
```

