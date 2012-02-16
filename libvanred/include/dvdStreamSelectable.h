#ifndef __DVD_STREAM_SELECTABLE_H_
#define __DVD_STREAM_SELECTABLE_H_

#include "dvdStream.h"

class dvdStreamSelectable : public dvdStream {
private:
      bool selected;
public:
       dvdStreamSelectable();
       void setStreamSelected(bool selected);
       bool streamIsSelected();     
};
#endif
