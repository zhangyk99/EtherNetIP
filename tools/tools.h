//
// Created by 79933 on 2024/11/26.
//

#ifndef SERVERDEVICE_ETHERNET_TOOLS_H
#define SERVERDEVICE_ETHERNET_TOOLS_H

#include <regex>
#include <string>

namespace EtherNetIP{
    bool isValidIPv4(const std::string& str) {
        std::regex ipv4Pattern("^((25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[0-1]?[0-9][0-9]?)$");
        return std::regex_match(str, ipv4Pattern);
    }

    int EuclideanAlgorithm(int a, int b){
        if(a < 1 || b < 1)
            return 0;
        if(a < b) {
            a = a + b;
            b = a - b;
            a = a - b;
        }
        int residue = 0;
        while(b){
            residue = a % b;
            a = b;
            b = residue;
        }
        return a;
    }
}

#endif //SERVERDEVICE_ETHERNET_TOOLS_H
