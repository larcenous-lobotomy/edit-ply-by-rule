#ifndef TASK1_H
#define TASK1_H

#include "Base.h"
#include <iostream>
#include <fstream>
#include <vector>
#include<bits/stdc++.h>
#include <string>
#include<cstring>
using namespace std;

class Task1: public Base{  

    private:
        vector<string> input_read;
        vector<string> output_write;
        int vertices, faces;
        string filename;
        vector<string> split_str(string);
        string custom_trunc(string);
      
    protected:
    public:
        Task1(string);
        void read() override;
        void operate() override;
        void write() override;

};

#endif //APROJECT_TASK1_H