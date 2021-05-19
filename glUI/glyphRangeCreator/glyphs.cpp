#include <iostream>
#include <stdio.h>
#include <codecvt>
#include <string>
#include <locale>
#include <assert.h>
#include <algorithm>
#include <vector>
#include <fstream>
#include <chrono>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
    if (!item.empty()) {
            elems.push_back(item);
        }
    }
    return elems;
}


int getCodepoints(){
    const char *outFileName = "output.txt";

    std::vector<std::string> glyphs;
    std::vector<std::string> glyphs_string;
    std::string line;

    std::vector<std::string> glyph_types;
    glyph_types.push_back( "[0-9]*	DIGIT (ZERO|ONE|TWO|THREE|FOUR|FIVE|SIX|SEVEN|EIGHT|NINE)\n");
    glyph_types.push_back("0020	SPACE");
    glyph_types.push_back("0021	EXCLAMATION MARK");
    glyph_types.push_back("0022	QUOTATION MARK");
    glyph_types.push_back("0023	NUMBER SIGN");
    glyph_types.push_back("0024	DOLLAR SIGN");
    glyph_types.push_back("0025	PERCENT SIGN");
    glyph_types.push_back("0026	AMPERSAND");
    glyph_types.push_back("0027	APOSTROPHE");
    glyph_types.push_back("0028	LEFT PARENTHESIS");
    glyph_types.push_back("0029	RIGHT PARENTHESIS");
    glyph_types.push_back("002A	ASTERISK");
    glyph_types.push_back("002B	PLUS SIGN");
    glyph_types.push_back("002C	COMMA");
    glyph_types.push_back("002D	HYPHEN-MINUS");
    glyph_types.push_back("002E	FULL STOP");
    glyph_types.push_back("002F	SOLIDUS");
    glyph_types.push_back("0030	DIGIT ZERO");
    glyph_types.push_back("0031	DIGIT ONE");
    glyph_types.push_back("0032	DIGIT TWO");
    glyph_types.push_back("0033	DIGIT THREE");
    glyph_types.push_back("0034	DIGIT FOUR");
    glyph_types.push_back("0035	DIGIT FIVE");
    glyph_types.push_back("0036	DIGIT SIX");
    glyph_types.push_back("0037	DIGIT SEVEN");
    glyph_types.push_back("0038	DIGIT EIGHT");
    glyph_types.push_back("0039	DIGIT NINE");
    glyph_types.push_back("003A	COLON");
    glyph_types.push_back("003B	SEMICOLON");
    glyph_types.push_back("003C	LESS-THAN SIGN");
    glyph_types.push_back("003D	EQUALS SIGN");
    glyph_types.push_back("003E	GREATER-THAN SIGN");
    glyph_types.push_back("003F	QUESTION MARK");
    glyph_types.push_back("0040	COMMERCIAL AT");
    glyph_types.push_back("[0-9]*	LATIN CAPITAL LETTER [A-Z]\n");
    glyph_types.push_back("[0-9]*	LATIN SMALL LETTER [A-Z]\n");


    glyph_types.push_back("[0-9]*	HIRAGANA LETTER");
    glyph_types.push_back("[0-9]*	KATAKANA LETTER");
    glyph_types.push_back("KANGXI RADICAL");
    glyph_types.push_back("CJK RADICAL");
    

    //https://www.unicode.org/Public/UCD/latest/ucd/NamesList.txt
    std::ifstream input_file("NameList2.txt");
    if (!input_file.is_open()) {
        std::cerr << "Could not open the input file" << std::endl;
        return -1;
    }


    std::ofstream ofs(outFileName);
    if (!ofs){
        std::cout << "Could not open the output file" << std::endl;
        return-1;
    }

    bool isContainGlyphType;
    unsigned int line_number = 0;
    unsigned int gCount = 1;
    char s[100];
    std::cmatch m__;

    while (getline(input_file, line)){
        // if( line[0] == '#' || line[0] == '	' || line[0] == '@' || 
        //     line.find('=') < 1glyph_types.push_back("000 || line.find("*") < 1glyph_types.push_back("000 ) continue;

        isContainGlyphType = false;
        // std::cout << ".";
        
        for(unsigned short i=0; i<glyph_types.size(); i++){
            isContainGlyphType |= std::regex_search(line  + "\n", std::regex(glyph_types[i]));
            if(isContainGlyphType)break;
        }


        // for(unsigned short i=0; i<glyph_types.size(); i++){
        //     isContainGlyphType |= (line.find(glyph_types[i]) != std::string::npos);
        //     if(isContainGlyphType)break;
        // }

        if(isContainGlyphType){
            auto AAAA = split(line, '	');
            glyphs_string.push_back(AAAA[0]);
            glyphs.push_back(AAAA[1]);

            std::cout << AAAA[0] << " = " << AAAA[1] << std::endl;
            ofs << "0x" << AAAA[0] << ",";
            if(gCount % 15 == 0){
                ofs << std::endl;
            }
            gCount++;
        }
        line_number++;
    }

    return 0;
}


int code_test(){
    std::string narrow_string("世界西宮直志食"); 
    std::u32string  u32str  = std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t>().from_bytes(narrow_string);
    for(int i=0; i<u32str.size(); i++){
        std::cout << std::hex <<  static_cast<int>(u32str[i]);
        std::cout << ", ";
    }
    return 0;
}


int main(){
    getCodepoints();
}