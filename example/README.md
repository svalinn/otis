How to Run
=====
The problem provided is a simple example consisting of 4 nodes, 1,2,3 and 4. 

otis -s 1 -f 4 -c dot.dot -m mcnp_output

Running the output
=====
Once you have run the above command you will have a number of files availble to you, 

   * graph1.dot -> the unique routes graph
   * flux_# -> the flux files for Alara
   * test_# -> the Alara input decks

You can then run each Alara problem as
```
    alara test_1
    alara test_2
```