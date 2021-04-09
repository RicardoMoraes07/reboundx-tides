/**
 * Constant time lag model for tides (Hut 1981)
 *
 * In particular, this simulates post-main sequence tidal interactions between the Earth and Sun near its tip-RGB phase.
 * Definitely see the corresponding ipython example, as well as the documentation, for more explanations along the way of the various parameters and assumptions.
 
 
 **Ricardo
     diss = dissipation factor -> only for star
    Rgyr = gyration radius ->  only for star
    added at core.c in reboundx/src 
**Ricardo
 
 
 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "rebound.h"
#include "reboundx.h"

void heartbeat(struct reb_simulation* const sim);
    double tmax = 2.5e4; // years

int main(int argc, char* argv[]){
    struct reb_simulation* sim = reb_create_simulation();
    sim->G = 4*M_PI*M_PI;           // Units of AU, yr and Msun

    sim->integrator = REB_INTEGRATOR_IAS15; // IAS15 por padrão
    
    struct reb_particle sun = {0}; 
    sun.m = 0.86;                   // post-MS in Msun
    reb_add(sim, sun);

    struct reb_orbit eo = {0};      // for Earth
    eo.a = 1.0;                     // in AU
    eo.e = 0.03;
    double e_mass = 2.988e-6;       // if planets don't have mass, they don't raise any tides
    struct reb_particle ep = reb_tools_orbit_to_particle(sim->G, sun, e_mass, eo.a, eo.e, eo.inc, eo.Omega, eo.omega, eo.f);
    reb_add(sim, ep);
    reb_move_to_com(sim);
    
    // Add REBOUNDx Additional Effect
    struct rebx_extras* rebx = rebx_attach(sim);                                
    struct rebx_force* tides = rebx_load_force(rebx, "tides_constant_time_lag");
    rebx_add_force(rebx, tides);

    // We first have to give the body being tidally distorted a physical radius, or nothing will happen
    sim->particles[0].r = 0.85;     // AU
    sim->particles[1].r = 4.26352e-3;

    // At a minimum, have to set tctl_k1 (apsidal precession constant) parameter, which will add the conservative potential of the equilibrium tidal distortion

    rebx_set_param_double(rebx, &sim->particles[0].ap, "tctl_k1", 0.03); 
    rebx_set_param_double(rebx, &sim->particles[1].ap, "tctl_k1", 0.01); 
    // We add dissipation by adding a constant time lag tctl_tau
   rebx_set_param_double(rebx, &sim->particles[0].ap, "tctl_tau", 0.4);
   rebx_set_param_double(rebx, &sim->particles[1].ap, "tctl_tau", 0.1);

    rebx_set_param_double(rebx, &sim->particles[1].ap, "obliquity", 30); //obliquity in degrees

    // We also can set the angular rotation rate of bodies Omega. Here we explicitly set the star's to zero, but implementation will assume zero if not specified. 
    rebx_set_param_double(rebx, &sim->particles[0].ap, "Omega_x", 0.);
    rebx_set_param_double(rebx, &sim->particles[0].ap, "Omega_y", 0.);
    rebx_set_param_double(rebx, &sim->particles[0].ap, "Omega_z", 0.);
    
 
    
    
    

    // Run simulation
    sim->heartbeat        = heartbeat;
system("rm -v r.txt");    
    system("rm -v orbit.txt");
    reb_integrate(sim, tmax);
    rebx_free(rebx); 
    reb_free_simulation(sim);
}

void heartbeat(struct reb_simulation* const sim){

    // Output some information to the screen every 100th timestep
    if(reb_output_check(sim, 100.*sim->dt)){
        reb_output_timing(sim, tmax);
    }
    // Output the particle position to a file every timestep.
    const struct reb_particle* const particles = sim->particles;

    
    FILE* f = fopen("r.txt","a");
    fprintf(f,"%e\t%e\t%e\n",sim->t,particles[1].x, particles[1].vx);
    fclose(f);
   int erro;
     struct reb_particle com= sim->particles[0];
   struct reb_orbit  part = reb_tools_particle_to_orbit_err(sim->G, sim->particles[1],com,&erro); 
 
    FILE* bla = fopen("orbit.txt", "a");
    fprintf(bla, "%e\t%e\t%e\t\n", sim->t, part.a, part.e);
    fclose(bla); 
	

}
