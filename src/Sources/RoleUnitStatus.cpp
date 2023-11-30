#include "Headers/RoleUnitStatus.h"

RoleUnitData::RoleUnitData(characterStatus::dataPack d) : 
m_dMaxHealth(d.MaxHealth), m_dHealth(d.MaxHealth), m_nAttackDamage(d.attackDamage), m_nAttackRange(d.attackRange),
m_nMoveRange(d.moveRange), m_Team(d.team), m_sClassType(d.classType)
{
}


void RoleUnitData::damaged(int damage){
    m_dHealth -= static_cast<double>(damage);
    printf("I got attack! HP left: %d", static_cast<int>(m_dHealth));
}

