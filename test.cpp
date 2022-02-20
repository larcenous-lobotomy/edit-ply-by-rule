#include "Task1.h"

int main() {
    Task1 test("test.ply");
    test.read();
    test.operate();
    test.write();
    return 0;
}