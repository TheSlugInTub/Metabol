#include <iostream>
#include <vector>
#include <string>
#include <fstream>

enum Type 
{
    Type_Int8,
    Type_Int16,
    Type_Int32,
    Type_Int64,
    Type_UnsignedInt8,
    Type_UnsignedInt16,
    Type_UnsignedInt32,
    Type_UnsignedInt64,
    Type_Float32,
    Type_Float64,
};

struct Variable
{
    std::string identifier;
    Type type;
    
    union 
    {
        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;
        float f32;
        double f64;
    };
};

struct Structure
{
    std::string identifier;
    std::vector<Variable> variables;
};

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cout << "Please provide files as arguments.\n";
    }

    std::vector<Structure> structures;

    for (int i = 0; i < argc - 1; i++)
    {
        std::string buffer = "";
        std::ifstream file(argv[i + 1]);
        
        std::string line = "";
        while (std::getline(file, line))
        {
            buffer = buffer + line;
        }

        for (int j = 0; j < buffer.size(); j++)
        {
            
        }
    }

    return 0;
}
