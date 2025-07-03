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
    std::vector<Structure> components;

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
            std::string compStr = buffer.substr(j, 14);

            if (compStr == "//// Component")
            {
                Structure comp;

                j += 14;
                char ch = buffer[j];

                while (ch != '{')
                {
                    j++;
                    ch = buffer[j];
                }

                j++; // Skip '{'
                ch = buffer[j];

                while (ch != '}')
                {
                    std::string typeStr;
                    std::string identifierStr;
                
                    while (ch == ' ')
                    {
                        j++;
                        ch = buffer[j];
                    }

                    if (ch == '}')
                    {
                        continue;
                    }

                    while (ch != ' ')
                    {
                        typeStr.append(ch, 1);
                        j++;
                        ch = buffer[j];
                    }

                    j++; // Skip space
            
                    while (ch != ';')
                    {
                        identifierStr.append(ch, 1);
                        j++;
                        ch = buffer[j];
                    }
                    
                    Variable var = {.identifier = identifierStr};

                    if (typeStr == "int")
                    {
                        var.type = Type_Int32;
                    }
                    else if (typeStr == "char")
                    {
                        var.type = Type_Int8;
                    }
                    else if (typeStr == "float")
                    {
                        var.type = Type_Float32;
                    }
                    else if (typeStr == "double")
                    {
                        var.type = Type_Float64;
                    }

                    comp.variables.push_back(var);
                }

                j++; // Skip '}'
            
                while (ch == ' ')
                {
                    j++;
                    ch = buffer[j];
                }

                std::string structIdentifier = "";

                while (ch != ';')
                {
                    structIdentifier.append(ch, 1);

                    j++;
                    ch = buffer[j];
                }   

                comp.identifier = structIdentifier;

                components.push_back(comp);
            }
        
            // j++;
        }
    }

    std::cout << "Components: " << components.size() << "\n\n";

    for (Structure& structure : components)
    {
        std::cout << "Name: " << structure.identifier << "\n\n";
        
        for (Variable& var : structure.variables)
        {
            std::cout << "Name: " << structure.identifier << '\n';
            std::cout << var.identifier << '\n';
            std::cout << var.type << "\n\n";
        }
    }

    return 0;
}
