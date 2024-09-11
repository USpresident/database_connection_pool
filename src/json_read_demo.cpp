#include "json.h"
#include <fstream>
#include <iostream>


using namespace std;
using namespace Json;

void ReadJson()
{
    ifstream inFile("/code/database_connection_pool/data/json_data.txt");
    if (!inFile.is_open()) {
        return;
    }

    Reader rd;
    Value root;
    rd.parse(inFile, root);
    inFile.close();

    for (int i = 0; i < root.size(); ++i) {
        Value item = root[i];
        if (item.isInt()) {
            cout << item.asInt() << endl;
            continue;
        }

        if (item.isString()) {
            cout << item.asString() << endl;
            continue;
        }

        if (item.isBool()) {
            cout << item.asBool() << endl;
            continue;
        }

        if (item.isArray()) {
            for (int j = 0; j < item.size(); ++j) {
                // 通常已经知道类型
                cout << item[j].asString() << ", ";
            }
            cout << endl;
            continue;
        }

        if (item.isObject()) {
            Value::Members keys = item.getMemberNames(); // 去除json对象所有key
            for (int j = 0; j < keys.size(); ++j) {
                cout << keys[j] << ":" << item[keys[j]] << endl;
            }
            continue;
        }
    }
}

int main()
{
    ReadJson();
    return 0;
}

// build目录： g++ ../src/json_read_demo.cpp -g -o json -I../include_json -L../lib -ljsoncpp