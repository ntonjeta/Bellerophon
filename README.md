BELLEROPHON A EVOLUTION SEARCH ENGINE FOR APPROXIMATE COMPUTING
==============================================================


Introduction
------------


Bellerophont is a genetic optimization tools thinking for approximate computing.


HOW TO USE
-------------

Requirement 
-------------

Bellerophon requires LLVM3.9.1 compiled with following flag: 

* -DLLVM_ENABLE_CXX1Y=true
* -DLLVM_ENABLE_RTTI=ON

Furthermore requires a Genetic Algorithm framework ParadisEO avaiable at this link:
 
* http://paradiseo.gforge.inria.fr/


build 
--------

    git clone https://github.com/ntonjeta/Bellerophon
    cd Bellerophon
    mkdir build
    cmake ../ 
    make

LICENSE 
--------

* [GPLV3.0] https://www.gnu.org/licenses/licenses.html

Contributing
----------

Github is for social coding: if you want to write code, I encourage contributions through pull requests from forks of this repository. 
