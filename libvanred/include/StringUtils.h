#ifndef __STRINGUTILS_H_
#define __STRINGUTILS_H_

#include <string>
#include <vector>

class StringUtils
{
public:

  static int splitString(const std::string& input, const std::string& delimiter, std::vector<std::string>& results);
};

#endif
