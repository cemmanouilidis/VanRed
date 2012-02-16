#include <iostream>
#include <iomanip>

#ifndef DVD_H
#define DVD_H
#include "DVD.h"
#endif

#ifndef DVD_CONST_H
#include "DVDConst.h"
#define DVD_CONST_H
#endif

class ReAuthorXML {
public:
       static void writeXml(DVD& dvd, std::string source, std::string destination,std::ostringstream& xml, std::ostringstream& palette);
};
