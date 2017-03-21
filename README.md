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



#### Quick Start ####
--------

For use bellerophon you can build it from source, as follow:

    git clone https://github.com/ntonjeta/Bellerophon
    cd Bellerophon
    mkdir build
    cd build
    cmake ../ 
    make
    sudo make install

#### Docker Container - for compiling unenthusiastic ### 

Or you can use docker image for build a [container](https://github.com/ntonjeta/iidea-Docker)

#### How To Use ####

    USAGE: bellerophon [subcommand] [options] <tau>

OPTIONS:

Bellerophon:
Options for the Bellerophon tool

  - P=<path>                                 - Specify the path of the parent directory of a .param file
  - arcs=<archive object-path>               - Specify an archive object, or multiple comma separated, to load when linking the executable. It can be specified multiple times
  - l                                        - Show approximate technique list
  - libdb=<string>                           - Specify the patch in which of the parent directory of a library_database.csv file
  - objs=<path to object file ('.o' , '.so'> - Specify an object, or multiple comma separated, to load when linking the executable. It can be specified multiple times
  - p=<path>                                 - Specify the path of the parent directory of a compile_database.json file
  - r=<string>                               - Report produced by Approximation Operator
  - t=<string>                               - Select the approximate technique to apply
  - test                                     - Simple test
  - v                                        - Enable verbose outputs

Generic Options:

  - help                                     - Display available options (-help-hidden for more)
  - help-list                                - Display list of available options (-help-list-hidden for more)
  - version                                  - Display the version of this program


Bellerophon take some input for testing the approimate variants.
First, with -p flags it take a [compilation database](http://clang.llvm.org/docs/JSONCompilationDatabase.html),  were was defined the code in wich was applyied the approximation technique.
With -r flag pass a report in csv format that specified were to apply the approximation technique. 
This two input usually was geenerated from [chimera](https://github.com/ntonjeta/clang-chimera).

Furthermore, Bellerophon take other input with -t flag we can specified the technique used for apply the approximation. 
With -P flag can pass to tool a .param file that specified Genetic Algorithm parameters.
Finaly is mandatory to specified a <tau> value for error generated with approximation.   

#### Write Approximation Technique #### 

Bellerophon can be extend with new approximate technique. If you want to  

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
