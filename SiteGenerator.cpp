#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

int readFile(std::string inputFilePath,std::string templatePath = "NONE");
int writeFirstHalfTemplate(std::fstream& outputStream,std::fstream& templateStream);
int writeSecondHalfTemplate(std::fstream& outputStream,std::fstream& templateStream);

int main(int argc,char* argv[]){
    //argv[1] = input file
    //argv[2] = optional insert template file
    if(argc ==3){
        //template path provided
        return readFile(std::string(argv[1]),std::string(argv[2]));
    }
    if(argc ==2){
        //output to inputfilename.html
        return readFile(std::string(argv[1]));
    }
    return 0;
}

/// @brief Reads in the input file, converts it to html, and writes to inputFileName.html. 
/// @param inputFilePath the input file
/// @param templatePath path of the template file to use. If this isn't default, will paste the 
/// @return 0 if successful, 1,2,3 if input/output/template weren't able to be opened, 4 if something went wrong
int readFile(std::string inputFilePath,std::string templatePath){
    std::fstream inputFile;
    std::fstream outputFile;
    //may not be open
    std::fstream templateFile;

    inputFile.open(inputFilePath);
    outputFile.open(inputFilePath.substr(2,inputFilePath.find_last_of('.')-2)+".html",std::fstream::out);

    int spaceLen;
    std::string spaceString;

    bool openPTag;    
    std::string line;

    if(outputFile.is_open() && inputFile.is_open()){
        if(templatePath!="NONE"){
                templateFile.open(templatePath);
                spaceLen = writeFirstHalfTemplate(outputFile,templateFile);
                if(spaceLen>=0)spaceString = std::string(spaceLen,' ');
                else return -spaceLen;
        }
        while(!inputFile.eof()){
            try{
                std::getline(inputFile,line);
                if(!line.empty()){
                    if(line.at(0)=='#'){
                        //title line
                        if(openPTag){
                             outputFile<<"</p>\n";
                             openPTag==false;
                        }
                        outputFile<<spaceString<<"<h3>"<<line<<"</h3>\n";
                    } else {
                        //paragraph
                        if(openPTag){
                            outputFile<<"</p>\n"<<spaceString<<"<p>"<<line;
                        } else {
                            outputFile<<spaceString<<"<p>"<<line;
                            openPTag=true;
                        }
                    }
                }
            } catch (...){
                std::cerr <<"Something went wrong!\n";
            }
        }
        outputFile << "</p>\n";

        if(templatePath!="NONE"){
            writeSecondHalfTemplate(outputFile,templateFile);
        }
    } else if(!inputFile.is_open()){
        std::cerr<<"Input file didn't open!\n";
        return 1;
    } else if(!outputFile.is_open()){
        std::cerr<<"Output file didn't open!\n";
        return 2;
    } else {
        std::cerr<<"Something went wrong!";
        return 4;
    }
    outputFile.close();
    inputFile.close();
    return 0;
}

/// @brief Writes to the output stream the content before the <CONTENTBODY> tag in the template file
/// @param outputStream The file to write to
/// @param templateStream The file to read all before the <CONTENTBODY> tag - assumes your template file has a <CONTENTBODY> tag in it.
/// @return the amount of spaces before <CONTENTBODY> plus one if successful, -1 if streams weren't open
int writeFirstHalfTemplate(std::fstream& outputStream,std::fstream& templateStream){
    if(templateStream.is_open()&&outputStream.is_open()){
        size_t pos;
        std::string templateLine;
        while(!templateStream.eof()){
            std::getline(templateStream,templateLine);
            pos = templateLine.find("<CONTENTBODY");
            if(pos!= -1){
                return pos+1;
            } else {
                outputStream << templateLine <<'\n';
            }
        }
    } else {
        return -3;
    }
    return -4;
}

/// @brief Writes the content after the <CONTENTBODY> tag in the template file
/// @param outputStream The file to write to
/// @param templateStream The file to read all before the <CONTENTBODY> tag - assumes your template file has a <CONTENTBODY> tag in it.
/// @return 0 if successful, -1 if streams weren't open
int writeSecondHalfTemplate(std::fstream& outputStream,std::fstream& templateStream){
    if(templateStream.is_open() && outputStream.is_open()){
        std::string templateLine;
        while(!templateStream.eof()){
            std::getline(templateStream,templateLine);
            outputStream << templateLine <<'\n';
        }
    } else {
        return -1;
    }
    return 0;
}