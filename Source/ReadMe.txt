The code for my Honours research project.

Needs some setting up before you can compile:

1. Install Boost and Eigen, and add them to the include directories. I like to install them under c:\ProgramData.

2. Compile the Boost libraries (I use the Filesystem library). Make sure to use correct options for .\b2. E.g. compile debug versions as well.

3. Build the f2c library file for your platform: mvnpack.c contains a function converted from Fortran to calculate the CDF of the multivariate normal distribution.

4. Make sure this file is linked to. (In Visual Studio, add it in "Configuration Properties->Linker->Input->Additional Dependencies" under the project properties.)