#ifndef CMNDPARSER_HPP
#define CMNDPARSER_HPP

#include <vector>
#include<string>

namespace machy {
namespace util {

    class CmndParser {
        std::vector<std::string> cmnds;

        public:
            CmndParser() {}
            ~CmndParser() {}

            void Read(const int& argc , char* argv[]);

            const std::string& getCmndOption(const std::string& option);
            bool cmndOptionExists(const std::string& option);
    };

}
}

#endif /* cmndParser.hpp */