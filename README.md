# BELLEROPHON #
## AN EVOLUTION SEARCH ENGINE FOR APPROXIMATE COMPUTING ##





### Introduction ###
------------


Bellerophont is a genetic optimization tools thinking for approximate computing.
It is think for use in collaboration with [clang-Chimera] (https://github.com/ntonjeta/clang-chimera). clang-Chimera provide to code mutation and Bellerophon use Genetic Algorithm for found the pareto-front of possible approximation. 

More details of Bellerophon implementation can found on my master's thesis 
[Link](null) 


### Install ###

#### Requirement ####
-------------

Bellerophon requires LLVM/Clang3.9.1 compiled with following flag: 

* -DLLVM_ENABLE_CXX1Y=true
* -DLLVM_ENABLE_RTTI=ON

NOTE: Bellerophon was develop and tested with x86 architecture, if you want run with other architecture you have to make some change:

After install LLVM/Clang3.9.1 on your architecture launch:

    llvm-config --componetns

Take output and in CMakeLists.txt at instruction "llvm_map_components_to_libnames", change the list of components after llvm_libs with list of components outputed from previous command.  

Furthermore requires Genetic Algorithm framework ParadisEO avaiable at this link:
 
* http://paradiseo.gforge.inria.fr/



#### Build ####
--------

    git clone https://github.com/ntonjeta/Bellerophon
    cd Bellerophon
    mkdir build
    cd build
    cmake ../ 
    make

#### Example ####
-------- 

You can test a simple exampe provide in repository, simply run the launch script.
The example use bit lenght reduction approximate tecnhnique provide by [FLAP library] (https://github.com/Ghost047/Fap) 

### LICENSE ###
--------

* [GPLV3.0](https://www.gnu.org/licenses/licenses.html)

### Contributing ###
----------

Github is for social coding: if you want to write code, I encourage contributions through pull requests from forks of this repository. 
