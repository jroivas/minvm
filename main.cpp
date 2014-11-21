#include <iostream>
#include <fstream>


int main(int argc, char **argv)
{
    if (argc <= 1) {
        std::cout << "Usage: " << argv[0] << " application\n";
        return 1;
    }

    std::ifstream input(argv[1]);
}
