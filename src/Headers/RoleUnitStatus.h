#ifndef ROLEUNITSTATE_H
#define ROLEUNITSTATE_H

#include "Headers/DataPackage.h"

class RoleUnitData 
{
private:
    int m_dHealth;
    int m_dMaxHealth;
    int m_nAttackDamage;
    int m_nMoveRange;
    int m_nAttackRange;
    std::string m_Team;
    std::string m_sClassType;

public:
    RoleUnitData (characterStatus::dataPack d);
    std::string getTeam(){ return m_Team; }
    std::string getClass(){ return m_sClassType; }
    int getHP(){ return m_dHealth; }
    int getMaxHP(){ return m_dMaxHealth; }
    int getAttackDamage(){ return m_nAttackDamage; }

    void damaged(int damage);
    bool isDead(){ return m_dHealth <= 0 ? true : false; }
    int AttackRange(){ return m_nAttackRange; }
    int MoveRange(){ return m_nMoveRange; }
};

#endif