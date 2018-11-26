#include "include/Element/Building/building.h"

Building::Building(QPair<uint16_t, uint16_t> position, uint16_t life)
    : Element(position,life) {

}
uint16_t Building::getLife(){
    return m_life;
}
