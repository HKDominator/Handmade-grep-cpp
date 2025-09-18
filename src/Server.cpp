#include <iostream>
#include <string>
#include <cstring>
#include <vector>

enum Type{
    CHAR = 1000,
    GROUP = 1001,
    NGROUP = 1001,
    SPECIAL_D = 1002,
    SPECIAL_W = 1003
};

class Element{
private:
    int type;
    std::string value;
    std::string character_group;
    int non_character_group;
public:
    Element(int type, std::string ch_group, std::string character_group, int in_group):type(type), value(ch_group), character_group(character_group), non_character_group(in_group){}
    int getType()
    {
        return type;
    }
    std::string getValue()
    {
        return value;
    }
    std::string getGrouped(){
        return character_group;
    }
    int nonGrouped()
    {
        return non_character_group;
    }
    void setType(int t)
    {
        type = t;
    }
    void setValue( std::string c ){
        value.append(c);
    }
    void setCharGroup( std::string group )
    {
        character_group = group;
    }
    void setNonCharGroup( int nott )
    {
        non_character_group = nott;
    }

};


class Data{
    private:
        std::vector<Element> input;
        int at;
    public:
    Data(std::vector<Element> input, int at): input(input), at(at){}
    Data()
    {
        at = 0;
    }

    std::vector<Element> getInput()
    {
        return input;
    }

    int getAt()
    {
        return at;
    }

    Element getElementAt()
    {
        return input.at(at);
    }

    void inc()
    {
        at++;
    }
    void dec()
    {
        at--;
    }

    void reset()
    {
        at = 0;
    }

    bool validator()
    {
        return at < input.size();
    }

    void addElement(Element& a)
    {
        input.push_back(a);
    }

    int getSize()
    {
        return input.size();
    }
};

Data myData;

void addCharacter(int type, std::string character,  std::string group, int in_group )
{
    Element el{type, character, group, in_group};
    myData.addElement(el);
}

void populate_input( std::string input_line )
{
    int i = 0;
    while( i < input_line.length() )
    {
        if( input_line[i] == '[' )
        {
            int in_group = 1;
            int type = GROUP;
            i++;
            if( input_line[i] == '^' )
            {
                in_group = 0;
                i++;
                type = NGROUP;
            }
            std::string group;
            while( input_line[i] != ']' )
            {
                group.push_back(input_line[i]);
                i++;
            }
            addCharacter(type, "", group, in_group);
            i++;
            in_group = false;
        }
        else if( i < input_line.length() && input_line[i] == '\\' )
        {
            std::string escape;
            escape.push_back(input_line[i]);
            i++;
            escape.push_back(input_line[i]);
            int value;
            switch( input_line[i] )
            {
                case 'd':
                    value = SPECIAL_D;
                    break;
                case 'w':
                    value = SPECIAL_W;
                    break;
            }
            addCharacter(value, escape, "", 1);
            i++;
        }
        else if( i < input_line.length() )
        {
            addCharacter(CHAR, std::string(1,input_line[i]), "", 0);
            i++;
            //std::cerr << "da";
        }
    }
}

bool matchHere(const std::string& input_line, Data& myData, size_t pos)
{

    if( !myData.validator() )
        return true;
    if( myData.getSize() == 0 )
        return true;
    
    if( pos >= input_line.size() )
        return false;

    char c = input_line[pos];
    Element data = myData.getElementAt();
    bool is_match = false;

    if( isdigit(c) && data.getType() == SPECIAL_D )
        is_match = true;
    else if( (isalnum(c) || c == '_' ) && data.getType() == SPECIAL_W ) 
        is_match = true;
    else if( data.getType() == CHAR && data.getValue().size() == 1 && data.getValue()[0] == c )
        is_match = true;
    else if( data.getType() == GROUP && data.getGrouped().find(c) != std::string::npos )
        is_match = true;
    else if( data.getType() == NGROUP && data.getGrouped().find(c) == std::string::npos )
        is_match = true;
    if( pos < input_line.size() && is_match )
    {   
        myData.inc();
        return matchHere(input_line, myData, pos + 1);
    }
    return false;
}

bool match_pattern(const std::string& input_line, Data& myData, size_t pos = 0) {
    for( size_t i = pos; i <= input_line.size(); i ++ )
    {
        myData.reset();
        if( matchHere(input_line, myData, i) )
            return true;
    }
    return false;
    
    // if (pattern.length() == 1) {
    //     return input_line.find(pattern) != std::string::npos;
    // }
    // else if( pattern == "\\d") // have to escape the character
    // {
    //     return input_line.find_first_of("0123456789") != std::string::npos;
    // }
    // else if( pattern == "\\w" )
    // {
    //     for( const char& c : input_line )
    //     {
    //         if( isalnum(c) || c == '_' )
    //             return 1;
    //     }
    //     return 0;
    // }
    // else if( pattern[0] == '[' && pattern[pattern.length()-1] == ']') 
    // {
    //     if( pattern[1] != '^' )
    //     { 
    //         std::string substr = pattern.substr(1, pattern.length() - 2);
    //         return input_line.find_first_of(substr) != std::string::npos;
    //     }
    //     else{
    //         std::string substr = pattern.substr(2, pattern.length() - 2);
    //         return input_line.find_first_not_of(substr) != std::string::npos;
    //     }
    // }
    // else {
    //     throw std::runtime_error("Unhandled pattern " + pattern);
    // }
}

int main(int argc, char* argv[]) {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cerr << "Logs from your program will appear here" << std::endl;

    if (argc != 3) {
        std::cerr << "Expected two arguments" << std::endl;
        return 1;
    }

    std::string flag = argv[1];
    std::string pattern = argv[2];

    if (flag != "-E") {
        std::cerr << "Expected first argument to be '-E'" << std::endl;
        return 1;
    }

    // Uncomment this block to pass the first stage
    
    std::string input_line;
    std::getline(std::cin, input_line);
    

    populate_input(pattern);

    try {
        // std::vector<Element> data = myData.getInput();
        // for( auto el : data )
        // {
        //     std::cerr<< el.getType() << " " << el.getValue() << " " << el.getGrouped() << '\n';
        // }
        if (match_pattern(input_line, myData)) {
            std::cerr<< "da\n";
            return 0;
        } else {
            std::cerr<<"nu\n";
            return 1;
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
