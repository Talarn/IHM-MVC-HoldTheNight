#pragma once
#include <stdint.h>
#include <QPair>
#include <QStringList>

class Game;
/**
 * @brief The Element class
 * An Element has his own position, life, angle
 */
class Element {

public:
    Element(QPair<uint16_t, uint16_t> position, uint16_t life);
    ~Element();
    QPair<uint16_t, uint16_t> getPosition() const;
    uint16_t getAngle() const;
    void setAngle(uint16_t newAngle);
    void setPosition(QPair<uint16_t, uint16_t> newPosition);
    void takeDamage(uint8_t attackDamage);
    bool isDead() const;

protected:
    QPair<uint16_t, uint16_t> m_position;
    uint16_t m_angle;
    uint16_t m_life;
    bool m_isDead;
};

