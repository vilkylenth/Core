
#include "PlayerbotPriestAI.h"

class PlayerbotAI;

PlayerbotPriestAI::PlayerbotPriestAI(Player* const master, Player* const bot, PlayerbotAI* const ai): PlayerbotClassAI(master, bot, ai)
{
    RENEW              = ai->getSpellId("renew");
    if((HEAL = ai->getSpellId ("greater heal"))>0)
        HEAL           = ai->getSpellId("greater heal");
    else if((HEAL = ai->getSpellId ("heal"))>0 && (HEAL = ai->getSpellId ("greater heal"))==0)
        HEAL           = ai->getSpellId("heal");
    else if((HEAL = ai->getSpellId ("greater heal"))==0 && (HEAL = ai->getSpellId ("heal"))==0)
        HEAL           = ai->getSpellId("lesser heal");

    GREAT_HEAL         = ai->getSpellId("great heal");
    FLASH_HEAL         = ai->getSpellId("flash heal");
    REZZ               = ai->getSpellId("resurrection");
    SMITE              = ai->getSpellId("smite");
    MANA_BURN          = ai->getSpellId("mana burn");
    HOLY_NOVA          = ai->getSpellId("holy nova");
    HOLY_FIRE          = ai->getSpellId("holy fire");
    DESPERATE_PRAYER   = ai->getSpellId("desperate prayer");
    PRAYER_OF_HEALING  = ai->getSpellId("prayer of healing");
    CIRCLE_OF_HEALING  = ai->getSpellId("circle of healing");
    BINDING_HEAL       = ai->getSpellId("binding heal");
    PRAYER_OF_MENDING  = ai->getSpellId("prayer of mending");
    //SHADOWMAGIC
    FADE               = ai->getSpellId("fade");
    PAIN               = ai->getSpellId("shadow word: pain");
    MIND_BLAST         = ai->getSpellId("mind blast");
    SCREAM             = ai->getSpellId("psychic scream");
    MIND_FLAY          = ai->getSpellId("mind flay");
    DEVOURING_PLAGUE   = ai->getSpellId("devouring plague");
    SHADOW_PROTECTION  = ai->getSpellId("shadow protection");
    VAMPIRIC_TOUCH     = ai->getSpellId("vampiric touch");
    PRAYER_OF_SHADOW_PROTECTION = ai->getSpellId("prayer of shadow protection");
    SHADOWFIEND        = ai->getSpellId("shadowfiend");
    MIND_SEAR          = ai->getSpellId("mind sear");
    //DISCIPLINE
    PENANCE         = ai->getSpellId("penance");
    INNER_FIRE         = ai->getSpellId("inner fire");
    PWS                = ai->getSpellId("power word: shield");
    if((FORTITUDE = ai->getSpellId ("prayer of fortitude"))>0)
        FORTITUDE      = ai->getSpellId("prayer of fortitude");
    if((FORTITUDE = ai->getSpellId ("power word: fortitude"))>0 && (FORTITUDE = ai->getSpellId ("prayer of fortitude"))==0)
        FORTITUDE      = ai->getSpellId("power word: fortitude");

    FEAR_WARD          = ai->getSpellId("fear ward");
    if((DSPIRIT = ai->getSpellId ("prayer of spirit"))>0)
        DSPIRIT        = ai->getSpellId("prayer of spirit");
    if((DSPIRIT = ai->getSpellId ("divine spirit"))>0 && (DSPIRIT = ai->getSpellId ("prayer of spirit"))==0)
        DSPIRIT        = ai->getSpellId("divine spirit");

    MASS_DISPEL        = ai->getSpellId("mass dispel");
    POWER_INFUSION     = ai->getSpellId("power infusion");
    DIVINE_SPIRIT      = ai->getSpellId("divine spirit");
    INNER_FOCUS        = ai->getSpellId("inner focus");

    RECENTLY_BANDAGED  = 11196; // first aid check

    // racial
    ARCANE_TORRENT          = ai->getSpellId("arcane torrent"); // blood elf
    GIFT_OF_THE_NAARU       = ai->getSpellId("gift of the naaru"); // draenei
    STONEFORM               = ai->getSpellId("stoneform"); // dwarf
    EVERY_MAN_FOR_HIMSELF   = ai->getSpellId("every man for himself"); // human
    SHADOWMELD              = ai->getSpellId("shadowmeld"); // night elf
    BERSERKING              = ai->getSpellId("berserking"); // troll
    WILL_OF_THE_FORSAKEN    = ai->getSpellId("will of the forsaken"); // undead
}

PlayerbotPriestAI::~PlayerbotPriestAI() {}

void PlayerbotPriestAI::HealTarget(Unit &target, uint8 hp)
{
    PlayerbotAI* ai = GetAI();

//    return ((hp < 80 && !target.HasAura(RENEW, EFFECT_INDEX_0) &&  ai->CastSpell(RENEW, target)) ||
//        (hp < 60 && ai->CastSpell(HEAL, target)) ||
//        (hp < 30 && ai->CastSpell(FLASH_HEAL, target)) );

    if (hp < 25 && FLASH_HEAL > 0 && ai->GetManaPercent() >= 20)
    {
        ai->TellMaster("I'm casting flash heal.");
        ai->CastSpell(FLASH_HEAL, target);
    }
    else if (hp < 30 && GREAT_HEAL > 0 && ai->GetManaPercent() >= 36)
    {
        ai->TellMaster("I'm casting one of the sorted heal spells.");
        ai->CastSpell(GREAT_HEAL, target);
    }
    else if (hp < 33 && BINDING_HEAL > 0 && ai->GetManaPercent() >= 27)
    {
        ai->TellMaster("I'm casting binding heal.");
        ai->CastSpell(BINDING_HEAL, target);
    }
    else if (hp < 40 && PRAYER_OF_HEALING > 0 && ai->GetManaPercent() >= 54)
    {
        ai->TellMaster("I'm casting prayer of healing.");
        ai->CastSpell(PRAYER_OF_HEALING, target);
    }
    else if (hp < 50 && CIRCLE_OF_HEALING > 0 && ai->GetManaPercent() >= 24)
    {
        ai->TellMaster("I'm casting circle of healing.");
        ai->CastSpell(CIRCLE_OF_HEALING, target);
    }
    else if (hp < 60 && HEAL > 0 && ai->GetManaPercent() >= 36)
    {
        ai->TellMaster("I'm casting one of the sorted heal spells.");
        ai->CastSpell(HEAL, target);
    }
    else if (hp < 80 && RENEW > 0 && ai->GetManaPercent() >= 19)
    {
        //ai->TellMaster("I'm casting renew.");
        ai->CastSpell(RENEW, target);
    }

} // end HealTarget

void PlayerbotPriestAI::DoNextCombatManeuver(Unit *pTarget)
{
    Unit* pVictim = pTarget->getVictim();
    PlayerbotAI* ai = GetAI();
    if (!ai)
        return;

    switch (ai->GetScenarioType())
    {
        case PlayerbotAI::SCENARIO_DUEL:
            (ai->HasAura(SCREAM,*pTarget) && ai->GetHealthPercent() < 60 && ai->CastSpell(HEAL)) ||
                ai->CastSpell(PAIN) ||
                (ai->GetHealthPercent() < 80 && ai->CastSpell(RENEW)) ||
                (ai->GetPlayerBot()->GetDistance(pTarget) <= 5 && ai->CastSpell(SCREAM)) ||
                ai->CastSpell(MIND_BLAST) ||
                (ai->GetHealthPercent() < 20 && ai->CastSpell(GREAT_HEAL)) ||
                ai->CastSpell(SMITE);
            return;
    }

    // ------- Non Duel combat ----------

    ai->SetMovementOrder( PlayerbotAI::MOVEMENT_FOLLOW, GetMaster() ); // dont want to melee mob

    Player *m_bot = GetPlayerBot();
    Group *m_group = m_bot->GetGroup();

    // Heal myself
    if (ai->GetHealthPercent() < 15 && FADE > 0 && !m_bot->HasAura(FADE, EFFECT_INDEX_0))
    {
        ai->TellMaster("I'm casting fade.");
        ai->CastSpell(FADE, *m_bot);
    }
    else if (ai->GetHealthPercent() < 25 && PWS > 0 && !m_bot->HasAura(PWS, EFFECT_INDEX_0))
    {
        ai->TellMaster("I'm casting pws on myself.");
        ai->CastSpell(PWS);
    }
    else if (ai->GetHealthPercent() < 35 && DESPERATE_PRAYER > 0)
    {
        ai->TellMaster("I'm casting desperate prayer.");
        ai->CastSpell(DESPERATE_PRAYER, *m_bot);
    }
    else if (ai->GetHealthPercent() < 80)
        HealTarget (*m_bot, ai->GetHealthPercent());

    // Heal master
    uint32 masterHP = GetMaster()->GetHealth()*100 / GetMaster()->GetMaxHealth();
    if (GetMaster()->isAlive())
    {
        if (masterHP < 25 && PWS > 0 && !GetMaster()->HasAura(PWS, EFFECT_INDEX_0))
                ai->CastSpell(PWS, *(GetMaster()));
        else if (masterHP < 80)
            HealTarget (*GetMaster(), masterHP);
    }

    // Heal group
    if( m_group )
    {
        Group::MemberSlotList const& groupSlot = m_group->GetMemberSlots();
        for (Group::member_citerator itr = groupSlot.begin(); itr != groupSlot.end(); itr++)
        {
            Player *m_groupMember = sObjectMgr.GetPlayer( itr->guid );
            if( !m_groupMember || !m_groupMember->isAlive() )
                continue;

            uint32 memberHP = m_groupMember->GetHealth()*100 / m_groupMember->GetMaxHealth();
            if( memberHP < 25 )
                HealTarget( *m_groupMember, memberHP );
        }
    }

    // Damage Spells
    ai->SetInFront( pTarget );
	float dist = m_bot->GetDistance( pTarget );

    switch (SpellSequence)
    {
        case SPELL_HOLY:
            if (SMITE > 0 && LastSpellHoly <1 && !pTarget->HasAura(SMITE, EFFECT_INDEX_0) && ai->GetManaPercent() >= 17)
            {
                ai->CastSpell(SMITE, *pTarget);
                SpellSequence = SPELL_SHADOWMAGIC;
                LastSpellHoly = LastSpellHoly +1;
                break;
            }
            else if (MANA_BURN > 0 && LastSpellHoly <2 && pTarget->GetPower(POWER_MANA) > 0 && ai->GetManaPercent() < 70 && ai->GetManaPercent() >= 14)
            {
                //ai->TellMaster("I'm casting mana burn.");
                ai->CastSpell(MANA_BURN, *pTarget);
                ai->SetIgnoreUpdateTime(3);
                SpellSequence = SPELL_SHADOWMAGIC;
                LastSpellHoly = LastSpellHoly +1;
                break;
            }
            else if (HOLY_NOVA > 0 && LastSpellHoly <3 && dist <= ATTACK_DISTANCE && ai->GetManaPercent() >= 22)
            {
                //ai->TellMaster("I'm casting holy nova.");
                ai->CastSpell(HOLY_NOVA);
                SpellSequence = SPELL_SHADOWMAGIC;
                LastSpellHoly = LastSpellHoly +1;
                break;
             }
             else if (HOLY_FIRE > 0 && LastSpellHoly <4 && !pTarget->HasAura(HOLY_FIRE, EFFECT_INDEX_0) && ai->GetManaPercent() >= 13)
             {
                 //ai->TellMaster("I'm casting holy fire.");
                 ai->CastSpell(HOLY_FIRE, *pTarget);
                 SpellSequence = SPELL_SHADOWMAGIC;
                 LastSpellHoly = LastSpellHoly +1;
                 break;
             }
             else if (PRAYER_OF_MENDING > 0 && LastSpellHoly <5 && pVictim == GetMaster() && GetMaster()->GetHealth() <= GetMaster()->GetMaxHealth()*0.7 && !GetMaster()->HasAura(PRAYER_OF_MENDING, EFFECT_INDEX_0) && ai->GetManaPercent() >= 15)
             {
                 //ai->TellMaster("I'm casting prayer of mending on master.");
                 ai->CastSpell(PRAYER_OF_MENDING, *GetMaster());
                 SpellSequence = SPELL_SHADOWMAGIC;
                 LastSpellHoly = LastSpellHoly +1;
                 break;
             }
             else if (LastSpellHoly > 6)
             {
                 LastSpellHoly = 0;
                 SpellSequence = SPELL_SHADOWMAGIC;
                 break;
             }
             LastSpellHoly = LastSpellHoly + 1;
             //SpellSequence = SPELL_SHADOWMAGIC;
             //break;

        case SPELL_SHADOWMAGIC:
            if (PAIN > 0 && LastSpellShadowMagic <1 && !pTarget->HasAura(PAIN, EFFECT_INDEX_0) && ai->GetManaPercent() >= 25)
            {
                //ai->TellMaster("I'm casting pain.");
                ai->CastSpell(PAIN, *pTarget);
                SpellSequence = SPELL_DISCIPLINE;
                LastSpellShadowMagic = LastSpellShadowMagic +1;
                break;
            }
            else if (MIND_BLAST > 0 && LastSpellShadowMagic <2 && ai->GetManaPercent() >= 19)
            {
                //ai->TellMaster("I'm casting mind blast.");
                ai->CastSpell(MIND_BLAST, *pTarget);
                SpellSequence = SPELL_DISCIPLINE;
                LastSpellShadowMagic = LastSpellShadowMagic +1;
                break;
            }
           else if (SCREAM > 0 && LastSpellShadowMagic <3 && ai->GetAttackerCount()>=3 && ai->GetManaPercent() >= 15)
            {
                ai->TellMaster("I'm casting scream.");
                ai->CastSpell(SCREAM);
                SpellSequence = SPELL_DISCIPLINE;
                (LastSpellShadowMagic = LastSpellShadowMagic +1);
                break;
            }

            else if (MIND_FLAY > 0 && LastSpellShadowMagic <4 && !pTarget->HasAura(MIND_FLAY, EFFECT_INDEX_0) && ai->GetManaPercent() >= 10)
            {
                //ai->TellMaster("I'm casting mind flay.");
                ai->CastSpell(MIND_FLAY, *pTarget);
                ai->SetIgnoreUpdateTime(3);
                SpellSequence = SPELL_DISCIPLINE;
                LastSpellShadowMagic = LastSpellShadowMagic +1;
                break;
            }
            else if (DEVOURING_PLAGUE > 0 && LastSpellShadowMagic <5 && !pTarget->HasAura(DEVOURING_PLAGUE, EFFECT_INDEX_0) && ai->GetManaPercent() >= 28)
            {
                ai->CastSpell(DEVOURING_PLAGUE, *pTarget);
                SpellSequence = SPELL_DISCIPLINE;
                LastSpellShadowMagic = LastSpellShadowMagic +1;
                break;
            }
            else if (SHADOW_PROTECTION > 0 && LastSpellShadowMagic <6 && ai->GetManaPercent() >= 60)
            {
                ai->CastSpell(SHADOW_PROTECTION, *pTarget);
                SpellSequence = SPELL_DISCIPLINE;
                LastSpellShadowMagic = LastSpellShadowMagic +1;
                break;
            }
            else if (VAMPIRIC_TOUCH > 0 && LastSpellShadowMagic <7 && !pTarget->HasAura(VAMPIRIC_TOUCH, EFFECT_INDEX_0) && ai->GetManaPercent() >= 18)
            {
                ai->CastSpell(VAMPIRIC_TOUCH, *pTarget);
                SpellSequence = SPELL_DISCIPLINE;
                LastSpellShadowMagic = LastSpellShadowMagic +1;
                break;
            }
            else if (SHADOWFIEND > 0 && LastSpellShadowMagic <8)
            {
                ai->CastSpell(SHADOWFIEND);
                SpellSequence = SPELL_DISCIPLINE;
                LastSpellShadowMagic = LastSpellShadowMagic +1;
                break;
            }
            else if (MIND_SEAR > 0 && LastSpellShadowMagic <9 && ai->GetAttackerCount()>=3 && ai->GetManaPercent() >= 28)
            {
                ai->CastSpell(MIND_SEAR, *pTarget);
                ai->SetIgnoreUpdateTime(5);
                SpellSequence = SPELL_DISCIPLINE;
                LastSpellShadowMagic = LastSpellShadowMagic +1;
                break;
            }
            else if (LastSpellShadowMagic > 10)
            {
                LastSpellShadowMagic = 0;
                SpellSequence = SPELL_DISCIPLINE;
                break;
            }
            LastSpellShadowMagic = LastSpellShadowMagic +1;
            //SpellSequence = SPELL_DISCIPLINE;
            //break;

        case SPELL_DISCIPLINE:
            if (FEAR_WARD > 0 && LastSpellDiscipline <1 && ai->GetManaPercent() >= 3)
            {
                //ai->TellMaster("I'm casting fear ward");
                ai->CastSpell(FEAR_WARD, *(GetMaster()));
                SpellSequence = SPELL_HOLY;
                LastSpellDiscipline = LastSpellDiscipline + 1;
                break;
            }
            else if (POWER_INFUSION > 0 && LastSpellDiscipline <2 && ai->GetManaPercent() >= 16)
            {
                //ai->TellMaster("I'm casting power infusion");
                ai->CastSpell(POWER_INFUSION, *(GetMaster()));
                SpellSequence = SPELL_HOLY;
                LastSpellDiscipline = LastSpellDiscipline + 1;
                break;
            }
            else if (MASS_DISPEL > 0 && LastSpellDiscipline <3 && ai->GetManaPercent() >= 33)
            {
                //ai->TellMaster("I'm casting mass dispel");
                ai->CastSpell(MASS_DISPEL);
                SpellSequence = SPELL_HOLY;
                LastSpellDiscipline = LastSpellDiscipline + 1;
                break;
            }
            else if (INNER_FOCUS > 0 && !m_bot->HasAura(INNER_FOCUS, EFFECT_INDEX_0) && LastSpellDiscipline <4)
            {
                //ai->TellMaster("I'm casting inner focus");
                ai->CastSpell(INNER_FOCUS, *m_bot);
                SpellSequence = SPELL_HOLY;
                LastSpellDiscipline = LastSpellDiscipline + 1;
                break;
            }
            else if (PENANCE > 0 && LastSpellDiscipline <5 && ai->GetManaPercent() >= 16)
            {
                //ai->TellMaster("I'm casting PENANCE");
                ai->CastSpell(PENANCE);
                SpellSequence = SPELL_HOLY;
                LastSpellDiscipline = LastSpellDiscipline + 1;
                break;
            }
            else if (LastSpellDiscipline > 6)
            {
                LastSpellDiscipline = 0;
                SpellSequence = SPELL_HOLY;
                break;
            }
            else
            {
                LastSpellDiscipline = LastSpellDiscipline + 1;
                SpellSequence = SPELL_HOLY;
            }
    }
} // end DoNextCombatManeuver

void PlayerbotPriestAI::DoNonCombatActions()
{
    PlayerbotAI* ai = GetAI();
    Player * m_bot = GetPlayerBot();
    if (!m_bot)
        return;

    SpellSequence = SPELL_HOLY;

    // buff myself
    if (FORTITUDE > 0)
        (!m_bot->HasAura(FORTITUDE, EFFECT_INDEX_0) && ai->CastSpell (FORTITUDE, *m_bot));

    if (INNER_FIRE > 0)
        (!m_bot->HasAura(INNER_FIRE, EFFECT_INDEX_0) && ai->CastSpell (INNER_FIRE, *m_bot));

    // buff master
    if (FORTITUDE > 0)
        (!GetMaster()->HasAura(FORTITUDE, EFFECT_INDEX_0) && ai->CastSpell(FORTITUDE,*(GetMaster())) );

    // mana check
    if (m_bot->getStandState() != UNIT_STAND_STATE_STAND)
        m_bot->SetStandState(UNIT_STAND_STATE_STAND);

    Item* pItem = ai->FindDrink();
    Item* fItem = ai->FindBandage();

    if (pItem != NULL && ai->GetManaPercent() < 30)
    {
        ai->TellMaster("I could use a drink.");
        ai->UseItem(*pItem);
        ai->SetIgnoreUpdateTime(30);
        return;
    }

    // hp check
    if (m_bot->getStandState() != UNIT_STAND_STATE_STAND)
        m_bot->SetStandState(UNIT_STAND_STATE_STAND);

    pItem = ai->FindFood();

    if (pItem != NULL && ai->GetHealthPercent() < 30)
    {
        ai->TellMaster("I could use some food.");
        ai->UseItem(*pItem);
        ai->SetIgnoreUpdateTime(30);
        return;
    }
    else if (pItem == NULL && fItem != NULL && !m_bot->HasAura(RECENTLY_BANDAGED, EFFECT_INDEX_0) && ai->GetHealthPercent() < 70)
    {
        ai->TellMaster("I could use first aid.");
        ai->UseItem(*fItem);
        ai->SetIgnoreUpdateTime(8);
        return;
    }

    // buff and heal master's group
    if (GetMaster()->GetGroup())
    {
        Group::MemberSlotList const& groupSlot = GetMaster()->GetGroup()->GetMemberSlots();
        for (Group::member_citerator itr = groupSlot.begin(); itr != groupSlot.end(); itr++)
        {
            Player *tPlayer = sObjectMgr.GetPlayer(uint64 (itr->guid));
            if( !tPlayer )
                continue;

            // first rezz em
            if ( !tPlayer->isAlive() && !tPlayer->GetPlayerbotAI() )
            {
                std::string msg = "rezzing ";
                msg += tPlayer->GetName();
                GetPlayerBot()->Say(msg, LANG_UNIVERSAL);
                ai->CastSpell(REZZ, *tPlayer);
                // rez is only 10 sec, but give time for lag
                ai->SetIgnoreUpdateTime(17);
            }
            else if( tPlayer->isAlive() )
            {
                // buff and heal
                (!tPlayer->HasAura(FORTITUDE, EFFECT_INDEX_0) && ai->CastSpell (FORTITUDE, *tPlayer));
                (HealTarget(*tPlayer, tPlayer->GetHealth()*100 / tPlayer->GetMaxHealth()));
            }
        }
    }
} // end DoNonCombatActions

void PlayerbotPriestAI::BuffPlayer(Player* target)
{
    GetAI()->CastSpell(FORTITUDE, *target);
}
