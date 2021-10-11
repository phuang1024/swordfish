Building
========

Swordfish uses CMake to build. First, clone or download the repository, and follow
these steps:

.. code-block:: bash

    cd /path/to/swordfish
    mkdir ./build
    cd ./build
    cmake ..
    make

If you are building frequently, there is a Makefile at the root of the repository with
convenient targets:

.. code-block:: bash

    cd /path/to/swordfish
    make
