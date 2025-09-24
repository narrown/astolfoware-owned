#include "CritHack.h"
#include "MD5.h"
#include "CreateMove.h"
#include "Vars.h"
#include "Utils.h"
#define MASK_SIGNED 0x7FFFFFFF


int LastGoodCritTick(const CUserCmd* pCmd, int crittick)
{
    int retVal = -1;
    bool popBack = false;

    if (crittick >= pCmd->command_number)
    {
        retVal = crittick;
    }
    else {
        retVal = 0;
    }
    return retVal;
}
int CCrithack::GetCritCommand(CUserCmd* pCmd, CBaseCombatWeapon* pWeapon, bool bCrit)
{
    int nCommandNumber = pCmd->command_number;
    int nBackupRandomSeed = *i::randomseed;
    int nCritCommand = nCommandNumber;
    int loopCount = 0;

    for (int n = 0; n < 230; n++)
    {
        int crit = nCommandNumber + n;
        *i::randomseed = MD5_PseudoRandom(crit) & MASK_SIGNED;
        loopCount++;
        bool bCalc = false;

        if (gLocalWeapon->GetSlot() == 2)
            bCalc = gLocalWeapon->CalcIsAttackCriticalHelperMelee();

        else bCalc = gLocalWeapon->CalcIsAttackCriticalHelper();

        if (bCrit ? bCalc : !bCalc)
        {
            nCritCommand = crit;
            //didCrit = false;
            break;
        }

        crit++;

    }
    //nCommandNumber += n;
    *i::randomseed = nBackupRandomSeed;
    return nCritCommand;
}
void CCrithack::init(CUserCmd* pCmd)
{
    if (!gLocalPlayer || !gLocalPlayer->IsAlive() || gLocalPlayer->IsCritBoosted() || gLocalPlayer->IsMiniCritBoosted())
        return;
    if (!gLocalWeapon)
        return;
    if (i::cvars->FindVar("tf_weapon_criticals")->GetInt() == 0)
        return;
    if (gLocalWeapon->GetWeaponID() == TF_WEAPON_KNIFE)
        return;

    int critCommand = GetCritCommand(pCmd, gLocalWeapon, true);
    int closestGoodTick = LastGoodCritTick(pCmd, critCommand);
    if ((GetAsyncKeyState(VK_CONTROL)))
    {
        if (closestGoodTick == 0)
            return;
        pCmd->command_number = closestGoodTick; //	set our cmdnumber to our wish
        pCmd->random_seed = MD5_PseudoRandom(closestGoodTick) & MASK_SIGNED;
    }
    else if (Vars::CritHax::SkipRandomCrits)
    {
        for (int tries = 1; tries < 250; tries++)
        {
            if (pCmd->command_number + tries == closestGoodTick)
            {
                continue; // what a useless attempt
            }
            pCmd->command_number += tries;
            pCmd->random_seed = MD5_PseudoRandom(pCmd->command_number) & MASK_SIGNED;
            break; //	we found a seed that we can use to avoid a crit and have skipped to it, woohoo
        }
    }
}