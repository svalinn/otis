Once Through Irradation System - OTIS
====
A tool to produce input that allows the calculation of irradiated fluids in pipes. OTIS can take either an acis *.sat file as input or a marked up dot file. The underlying implementation builds a network of connected volumes, and then using a depth first search algorith, determines all of the unique routes between two nominated target volumes. Output is Alara ready flux and input files.

Build & Install
====
To compile:

```bash
   %> mkdir bld
   %> cd bld
   %> cmake ../. -DCGM_DIR=<path to cgm install> -DCUBIT_DIR=<path to cubit install> -CMAKE_INSTALL_PREFIX=<path to where you want it>
   %> make
   %> make install
```

Running
=====
In order to run OTIS you will need either an appropriately marked up input file (either acis or dot) and an MCNP output file with spectra. 
```   
   ./otis -s <source volume id> 
          -f <target volume id>
	  -c <geometry input name & extension>
	  -m <mcnp output filename> 
	  -n <no irradiation just produce connectivity data>
	  -h help
```

The below dot file can be used as example input. The input says that volume 1 is connected to 2 and 3, volume three is connected to 2 and volume 2 is connected to 4. OTIS relies on a Directed Acyclic Graph method, so the direction of connectivity matter.

```
// commment
digraph g {
 // connections
 1 -> 2;
 1 -> 3;
 3 -> 2;
 2 -> 4;

 // residence times and id numbers
 1 [label="vol 1 40 s"]
 2 [label="vol 2 10.1 s"]
 3 [label="vol 3 3 s"]
 4 [label="vol 4 1.0 s"]
}
```

For example, let us say that we are interested in the unique routes between nodes 1 and 4, then there are 2 unique routes, 
1 -> 2 -> 4 and 1 -> 3 -> 2 -> 4. This would result in two irradation files for Alara. It is up to the end user to determine how
to weight the average of these two results.