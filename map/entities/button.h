#ifndef BUTTON_H
#define BUTTON_H

#include "entity.h"

void initButton(Button* button, Map *map);

void drawButtonAt(Button *button, QRect tileRect);


#endif // BUTTON_H
