#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <regex>

const int MAX_LINE_LENGTH = 1024;
const int MAX_TEMPLATES = 100;
const int MAX_INSTANTIATIONS = 100;

struct Template
{
    std::string              name;
    std::vector<std::string> parameterNames;
    std::string              body;
    int                      startLine;
    int                      endLine;
    bool isStruct; // true for struct, false for function
};

struct Instantiation
{
    std::string              templateName;
    std::vector<std::string> types;
    int                      lineNumber;
    bool isStruct; // true for struct, false for function
};

// Global arrays to store templates and instantiations
Template      templates[MAX_TEMPLATES];
Instantiation instantiations[MAX_INSTANTIATIONS];
int           templateCount = 0;
int           instantiationCount = 0;

std::string Trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::string ReplaceAll(const std::string& str,
                       const std::string& from, const std::string& to)
{
    std::string result = str;
    size_t      pos = 0;
    while ((pos = result.find(from, pos)) != std::string::npos)
    {
        result.replace(pos, from.length(), to);
        pos += to.length();
    }
    return result;
}

std::string ReplaceCharInString(const std::string& str, char unwanted,
                                char wanted)
{
    std::string newStr = str;
    std::replace(newStr.begin(), newStr.end(), unwanted, wanted);
    return newStr;
}

bool IsTemplateBegin(const std::string& line)
{
    return line.find("// TEMPLATE BEGIN") != std::string::npos;
}

bool IsTemplateEnd(const std::string& line)
{
    return line.find("// TEMPLATE END") != std::string::npos;
}

std::vector<std::string>
ExtractTemplateParameters(const std::string& line)
{
    std::vector<std::string> parameters;

    // Find the template declaration
    size_t templatePos = line.find("template");
    if (templatePos == std::string::npos)
        return parameters;

    // Extract everything after "template"
    std::string paramPart =
        line.substr(templatePos + 8); // Skip "template"
    std::istringstream iss(paramPart);
    std::string        param;

    // Split by comma and trim whitespace
    std::string token;
    while (std::getline(iss, token, ','))
    {
        token = Trim(token);
        if (!token.empty())
        {
            parameters.push_back(token);
        }
    }

    // If no comma found, treat as single parameter
    if (parameters.empty())
    {
        std::string singleParam = Trim(paramPart);
        if (!singleParam.empty())
        {
            parameters.push_back(singleParam);
        }
    }

    return parameters;
}

std::string ExtractFunctionName(const std::string& line)
{
    // Find function name in function definition
    std::regex  functionRegex(R"(\w+\s+(\w+)\s*\()");
    std::smatch match;

    if (std::regex_search(line, match, functionRegex))
    {
        return match[1].str();
    }

    return "";
}

std::string ExtractStructName(const std::string& line)
{
    // Find struct name in struct definition
    std::regex  structRegex(R"(struct\s+(\w+))");
    std::smatch match;

    if (std::regex_search(line, match, structRegex))
    {
        return match[1].str();
    }

    return "";
}

void ParseTemplates(const std::vector<std::string>& lines)
{
    templateCount = 0;
    bool                     inTemplate = false;
    int                      currentTemplateStart = -1;
    std::vector<std::string> currentParameterNames;
    std::string              currentTemplateName;
    std::string              currentBody;
    bool                     currentIsStruct = false;

    for (int i = 0; i < lines.size(); i++)
    {
        const std::string& line = lines[i];

        if (IsTemplateBegin(line))
        {
            inTemplate = true;
            currentTemplateStart = i;
            currentBody = "";
            currentParameterNames.clear();
            currentTemplateName = "";
            currentIsStruct = false;
            continue;
        }

        if (IsTemplateEnd(line))
        {
            if (inTemplate && templateCount < MAX_TEMPLATES)
            {
                templates[templateCount].parameterNames =
                    currentParameterNames;
                templates[templateCount].name = currentTemplateName;
                templates[templateCount].body = currentBody;
                templates[templateCount].startLine =
                    currentTemplateStart;
                templates[templateCount].endLine = i;
                templates[templateCount].isStruct = currentIsStruct;
                templateCount++;
            }
            inTemplate = false;
            continue;
        }

        if (inTemplate)
        {
            // Check if this line contains the template parameter
            // declaration
            if (line.find("template") != std::string::npos)
            {
                currentParameterNames =
                    ExtractTemplateParameters(line);
                continue;
            }

            // Check if this line contains a struct or function
            // definition
            if (currentTemplateName.empty())
            {
                std::string structName = ExtractStructName(line);
                if (!structName.empty())
                {
                    currentTemplateName = structName;
                    currentIsStruct = true;
                }
                else
                {
                    std::string funcName = ExtractFunctionName(line);
                    if (!funcName.empty())
                    {
                        currentTemplateName = funcName;
                        currentIsStruct = false;
                    }
                }
            }

            currentBody += line + "\n";
        }
    }
}

void ParseInstantiations(const std::vector<std::string>& lines)
{
    instantiationCount = 0;
    std::regex funcCallRegex(
        R"((\w+)<([^>]+)>\s*\()"); // Function calls
    std::regex structDeclRegex(
        R"((\w+)<([^>]+)>\s+\w+)"); // Struct declarations like
                                    // "MyStruct<int> var"
    std::regex structTypeRegex(
        R"((\w+)<([^>]+)>(?!\s*\())"); // Struct types in other
                                       // contexts

    for (int i = 0; i < lines.size(); i++)
    {
        const std::string& line = lines[i];
        std::smatch        match;

        // Check for function calls first
        if (std::regex_search(line, match, funcCallRegex))
        {
            if (instantiationCount < MAX_INSTANTIATIONS)
            {
                instantiations[instantiationCount].templateName =
                    match[1].str();
                instantiations[instantiationCount].isStruct = false;

                // Parse the types inside the angle brackets
                std::string              typeList = match[2].str();
                std::vector<std::string> types;
                std::istringstream       iss(typeList);
                std::string              type;

                while (std::getline(iss, type, ','))
                {
                    type = Trim(type);
                    if (!type.empty())
                    {
                        types.push_back(type);
                    }
                }

                instantiations[instantiationCount].types = types;
                instantiations[instantiationCount].lineNumber = i;
                instantiationCount++;
            }
        }
        // Check for struct declarations/usage
        else if (std::regex_search(line, match, structDeclRegex) ||
                 std::regex_search(line, match, structTypeRegex))
        {
            if (instantiationCount < MAX_INSTANTIATIONS)
            {
                instantiations[instantiationCount].templateName =
                    match[1].str();
                instantiations[instantiationCount].isStruct = true;

                // Parse the types inside the angle brackets
                std::string              typeList = match[2].str();
                std::vector<std::string> types;
                std::istringstream       iss(typeList);
                std::string              type;

                while (std::getline(iss, type, ','))
                {
                    type = Trim(type);
                    if (!type.empty())
                    {
                        types.push_back(type);
                    }
                }

                instantiations[instantiationCount].types = types;
                instantiations[instantiationCount].lineNumber = i;
                instantiationCount++;
            }
        }
    }
}

std::string
GenerateSpecializedFunction(const Template&                 tmpl,
                            const std::vector<std::string>& types)
{
    std::string specialized = tmpl.body;

    // Replace template parameters with actual types
    for (int i = 0;
         i < tmpl.parameterNames.size() && i < types.size(); i++)
    {
        specialized =
            ReplaceAll(specialized, tmpl.parameterNames[i], types[i]);
    }

    // Generate specialized function name
    std::string newFuncName = tmpl.name;
    for (const std::string& type : types)
    {
        std::string newStr = ReplaceCharInString(type, ' ', '_');
        newFuncName += "_" + newStr;
    }

    // Replace function name with specialized name
    specialized = ReplaceAll(specialized, tmpl.name, newFuncName);

    return specialized;
}

std::string ProcessLine(const std::string& line, int lineNumber)
{
    std::string result = line;

    // Replace template calls/declarations with regular ones
    for (int i = 0; i < instantiationCount; i++)
    {
        const Instantiation& inst = instantiations[i];
        if (inst.lineNumber == lineNumber)
        {
            // Build old pattern
            std::string oldPattern = inst.templateName + "<";
            for (int j = 0; j < inst.types.size(); j++)
            {
                if (j > 0)
                    oldPattern += ", ";
                oldPattern += inst.types[j];
            }
            oldPattern += ">";

            // Build new pattern
            std::string newPattern = inst.templateName;
            for (const std::string& type : inst.types)
            {
                newPattern += "_" + ReplaceCharInString(type, ' ', '_');
            }

            result = ReplaceAll(result, oldPattern, newPattern);
        }
    }

    return result;
}

void ProcessFile(const std::string& inputFile,
                 const std::string& outputFile)
{
    std::ifstream inFile(inputFile);
    if (!inFile.is_open())
    {
        std::cerr << "Error: Could not open input file " << inputFile
                  << std::endl;
        return;
    }

    // Read all lines
    std::vector<std::string> lines;
    std::string              line;
    while (std::getline(inFile, line)) { lines.push_back(line); }
    inFile.close();

    // Parse templates and instantiations
    ParseTemplates(lines);
    ParseInstantiations(lines);

    // Get unique type instantiations for each template
    std::map<std::string, std::set<std::vector<std::string>>>
        uniqueInstantiations;
    for (int i = 0; i < instantiationCount; i++)
    {
        uniqueInstantiations[instantiations[i].templateName].insert(
            instantiations[i].types);
    }

    // Write output file
    std::ofstream outFile(outputFile);
    if (!outFile.is_open())
    {
        std::cerr << "Error: Could not open output file "
                  << outputFile << std::endl;
        return;
    }

    for (int i = 0; i < lines.size(); i++)
    {
        bool skipLine = false;
        int  currentTemplateIndex = -1;

        // Check if this line is part of a template definition
        for (int j = 0; j < templateCount; j++)
        {
            if (i >= templates[j].startLine &&
                i <= templates[j].endLine)
            {
                // Comment out template definition
                outFile << "// " << lines[i] << std::endl;
                skipLine = true;
                currentTemplateIndex = j;
                break;
            }
        }

        // If we just finished a template definition, generate
        // specialized functions
        if (currentTemplateIndex >= 0 &&
            i == templates[currentTemplateIndex].endLine)
        {
            const Template& tmpl = templates[currentTemplateIndex];
            const std::set<std::vector<std::string>>& typeSets =
                uniqueInstantiations[tmpl.name];

            outFile << std::endl
                    << "// Generated specialized "
                    << (tmpl.isStruct ? "structs" : "functions")
                    << " for " << tmpl.name << std::endl;
            for (const std::vector<std::string>& types : typeSets)
            {
                outFile << GenerateSpecializedFunction(tmpl, types)
                        << std::endl;
            }
        }

        if (!skipLine)
        {
            // Process regular lines (replace template calls)
            std::string processedLine = ProcessLine(lines[i], i);
            outFile << processedLine << std::endl;
        }
    }

    outFile.close();

    std::cout << "Processing complete. Output written to "
              << outputFile << std::endl;
    std::cout << "Generated " << templateCount << " template(s) with "
              << instantiationCount << " instantiation(s)"
              << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cout << "Usage: " << argv[0]
                  << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    ProcessFile(inputFile, outputFile);

    return 0;
}
