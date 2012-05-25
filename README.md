Sol - Stochastic Learner
========================

Copyright (C) 2011-2012 Heidelberg University

Author: Sascha Fendrich  


Description
-----------

Sol implements several stochastic machine learning algorithms.

sol-bin   - binary classification  
sol-mucl  - multi-class classification  
sol-mulab - multi-label classification (small number of labels, only)  


Input Format
------------

*   Uses [svmlight's  sparse data format](http://svmlight.joachims.org)
    for input
*   For multi-class classification classes should be numbered from 0 to N-1
*   For multi-label classification the labels should be numbered from 
    0 to N-1 and correspond to the bits in the class number given in the
    training data. This will probably change in future versions.


Dependencies
------------

*   STL
*   Boost program options
*   tiny_log logging library (directly imported as a git submodule)


Compilation
-----------

*   Clone the github repository:

        git clone git@github.com:sfendrich/sol.git

*   Register and fetch submodules:

        git submodule init
        git submodule update

*   Compile logging library:

        cd tiny_log
        make
        cd ..

*   Compile sol:

        make

