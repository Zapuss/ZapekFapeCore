/*
 * Copyright (C) 2011-2012 Project SkyFire <http://www.projectskyfire.org/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

/*
 * Scripts for spells with SPELLFAMILY_DRUID and SPELLFAMILY_GENERIC spells used by druid players.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_dru_".
 */

#include "ScriptPCH.h"
#include "SpellAuraEffects.h"

enum DruidSpells
{
    DRUID_INCREASED_MOONFIRE_DURATION   = 38414,
    DRUID_NATURES_SPLENDOR              = 57865,
	GLYPH_OF_FRENZIED_REGENERATION		= 54810,
};

// 54846 Glyph of Starfire
class spell_dru_glyph_of_starfire : public SpellScriptLoader
{
    public:
        spell_dru_glyph_of_starfire() : SpellScriptLoader("spell_dru_glyph_of_starfire") { }

        class spell_dru_glyph_of_starfire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_glyph_of_starfire_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(DRUID_INCREASED_MOONFIRE_DURATION))
                    return false;
                if (!sSpellMgr->GetSpellInfo(DRUID_NATURES_SPLENDOR))
                    return false;
                return true;
            }

            void HandleScriptEffect(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                if (Unit* unitTarget = GetHitUnit())
                    if (AuraEffect const* aurEff = unitTarget->GetAuraEffect(SPELL_AURA_PERIODIC_DAMAGE, SPELLFAMILY_DRUID, 0x00000002, 0, 0, caster->GetGUID()))
                    {
                        Aura* aura = aurEff->GetBase();

                        uint32 countMin = aura->GetMaxDuration();
                        uint32 countMax = aura->GetSpellInfo()->GetMaxDuration() + 9000;
                        if (caster->HasAura(DRUID_INCREASED_MOONFIRE_DURATION))
                            countMax += 3000;
                        if (caster->HasAura(DRUID_NATURES_SPLENDOR))
                            countMax += 3000;

                        if (countMin < countMax)
                        {
                            aura->SetDuration(uint32(aura->GetDuration() + 3000));
                            aura->SetMaxDuration(countMin + 3000);
                        }
                    }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_glyph_of_starfire_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_glyph_of_starfire_SpellScript();
        }
};

// 69366 - Moonkin Form passive
class spell_dru_moonkin_form_passive : public SpellScriptLoader
{
    public:
        spell_dru_moonkin_form_passive() : SpellScriptLoader("spell_dru_moonkin_form_passive") { }

        class spell_dru_moonkin_form_passive_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_moonkin_form_passive_AuraScript);

            uint32 absorbPct;

            bool Load()
            {
                absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                return true;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                // Set absorbtion amount to unlimited
                amount = -1;
            }

            void Absorb(AuraEffect* /*aurEff*/, DamageInfo & dmgInfo, uint32 & absorbAmount)
            {
                // reduces all damage taken while Stunned in Moonkin Form
                if (GetTarget()->GetUInt32Value(UNIT_FIELD_FLAGS) & (UNIT_FLAG_STUNNED) && GetTarget()->HasAuraWithMechanic(1<<MECHANIC_STUN))
                    absorbAmount = CalculatePctN(dmgInfo.GetDamage(), absorbPct);
            }

            void Register()
            {
                 DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_moonkin_form_passive_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                 OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_moonkin_form_passive_AuraScript::Absorb, EFFECT_0);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_moonkin_form_passive_AuraScript();
        }
};

// 62606 - Savage Defense
class spell_dru_savage_defense : public SpellScriptLoader
{
    public:
        spell_dru_savage_defense() : SpellScriptLoader("spell_dru_savage_defense") { }

        class spell_dru_savage_defense_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_savage_defense_AuraScript);

            uint32 absorbPct;

            bool Load()
            {
                absorbPct = GetSpellInfo()->Effects[EFFECT_0].CalcValue(GetCaster());
                return true;
            }

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                // Set absorbtion amount to unlimited
                amount = -1;
            }

            void Absorb(AuraEffect* aurEff, DamageInfo & /*dmgInfo*/, uint32 & absorbAmount)
            {
                absorbAmount = uint32(CalculatePctN(GetTarget()->GetTotalAttackPowerValue(BASE_ATTACK), absorbPct));
                aurEff->SetAmount(0);
            }

            void Register()
            {
                 DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_savage_defense_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                 OnEffectAbsorb += AuraEffectAbsorbFn(spell_dru_savage_defense_AuraScript::Absorb, EFFECT_0);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_savage_defense_AuraScript();
        }
};

class spell_dru_t10_restoration_4p_bonus : public SpellScriptLoader
{
    public:
        spell_dru_t10_restoration_4p_bonus() : SpellScriptLoader("spell_dru_t10_restoration_4p_bonus") { }

        class spell_dru_t10_restoration_4p_bonus_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_t10_restoration_4p_bonus_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_PLAYER;
            }

            void FilterTargets(std::list<Unit*>& unitList)
            {
                if (!GetCaster()->ToPlayer()->GetGroup())
                {
                    unitList.clear();
                    unitList.push_back(GetCaster());
                }
                else
                {
                    unitList.remove(GetTargetUnit());
                    std::list<Unit*> tempTargets;
                    for (std::list<Unit*>::const_iterator itr = unitList.begin(); itr != unitList.end(); ++itr)
                        if ((*itr)->GetTypeId() == TYPEID_PLAYER && GetCaster()->IsInRaidWith(*itr))
                            tempTargets.push_back(*itr);

                    if (tempTargets.empty())
                    {
                        unitList.clear();
                        FinishCast(SPELL_FAILED_DONT_REPORT);
                        return;
                    }

                    Unit* target = SelectRandomContainerElement(tempTargets);
                    unitList.clear();
                    unitList.push_back(target);
                }
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_dru_t10_restoration_4p_bonus_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_t10_restoration_4p_bonus_SpellScript();
        }
};

class spell_dru_starfall_aoe : public SpellScriptLoader
{
    public:
        spell_dru_starfall_aoe() : SpellScriptLoader("spell_dru_starfall_aoe") { }

        class spell_dru_starfall_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_starfall_aoe_SpellScript);

            void FilterTargets(std::list<Unit*>& unitList)
            {
                unitList.remove(GetTargetUnit());
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_dru_starfall_aoe_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_starfall_aoe_SpellScript();
        }
};

// 40121 - Swift Flight Form (Passive)
class spell_dru_swift_flight_passive : public SpellScriptLoader
{
    public:
        spell_dru_swift_flight_passive() : SpellScriptLoader("spell_dru_swift_flight_passive") { }

        class spell_dru_swift_flight_passive_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_dru_swift_flight_passive_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                Unit* caster = GetCaster();
                if (!caster || !caster->ToPlayer())
                    return;

                if (caster->ToPlayer()->Has310Flyer(false))
                    amount = 310;
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_swift_flight_passive_AuraScript::CalculateAmount, EFFECT_1, SPELL_AURA_MOD_INCREASE_VEHICLE_FLIGHT_SPEED);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_dru_swift_flight_passive_AuraScript();
        }
};

// Ferocious Bite
// Spell Id: 22568
class spell_dru_ferocious_bite : public SpellScriptLoader
{
    public:
        spell_dru_ferocious_bite() : SpellScriptLoader("spell_dru_ferocious_bite") { }

        class spell_dru_ferocious_bite_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_ferocious_bite_SpellScript);

            void CalculateDamage(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    int32 damage = GetHitDamage();
                    float ap = caster->GetTotalAttackPowerValue(BASE_ATTACK);
                    float multiple = ap / 410 + GetSpellInfo()->Effects[EFFECT_1].CalcValue();
                    int32 energy = -(caster->ModifyPower(POWER_ENERGY, -30));
                    damage += int32(energy * multiple);
                    damage += int32(CalculatePctN(caster->ToPlayer()->GetComboPoints() * ap, 7));
                    SetHitDamage(damage);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_ferocious_bite_SpellScript::CalculateDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_ferocious_bite_SpellScript;
        }
};

// Mark Of The Wild
// Spell Id: 1126
class spell_dru_mark_of_the_wild : public SpellScriptLoader
{
    public:
        spell_dru_mark_of_the_wild() : SpellScriptLoader("spell_dru_mark_of_the_wild") { }

        class spell_dru_mark_of_the_wild_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dru_mark_of_the_wild_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (caster->GetTypeId() != TYPEID_PLAYER)
                        return;

                    std::list<Unit*> PartyMembers;
                    caster->GetPartyMembers(PartyMembers);

                    if (PartyMembers.size() > 1)
                        caster->CastSpell(GetHitUnit(), 79061, true); // Mark of the Wild (Raid)
                    else
                        caster->CastSpell(GetHitUnit(), 79060, true); // Mark of the Wild (Caster)
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_dru_mark_of_the_wild_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dru_mark_of_the_wild_SpellScript;
        }
};

// Berserk
// Spellid: 50334
class spell_dru_berserk : public SpellScriptLoader
{
   public:
       spell_dru_berserk() : SpellScriptLoader("spell_dru_berserk") {}

       class spell_dru_berserk_AuraScript : public AuraScript
       {
           PrepareAuraScript(spell_dru_berserk_AuraScript);
           void HandleEffectApply(AuraEffect const * /*aurEff*/, AuraEffectHandleModes /*mode*/)
           {
               if (Unit* target = GetTarget())
                   if (target->GetTypeId() == TYPEID_PLAYER)
                       target->ToPlayer()->RemoveSpellCategoryCooldown(971, true);
           }

           void Register()
           {
               OnEffectApply += AuraEffectApplyFn(spell_dru_berserk_AuraScript::HandleEffectApply, EFFECT_2, SPELL_AURA_MECHANIC_IMMUNITY, AURA_EFFECT_HANDLE_REAL);
           }
       };

       AuraScript* GetAuraScript() const
       {
           return new spell_dru_berserk_AuraScript();
       }
};
class spell_dru_frenzied_regeneration : public SpellScriptLoader
{
	public:
	spell_dru_frenzied_regeneration() : SpellScriptLoader("spell_dru_frenzied_regeneration") { }

	class spell_dru_frenzied_regeneration_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_dru_frenzied_regeneration_AuraScript)
		
		bool Validate(SpellInfo const* spellEntry)
		{
			if (!sSpellMgr->GetSpellInfo(GLYPH_OF_FRENZIED_REGENERATION))
				return false;
			return true;
		}
		bool Load()
		{
			if (GetCaster()->GetTypeId() != TYPEID_PLAYER)
                return false;
            return true;
        }

//*****************Efekt 0  "converts up to 10 rage per second into health"************************
// Przeniesione z AuraEffect::HandlePeriodicDummyAuraTick zeby bylo wszystko w jednym miejscu
		void OnTick(AuraEffect const* aurEff)
		{
			 Unit* caster = GetCaster();
			 if (caster->getPowerType() != POWER_RAGE)
				return;
             uint32 rage = caster->GetPower(POWER_RAGE);
                    // Nothing todo
			 if (rage == 0)
				return;
			 int32 mod = (rage < 100) ? rage : 100;
             int32 points = caster->CalculateSpellDamage(caster, GetSpellInfo(), EFFECT_1); // Effekt_0 ma wg bazy value 15 0.o
             int32 regen = caster->GetMaxHealth() * (mod * points / 10) / 1000;
             caster->CastCustomSpell(caster, 22845, &regen, 0, 0, true, 0, aurEff);
             caster->SetPower(POWER_RAGE, rage-mod);
		}
//      Glyph of FR: "causes your Frenzied Regeneration to no longer convert rage into health"
		void CalcPeriodicEff0(AuraEffect const* aurEff, bool& isPeriodic, int32& amplitude)
		{
			if (GetCaster()->HasAura(GLYPH_OF_FRENZIED_REGENERATION))
				isPeriodic = false;
		}

//******************Effect 1 : Increases maximum health by 30% (not flat 30)******************			
		void CalcAmountEff1(AuraEffect const* aurEff, int32& amount, bool& canBeRecalculated)
		{
			amount = int32(GetCaster()->CountPctFromMaxHealth(amount));
		}

// 		      increases health to 30% (if below that value)
		void AfterEffect1Apply(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
		{
			Unit* caster = GetCaster();
			uint32 health_to_reach = caster->CountPctFromMaxHealth(30); // wartosc 30% max healtha
//AuraEffect::HandleAuraModIncreaseHealth zwieksza max health, a nastepnie aktualny health o ta sama wartosc
// w tym skillu aktualny health powinien byc zmieniony tylko i wylacznie jezeli jest ponizej 30%
			uint32 healththeory = caster->GetHealth() - aurEff->GetAmount();  // Wartosc heltha jaka powinna byc po odpaleniu skilla
			uint32 amount = healththeory < health_to_reach ? health_to_reach : healththeory;
			caster->SetHealth(amount);
		} 

//AuraEffect::HandleAuraModIncreaseHealth po zdjeciu buffa zmniejsza aktualny health o taka ilosc, 
//		o jaka zwiekszyl przy aplikowaniu aury, trzeba dodac to, co aura chce zabrac. 
		void OnEff1Remove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
		{
			GetCaster()->ModifyHealth(aurEff->GetAmount());
		}

//Efekt 2 nie wymaga wylaczenia przy braku glypha, bo i tak ma value=0, taka informacja jak by ktos sprawdzal czy dziala skrypt:P
		
		void Register()
		{
			DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_dru_frenzied_regeneration_AuraScript::CalcAmountEff1, EFFECT_1, SPELL_AURA_MOD_INCREASE_HEALTH_2);
			AfterEffectApply += AuraEffectApplyFn(spell_dru_frenzied_regeneration_AuraScript::AfterEffect1Apply, EFFECT_1, SPELL_AURA_MOD_INCREASE_HEALTH_2, AURA_EFFECT_HANDLE_REAL);
			OnEffectPeriodic += AuraEffectPeriodicFn(spell_dru_frenzied_regeneration_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
			DoEffectCalcPeriodic += AuraEffectCalcPeriodicFn(spell_dru_frenzied_regeneration_AuraScript::CalcPeriodicEff0, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
			OnEffectRemove += AuraEffectRemoveFn(spell_dru_frenzied_regeneration_AuraScript::OnEff1Remove, EFFECT_1, SPELL_AURA_MOD_INCREASE_HEALTH_2, AURA_EFFECT_HANDLE_REAL);
		}
	};

	AuraScript* GetAuraScript() const
	{
	return new spell_dru_frenzied_regeneration_AuraScript();
	}
};

void AddSC_druid_spell_scripts()
{
    new spell_dru_glyph_of_starfire();
    new spell_dru_moonkin_form_passive();
    new spell_dru_savage_defense();
    new spell_dru_t10_restoration_4p_bonus();
    new spell_dru_starfall_aoe();
    new spell_dru_swift_flight_passive();
    new spell_dru_ferocious_bite();
    new spell_dru_mark_of_the_wild();
    new spell_dru_berserk();
	new spell_dru_frenzied_regeneration();
}