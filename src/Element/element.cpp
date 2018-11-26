#include "include/Element/element.h"

Element::Element(QPair<uint16_t, uint16_t> position, uint16_t life)
    : m_position(position), m_angle(0), m_isDead(false), m_life(life) {

}

Element::~Element() {

}

uint16_t Element::getAngle() const {
    return m_angle;
}

QPair<uint16_t, uint16_t> Element::getPosition() const {
    return m_position;
}

void Element::setPosition(QPair<uint16_t, uint16_t> newPosition) {
    m_position = newPosition;
}

void Element::setAngle(uint16_t newAngle) {
    m_angle = newAngle;
}

void Element::takeDamage(uint8_t attackDamage) {
    if(attackDamage >= m_life){
        m_isDead = true;
        return;
    }
    if(m_life > 0){
        m_life -= attackDamage;
        if(m_life == 0){
            m_isDead = true;
        }
    }
}


bool Element::isDead() const {
    return m_isDead;
}

