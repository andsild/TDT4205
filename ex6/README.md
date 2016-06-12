## Comments:
I solved this in a short time, and the quality of the code suffers from it  

I did *not* remember in time that there is special handling for > 6 variables. This will not crash the program, but 7th variables etc will not be assigned a value when calling  functions  

## Feedback:
No worries about the T/F labelling of edges, but do mind the order of c,d in
the for case: the induction variable modification is usually not run prior to the
first iteration in conventional for loops.
