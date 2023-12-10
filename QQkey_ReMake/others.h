#include <string>
#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <thread>
#include <fstream>  
std::string GenerateRandomString(int n)
{
    std::string szRand2;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 1);

    for (int j = 0; j < n; j++)
    {
        char randChar;
        switch (dis(gen))
        {
        case 1:
            randChar = static_cast<char>(rand() % 5 + 48);
            break;
        default:
            randChar = static_cast<char>(rand() % 5 + 53);
        }
        szRand2 += randChar;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return szRand2;
}
void findValueInJS(const char* jsCode, const char* key, char* value, int maxSize) {
    std::string code(jsCode);
    std::string searchKey(key);
    std::string result;

    std::size_t keyPos = code.find(searchKey);
    if (keyPos != std::string::npos) {
        std::size_t valueStartPos = code.find(':', keyPos);
        if (valueStartPos != std::string::npos) {
            valueStartPos = code.find_first_not_of(" \t\n\r\f\v", valueStartPos + 1);
            if (valueStartPos != std::string::npos && (code[valueStartPos] == '"' || code[valueStartPos] == '\'')) {
                std::size_t valueEndPos = code.find(code[valueStartPos], valueStartPos + 1);
                if (valueEndPos != std::string::npos) {
                    result = code.substr(valueStartPos + 1, valueEndPos - valueStartPos - 1);
                }
            }
            else {
                valueStartPos = code.find_first_not_of(" \t\n\r\f\v", valueStartPos);
                std::size_t valueEndPos = code.find_first_of(",};", valueStartPos);
                if (valueEndPos != std::string::npos) {
                    result = code.substr(valueStartPos, valueEndPos - valueStartPos);
                }
            }
        }
    }

    // 将结果复制到指定的 char 数组中
    strncpy_s(value, maxSize, result.c_str(), _TRUNCATE);
    value[maxSize - 1] = '\0';
}