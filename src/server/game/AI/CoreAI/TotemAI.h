/*
 * Copyright (C) 2011-2012 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2012 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TRINITY_TOTEMAI_H
#define TRINITY_TOTEMAI_H

#include "CreatureAI.h"
#include "Timer.h"

class Creature;
class Totem;

#define FLAME_SHOCK 8050
#define STORMSTRIKE 17364

class TotemAI : public CreatureAI
{
    public:

        explicit TotemAI(Creature* c);

        void MoveInLineOfSight(Unit*);
        void AttackStart(Unit*);
        void EnterEvadeMode();

        void UpdateAI(const uint32);
        static int Permissible(const Creature*);

    private:
        uint64 i_victimGuid;
};

class SentryTotemEnemyCheck
{
public:
    SentryTotemEnemyCheck(Unit* totem, float max_range, uint64 casterGUID = 0): _max_range(max_range), _totem(totem),
        _auraCheckerSpellOne(true, FLAME_SHOCK, casterGUID), _auraCheckerSpellTwo(true, STORMSTRIKE, casterGUID){}

    bool operator()(Unit* unit)
    {
        if (_totem->IsWithinDistInMap(unit, _max_range))
            return (_auraCheckerSpellOne(unit) || _auraCheckerSpellTwo(unit));
        return false;
    }

private:
   float _max_range;
   Trinity::UnitAuraCheck _auraCheckerSpellOne;
   Trinity::UnitAuraCheck _auraCheckerSpellTwo;
   Unit* _totem;
};

#endif
