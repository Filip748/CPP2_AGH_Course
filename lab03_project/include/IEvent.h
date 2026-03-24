#pragma once

enum class EventType {
    EnemyDamaged,
    EnemyDied,
    PlayerHealed,
    PlayerPickedItem
};

class IEvent {
public:
    virtual ~IEvent() = default;
    virtual EventType getType() const = 0;
};