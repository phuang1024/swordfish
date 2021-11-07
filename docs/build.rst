Building
========

Use the Makefile at the root of the repository to build. The Makefile downloads
dependencies and invokes CMake.

.. code-block:: bash

    make

The executable will be located at ``./build/swordfish``. You can also ``sudo make
install`` inside ``./build`` to install to ``/usr/local/bin``.
