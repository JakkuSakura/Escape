//
// Created by jack on 20-2-29.
//

#ifndef ESCAPE_MESSAGE_H
#define ESCAPE_MESSAGE_H

template<typename T>
struct Message/* : public VirtualBase*/ {
    Message(entt::entity e_id, const T &t) : entity_id(e_id), data(t), processed(false) {}
    entt::entity entity_id;
    T data;
    bool processed;
};

struct Shooting {
    float angle;
};

struct ChangeWeapon {
    WeaponType weapon;
};
// TODO add message system. Hide Message as Message should only be used in network
#endif //ESCAPE_MESSAGE_H
