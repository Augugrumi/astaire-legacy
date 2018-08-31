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
## Building with docker
You can build on different distros using docker. Currently, we offer pre-configured images for:
- Ubuntu 16.04
- Ubuntu 18.04
- Ubuntu 18.10
- Arch Linux

These are the distro we officially support. If you want, you can add Dockerfiles in the [dockerbuilds repository](https://github.com/Augugrumi/Astaire-dockerbuilds) to integrate them in our CI system.

To build with docker, clone the repository, and then, in the root of the project, run:
```bash
docker run --rm -v $(pwd):/build augugrumi/astaire-dockerbuilds:ubuntu16.04
```
Available tags are:
- `ubuntu16.04`
- `ubuntu18.04`
- `ubuntu18.10`
- `arch`

# License

See LICENSE file for more details
