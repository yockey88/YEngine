#include "Parsers/cmndParser.hpp"

namespace machy {
namespace util {

    void CmndParser::Read(const int& argc , char* argv[]) {
        for (int i = 0; i < argc; i++) 
            cmnds.push_back(std::string(argv[i]));
    }

    const std::string& CmndParser::getCmndOption(const std::string& option) {
        std::vector<std::string>::const_iterator itr;
        itr = std::find(cmnds.begin() , cmnds.end() , option);
        if (itr != cmnds.end() && ++itr != cmnds.end()) 
            return *itr;
        
        static const std::string empty_string("");
        
        return empty_string;
    }

    bool CmndParser::cmndOptionExists(const std::string& option) {
        return (std::find(cmnds.begin() , cmnds.end() , option) != cmnds.end());
    }

}
}