ios-push-service
================

This is a c++ wrap class for libcapn, you need compile this source code with libcapn.

How to compile
--------------

You need install compile and install libcapn to your computer first. And then you need to install boost-bjam cause the compile script is writting in boost-bjam style. (For more information about the boost compile system, you can check it from http://www.boost.org) After these two things complete, you will be able to download ios-push-service code and compile/install it.

Compile libcapn
---------------

Use git to clone libcapn repo from https://github.com/adobkin/libcapn and make sure you have initialized submodules, because it will need jansson source to compile. And then please use cmake to generate makefile and platform.h file from libapn root dir.

You can follow these commands to compile libcapn on your own system (make sure your system have make, cmake and git installed)

    git clone https://github.com/adobkin/libcapn
    cd libcapn
    git submodule update --init
    cmake .
    make && make install

Compile ios-push-service
------------------------

Install boost-bjam on your system. You can download last version of boost source package from http://www.boost.org and use boost-bjam to compile io-push-service

    bjam toolset=gcc
    
