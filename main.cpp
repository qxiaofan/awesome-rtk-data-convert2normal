
#include "converter.h"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>

const double DEG2RAD = 0.017453292519943; // = pi/180

int recordSplit(std::string p_OneRecord, long long &p_Timestamp, double &p_Lon, double &p_Lat, double &p_Height) {
    // std::cout << p_OneRecord << std::endl;
    char *pStr = const_cast<char *>(p_OneRecord.c_str());
    const char *pSplit = ",";
    char *pRet;
    int i = 0;
    bool bCheckHead = false;
    pRet = strtok(pStr, pSplit);
    while (pRet) {
        switch (i++) {
            case 0 : {
                p_Timestamp = atoll(pRet);
                break;
            }
            case 1 : {
                if (0 == strncmp(pRet, "$KSXT", 5)) {
                    bCheckHead = true;
                }
                break;
            }
            case 2 : {
                /// TODO: trans utc-time to clock
                break;
            }
            case 3 : {
                p_Lon = atof(pRet);
                if (p_Lon < 100) {
                    bCheckHead = false;
                }
                break;
            }
            case 4 : {
                p_Lat = atof(pRet);
                if (p_Lat < 20) {
                    bCheckHead = false;
                }
                break;
            }
            case 5 : {
                p_Height= atof(pRet);
                break;
            }
            default : {
                break;
            }
        }
        pRet = strtok(NULL, pSplit);
    }

    if ((14 >= i) || !bCheckHead) {
        return -1;
    }

    // std::cout << "i: " << i << std::endl;
    return 0;
}

int readFromFile(const std::string &p_File) {
    std::ifstream f1;
    f1.open(p_File, std::ios::in);
    if (!f1.is_open()) {
        std::cerr << "Can't open " << p_File << std::endl;
        return -1;
    }
    // std::cout << "Open " << p_File << " success." << std::endl;

    /// converter
    converter::LLAToENU transfer;

    std::string strLine;
    long long timestamp;
    double lon, lat, height;
    int line_num = 0;
    std::cout.precision(12);
    while (1) {
        if (f1.eof()) {
			std::cerr << "all line num: " << line_num << std::endl;
            break;
        }

        getline(f1, strLine);
        ++line_num;
        if (!strLine.empty()) {
            if (0 == recordSplit(strLine, timestamp, lon, lat, height)) {
                /// convert coordinate in LLA to ENU
                double xe, xn, xu;
                transfer.transLLAToENU(lon * DEG2RAD, lat * DEG2RAD, height, xe, xn, xu);
                std::cout << timestamp << " " << xe << " " << xn << " " << xu << std::endl;
            }
        }
    }

    return 0;
}

int main(int argc, char **argv) {

    if (argc < 2) {
        std::cerr << "usage: excute file-path" << std::endl;
        return -1;
    }

    std::ofstream cmdoutfile("./rtk_converted_data.txt");

    std::cout.rdbuf(cmdoutfile.rdbuf());

    std::string file(argv[1]);

    readFromFile(file);

    return 0;
}
