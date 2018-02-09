
#include <vector>
#include <iostream>

using std::vector;
int main() {
vector<int> vec{1,2,23,3,4};
    for (const auto &item : vec) {
        std::cout << item << std::endl;
    }

}