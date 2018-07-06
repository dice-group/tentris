# prerequisites

## install C++17 compiler


## install GTest for Testing
On Ubuntu run `sudo apt-get install libgtest-dev` to install the sources and `cd /usr/src/gtest && sudo cmake . && sudo make && sudo mv libg* /usr/lib/ && cd -` to compile it. Tested with Ubuntu 17.10. 

Further install the depencencies for antlr `sudo apt-get install uuid-dev`.

On Fedora run `sudo dnf install gtest gtest-devel`. Tested on Fedora 27.