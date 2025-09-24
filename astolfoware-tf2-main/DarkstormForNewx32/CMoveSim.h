#pragma once
#include "SDK.h"
#include <map>
#include <deque>
class CMovementSimulation
{
private:
    C_BasePlayer* m_pPlayer = nullptr;
    CMoveData m_MoveData = {};

private:
    void SetupMoveData(C_BasePlayer* pPlayer, CMoveData* pMoveData);

private:
    bool m_bOldInPrediction = false;
    bool m_bOldFirstTimePredicted = false;
    float m_flOldFrametime = 0.0f;

public:
    bool StrafePredLocal = false;
    std::map<int, std::deque<Vector>> m_Velocities;
    inline static std::vector<Vector> vecPositions;
    bool Initialize(C_BasePlayer* pPlayer);
    void FillVelocities();
    bool BetaStrafePred();
    void Restore();
    void RunTick();
    bool bFirstRunTick = false;

    inline const Vector& GetOrigin() { return m_MoveData.m_vecAbsOrigin; }
    inline const Vector& GetVelocity() {
        return m_MoveData.m_vecVelocity;
    }
    inline float GetForwardMove()
    {
        return m_MoveData.m_flForwardMove;
    }
    inline int GetButtons()
    {
        return m_MoveData.m_nButtons;
    }
};

inline CMovementSimulation gMovementSim;