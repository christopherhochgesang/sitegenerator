#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
int readFile(std::string inputFilePath,std::string outputPath=".html");

int main(int argc,char* argv[]){
    //argv[1] = input file
    //argv[2] = optional insert template file
    if(argc ==3){
        //template
    }
    if(argc ==2){
        //output to inputfilename.html
        return readFile(std::string(argv[1]));
    }
    return 0;
}

int readFile(std::string inputFilePath,std::string outputPath){
    std::fstream inputFile;
    std::fstream outputFile;
    inputFile.open(inputFilePath);
    bool openPTag;

    if(outputPath==".html"){
        std::cout<< inputFilePath<<"\n";
        std::cout<<inputFilePath.substr(2,inputFilePath.find_last_of('.')-2)+".html\n";
        outputFile.open(inputFilePath.substr(2,inputFilePath.find_last_of('.')-2)+".html",std::fstream::out);
    } else {
        outputFile.open(outputPath,std::fstream::out);
    }
    char* line;
    if(outputFile.is_open() && inputFile.is_open()){
        while(!inputFile.eof()){
            inputFile.getline(line,10000);
            if(strlen(line)>0){
                if(openPTag){
                    outputFile.write("</p>\n",5);
                    outputFile.write("<p>",3);
                    outputFile.write(line,strlen(line));
                } else {
                    outputFile.write("<p>",3);
                    outputFile.write(line,strlen(line));
                    openPTag=true;
                }
            }
        }
    } else if(!outputFile.is_open()){
        std::cout<<"Output file didn't open!\n";
        return -1;
    }
    outputFile.write("</p>",4);
    outputFile.close();
    inputFile.close();
    return 0;
}


