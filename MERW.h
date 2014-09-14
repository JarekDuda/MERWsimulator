/* 
 * File:   MERW.h
 * Author: Jarek
 *
 * Created on September 8, 2014, 12:11 PM
 */


#ifndef MERW_H
#define	MERW_H

#include <cstdlib>
#include <vector>
#include <math.h>
#include <iostream>
using namespace std;

struct edge {float u,d,l,r,s;};                             // edge types
struct coord {int x, y, r, g, b;};

class MERW {
public:
    MERW(int, int, float, float, float);
    ~MERW();
    void printm(vector<float> *v);                          // print matrix
    void update_walkers();                                  // update number of walkers
    void make_step();
    void update();                                          // if small change: of gradients, added defect
    void init();                                            // if large step: of size, randomized defects
    void point_density(int,int);                            // change to point density in given point
    void gen_def();                                         // generate random defects

    int w,h, wh;                                            // width, height
    vector<float> * display;                                // current density to display
    vector<coord> walkers;                                  // walkers
    vector<float> self;                                     // self-loops (not a defect)
    bool MG, cont, cych, cycv;                              // MG: MERW/GRW, cont - if continuous evolution
    float defp, gradx, grady, maxv;                         // defect probability and gradients
    float accuracy ;                                        // for convergence of power method
    int steps, numbwalk;

    float lam;                                              // dominant eigenvalue     
    edge wg;                                                // weights
    vector<float> *rhoM, *rhoG;                             // stationary densities
    vector<edge> M, G;                                      // transition probabilities for MERW and GRW
    vector<float> *leig, *leigp, *reig, *reigp;             // left, right dominant eigenvector
    vector<float> *den, *pden;
private:
    float normalize(vector<float> *v);                       // normalize (L1 as for probability distribution)        
    float dif(vector<float> *v, vector<float> *w);           // some (L1) distance for checking convergence        
    void copy(vector<float> *v, vector<float> *w);           // copy v to w
    void find_max();
    float density(int x, int y);    
    void find_MERW();                                        // find MERW stochastic matrix
    void find_GRW();                                         // find GRW stochastic matrix
    void den_step(bool, bool);                               // evolve density using MERW or GRW stochastic matrix
    void eig_step(bool);                                     // step of power method searching for dominant eigenvectors
    void upd_eig();                                          // update eigenvectors - when gradient change, small defect change
    void find_rhoG();                                        // find GRW stationary probability
    void walker_step();                                      // perform random walk step for all walkers        
};

#endif	/* MERW_H */
