/* 
 * File:   MERW.cpp
 * Author: Jarek
 * 
 * Created on September 8, 2014, 12:11 PM
 */

#include "MERW.h"
#include <cstdlib>
#include <vector>
#include <math.h>
#include <iostream>
#include <QDebug>


using namespace std;

MERW::MERW(int ww, int hh, float dp, float gx=0, float gy=0){
        w=ww; h=hh; wg.s=1; defp=dp; gradx=gx; grady=gy; 
        cych=true, cycv=true;
        MG=true; cont=true; numbwalk=100;
        accuracy = 1E-7; steps = 10;
        leig=new vector<float>(); leigp=new vector<float>(); 
        reig=new vector<float>(); reigp =new vector<float>();
        den=new vector<float>(); pden = new vector<float>();
        rhoM= new vector<float>(); rhoG= new vector<float>();
        init();               
    }
void MERW::update(){         // if small change: of gradients, added defect
    wg.u=exp(grady); wg.d=exp(-grady); wg.l=exp(gradx); wg.r=exp(-gradx); wg.s=1;
    if(MG){              // which mode
        upd_eig();
        find_MERW();
        if(cont) display=den; else {display=rhoM;}
    } else
    {   find_GRW();
        if(cont) display=den; else {find_rhoG(); display=rhoG;}
    }
    find_max();
}
void MERW::init(){            // if large change: of size, randomized defects
    wh=w*h; float t=1/(float)wh;
    leig->clear(); reig->clear(); den->clear(); rhoG->clear(); rhoM->clear(); walkers.clear();
    gen_def();
    for(int i=0; i<wh; i++) {reig->push_back(1); den->push_back(t); }
    walkers.clear(); update_walkers();
    update();
}
MERW::~MERW() {delete leig, leigp, reig, reigp, den, pden, rhoM, rhoG; }
float MERW::normalize(vector<float> *v)                       // normalize (L1 as for probability distribution)
    {float s=0,t; for(int i=0; i<wh; i++) s+=(*v)[i]; t=1/s; for(int i=0; i<wh; i++) (*v)[i]*=t; return s;}
void MERW::find_max(){maxv=0; for(int i=0; i<wh; i++)maxv = max(maxv,(*display)[i]);}
float MERW::dif(vector<float> *v, vector<float> *w)           // some (L1) distance for checking convergence
    {float d=0; for(int i=0; i<wh; i++) d+=fabs((*v)[i]-(*w)[i]); return d/(float)wh;} 
void MERW::copy(vector<float> *v, vector<float> *w)           // copy v to w
    {w->clear(); for(int i=0; i<wh; i++)w->push_back((*v)[i]);}
void MERW::printm(vector<float> *v)                           // print matrix
    {for(int y=0; y<wh; y+=w){for(int ps=y; ps<y+w; ps++) cout<<(*v)[ps]<<" "; cout<<endl;} cout<<endl;}
float MERW::density(int x, int y){return (*den)[x + w * y];}
void MERW::gen_def()                                          // generate random defects
    {self.clear();for(int i=0; i<w*h; i++) self.push_back(wg.s*((float)rand()/RAND_MAX > defp));}
void MERW::find_MERW(){               // find MERW stochastic matrix
    M.clear();
    edge t;
    int ps=0;
    for(int y=0; y<h; y++)
        for(int x=0; x<w; x++)
        {   float c = 1/ (lam * (*reig)[ps]);
            t.s = self[ps] /lam;
            t.l = (cych || x) ? (c * wg.l * (*reig)[x ? ps-1 : ps + w - 1]) : 0;
            t.r = (cych || (x-w+1)) ? (c * wg.r * (*reig)[x-w+1 ? ps+1 : ps - w + 1]) : 0;
            t.d = (cycv || y) ? (c * wg.d * (*reig)[y ? ps-w : ps + wh - w]) : 0;
            t.u = (cycv || (y-h+1)) ? (c * wg.u * (*reig)[y-h+1 ? ps+w : ps - wh + w]) : 0;
            M.push_back(t); ps++;
            //qDebug()<<"M:"<<x<<y<<ps<<t.s<<t.l<<t.r<<t.d<<t.u;
        }        
}
void MERW::find_GRW(){               // find GRW stochastic matrix
    G.clear();
    int ps=0;
    edge t;
    for(int y=0; y<h; y++)
        for(int x=0; x<w; x++)
        {   t.s = self[ps];
            t.l = (cych || x) ? wg.l : 0;
            t.r = (cych || (x-w+1)) ? wg.r : 0;
            t.d = (cycv || y) ? wg.d : 0;
            t.u = (cycv || (y-h+1)) ? wg.u : 0;
            float c = 1 / (t.s + t.d + t.u + t.l + t.r);
            t.s *= c; t.u *= c; t.d *= c; t.l *= c; t.r *= c; 
            G.push_back(t); ps++;
            //qDebug()<<"G:"<<x<<y<<ps<<t.s<<t.l<<t.r<<t.d<<t.u;
        }
}
void MERW::den_step(bool mrw, bool Gden=false){     // evolve density using MERW or GRW stochastic matrix
    vector<edge> *cs;             // current stochastic matrix
    vector <float> *prev, *cur;
    if(mrw) cs=&M; else cs=&G;
    if(Gden) {prev=rhoG; cur=pden; pden=prev; rhoG=cur;}
        else {prev=den; cur=pden; pden=prev; den=cur;};
    cur->clear();
    for(int ps=0; ps<wh; ps++) cur->push_back((*prev)[ps] * (*cs)[ps].s);
    int ps=0;
    for(int y=0; y<h; y++)
        for(int x=0; x<w; x++)
        {   (*cur)[x ? ps-1 : ps+ w-1] += (*prev)[ps] * (*cs)[ps].l;
            (*cur)[(x-w+1) ? ps+1 : ps-w+1] += (*prev)[ps] * (*cs)[ps].r;
            (*cur)[y ? ps-w : ps + wh-w] += (*prev)[ps] * (*cs)[ps].d;
            (*cur)[(y-h+1) ? ps+w : ps - wh+w] += (*prev)[ps] * (*cs)[ps].u;
            ps++;            
        }            
}
void MERW::eig_step(bool right=true){            // step of power method searching for dominant eigenvectors
    vector<float> *prev, *cur;                                  
    float  wu, wd, wl, wr;                                      // edge weights in 4 directions
    if(right)                                                   // left or right eigenvector
        {wl=wg.l; wr=wg.r; wu=wg.u; wd=wg.d; prev=reig; cur=reigp; reig=cur; reigp=prev;}
    else {wl=wg.r; wr=wg.l; wu=wg.d; wd=wg.u; prev=leig; cur=leigp; leig=cur; leigp=prev;};
    cur->clear();
    for(int i=0; i<wh; i++) cur->push_back((*prev)[i]*self[i]);             // initialization with self-loops
    for(int y=0; y<wh; y+=w){                                   // vertical edges
        if(cych)                                                // eventual cyclic
            {(*cur)[y+w-1] += wr * (*prev)[y]; (*cur)[y] += wl * (*prev)[y+w-1];};         
        for(int pos=y; pos<y+w-1; pos++)                        // main loop
            {(*cur)[pos] += wr * (*prev)[pos+1]; (*cur)[pos+1] += wl * (*prev)[pos];}          
    }
    for(int x=0; x<w; x++){                                     // horizontal edges
        if(cycv)                                                // eventual cyclic
            {(*cur)[wh-w+x] += wu * (*prev)[x]; (*cur)[x] += wd * (*prev)[wh-w+x]; }
        for(int pos=x+w; pos<wh; pos+=w){                           // main loop
            {(*cur)[pos-w] += wu * (*prev)[pos]; (*cur)[pos] += wd * (*prev)[pos-w];} 
        }
    } 
}
void MERW::upd_eig(){             // update eigenvectors - when gradient change, small defect change
    do {for(int i=1;i<steps;i++) eig_step();               // power method for right eigenvector
        normalize(reig); eig_step(); normalize(reig);               
    } while (dif(reig, reigp) >  accuracy);
    if(leig->empty()) copy(reig,leig);                // found eigenvector is a good approximation for left eigenvetor
    do {for(int i=1;i<steps;i++) eig_step(false);           // power method for left eigenvector
        normalize(leig); eig_step(false); lam=normalize(leig);    
    } while (dif(leig, leigp) >  accuracy);
    rhoM->clear();
    for(int i=0; i<wh; i++) rhoM->push_back((*leig)[i] * (*reig)[i]);        // rho is  
    normalize(rhoM);        
}
void MERW::find_rhoG(){
    if(rhoG->empty()) { for(int i=0; i<wh; i++) rhoG->push_back(1);}        
    do {for(int i=1;i<steps;i++) den_step(false, true);
        normalize(rhoG); den_step(false, true); normalize(rhoG);             
       } while (dif(rhoG, pden) >  accuracy); 
}
void MERW::walker_step(){             // perform random walk step for all walkers
    vector<edge> *cs;             // current stochastic matrix
    for(int i=0; i<walkers.size(); i++){
        coord c = walkers[i];
        edge e = MG ? M[w * c.y + c.x] : G[w * c.y + c.x];
        float r = (float)rand()/RAND_MAX;
        //qDebug()<<c.x<<c.y<<r<<e.l<<e.r<<e.d<<e.u<<e.s<<e.l+e.r+e.d+e.u+e.s;
        if((r -= e.l) < 0 ) {walkers[i].x = c.x ? c.x-1: w-1;}
            else if((r -= e.r) < 0 ) {walkers[i].x = (c.x+1)%w;}
                else if((r -= e.d) < 0 ) {walkers[i].y = c.y ? c.y-1 : h-1;}
                    else if((r -= e.u) < 0 ) {walkers[i].y = (c.y+1)%h;}   // no change in e.s case
    }
}
void MERW::update_walkers()                 // update number of walkers
 {int dif = numbwalk - walkers.size();
    if(dif>0) {for(int i=0; i<dif; i++)
        walkers.push_back({rand()%w, rand()%h, rand()%255, rand()%255, rand()%255});}
    else {for(int i=0; i>dif; i--) walkers.pop_back();}
 }
void MERW::make_step(){
    if(cont) {den_step(MG); display=den; find_max();}
    walker_step();
}
void MERW::point_density(int x,int y){            // change density to point
    for(int i=0; i<wh; i++) (*den)[i]=0; (*den)[w * y + x] = 1;
}
