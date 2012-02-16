#include "StringUtils.h"

int StringUtils::splitString(const std::string& input, const std::string& delimiter, std::vector<std::string>& results)
{
  int iPos = 0;
  int newPos = -1;
  int sizeS2 = delimiter.size();
  int isize = input.size();

  std::vector<int> positions;

  newPos = input.find (delimiter, 0);

  if( newPos < 0 ) { return 0; }

  int numFound = 0;

  while( newPos > iPos )
  {
    numFound++;
    positions.push_back(newPos);
    iPos = newPos;
    newPos = input.find (delimiter, iPos+sizeS2+1);
  }

  for( int i=0; i <= positions.size(); i++ )
  {
    std::string s;
    if( i == 0 ) { s = input.substr( i, positions[i] ); }
    int offset = positions[i-1] + sizeS2;
    if( offset < isize )
    {
      if( i == positions.size() )
      {
        s = input.substr(offset);
      }
      else if( i > 0 )
      {
        s = input.substr( positions[i-1] + sizeS2, 
          positions[i] - positions[i-1] - sizeS2 );
      }
    }
    if( s.size() > 0 )
    
      results.push_back(s);
    
  }
  return numFound;
}

