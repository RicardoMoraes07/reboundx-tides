Fork version of the tides implementation in reboundx.

This folder should be inside a regular rebound folder.

The folder examples should be placed on the main rebound folder.

The Makefile should be changed.

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Updates:

Now the spin of the orbiting bodies are in pseudo-synchronization.

The central bodies' spin doesn't evolve.

The routine was improved to account for an initial \vec{Omega} = (Omega_x, Omega_y, Omega_z).

Files modified:
-> examples/tides_constant_time_lag/problem.c
-> examples/tides_constant_time_lag/Makefile
-> src/tides_constant_time_lag.c
-> src/code.c

The values used on problem.c for the orbiting body are not realistic and it was taken only for testing.
