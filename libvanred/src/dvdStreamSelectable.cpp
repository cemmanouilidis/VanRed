#include "dvdStreamSelectable.h"

dvdStreamSelectable::dvdStreamSelectable() {
     setStreamSelected(true);
}
void dvdStreamSelectable::setStreamSelected(bool selected) {
     this->selected = selected;
}
bool dvdStreamSelectable::streamIsSelected() {
     return selected;
}
