#include<iostream>
#include<fstream>
#include<string>

using namespace std;

void WriteByteArrayLiteral(const char* file_name, ofstream& out_stream){
    ifstream in_stream(file_name, ios::binary);
    out_stream<<"{";
    char b;
    in_stream.get(b);
    out_stream<<dec<<"0x"<<hex<<(unsigned int)*reinterpret_cast<unsigned char*>(&b);
    while(in_stream.get(b)){
        out_stream<<", 0x"<<hex<<(unsigned int)*reinterpret_cast<unsigned char*>(&b);
    }
    in_stream.close();
    out_stream<<"};";
}

int main(int argc, const char** argv){
    string _name(argv[1]);
    string src_name = _name + ".cpp";
    string header_name = _name + ".hpp";
    ofstream source(src_name, std::ios_base::trunc);
    ofstream header(header_name, std::ios_base::trunc);
    
    source<<"#include \""<<header_name<<"\"\n";
    source<<"#include<Glewy/Content/data.hpp>\n";

    header<<"#ifndef ASSET_"<<argv[1]<<"\n";
    header<<"#define ASSET_"<<argv[1]<<"\n";

    header<<"namespace gly{class Data;}\n";
    header<<"class "<<argv[1]<<"{\npublic:\n";

    for(int i = 2; i < argc; i++){
        string name(argv[i]);
        name = name.substr(0, name.find_first_of('.'));

        source<<"unsigned char "<<name<<"_Bytes[] = ";WriteByteArrayLiteral(argv[i], source);source<<"\n";
        source<<"gly::Data "<<argv[1]<<"::"<<name<<" = {sizeof("<<name<<"_Bytes), "<<name<<"_Bytes};\n\n";

        header<<"static gly::Data "<<name<<";\n";
    }

    header<<"};\n";
    header<<"#endif";

    source.close();
    header.close();

    string cmd("g++ -c -O3 -I../GLEWY/include -o");
    system((cmd + _name + ".o " + src_name).c_str());
    remove(src_name.c_str());
}