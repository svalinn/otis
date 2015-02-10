otis
====

A tool to perform once through water irradiations in CAD based pipe geometries

build
====
To compile:

```bash
   %> mkdir bld
   %> cd bld
   %> cmake ../. -DCGM_DIR=<path to cgm install> -DCUBIT_DIR=<path to cubit install> -CMAKE_INSTALL_PREFIX=<path to where you want it>
   %> make
   %> make install
```
