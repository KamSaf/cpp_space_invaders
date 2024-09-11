# Overview (WORK IN PROGRESS)

C++ simple space invaders game created with the usage of ncurses library.

# How to run (Linux):

1. Clone this repository:

        git clone https://github.com/KamSaf/cpp_space_invaders.git


2. Make sure you've got C++ compiler, like GCC, installed on your system.

3. Install requiered ```ncurses``` library, for example:

        sudo apt-get install libncurses5-dev libncursesw5-dev

4. When in project root directory compile the ```app.cpp``` file, if you use GCC run:

        g++ -o space_invaders.out app.cpp -lncurses 

5. When compiled, run the executable file:

        ./space_invaders.out

# Author

Created by Kamil Safaryjski 2024