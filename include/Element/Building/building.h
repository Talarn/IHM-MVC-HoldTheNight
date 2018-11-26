#pragma once

#include "include/Element/element.h"
/**
 * @brief The Building class
 */
class Building : public Element {

public:
    Building(QPair<uint16_t, uint16_t> position, uint16_t life);
    uint16_t getLife();
};


