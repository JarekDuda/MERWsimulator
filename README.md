MERWsimulator
=============

Simulator for Maximal Entropy Random Walk on defected lattice (all but defected nodes have self-loop)

A standard way of choosing random walk on a graph (GRW) is: each outgoing edge from a given node is equally probable. It turns out that this choice of stochastic process usually does not maximize entropy production: sum_i Pr(node i) * sum_j Pr(i->j) log(Pr(i->j)). It is maximized in MERW, what usually leads to a different behavior, for example with much stronger localization properties (in analogy to quantum Anderson localization).

This simple simulator allows to compare behavior of GRW and MERW on defected lattice (a model of semiconductor): in which all but defected nodes have self-loop. Additionally it allows at set potential gradients in both directions to simulate conductance scenarios. 

Some sources: [article]( http://prl.aps.org/abstract/PRL/v102/i16/e160602), [PhD thesis]( http://www.fais.uj.edu.pl/documents/41628/d63bc0b7-cb71-4eba-8a5a-d974256fd065), [slides]( http://dl.dropbox.com/u/12405967/ANSang.pdf).

Created in Qt Creator 3.1.2. [Here is compiled for Windows]( https://dl.dropboxusercontent.com/u/12405967/MERW.zip) with all required files.

Jarek Duda, September 2014
