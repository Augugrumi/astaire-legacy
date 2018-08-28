Astaire
===

# How to build
First of all, generate all the autotools script with this command:
```bash
aclocal -I m4 --install && autoconf && automake --add-missing
```

At this point, the compilation is simply:
```bash
./configure
make
```

If you want to install the program (this operation needs root access):
```bash
sudo make install
```