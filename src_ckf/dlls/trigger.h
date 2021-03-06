#ifndef TRIGGER_H
#define TRIGGER_H

class CBaseTrigger : public CBaseToggle
{
public:
	void KeyValue(KeyValueData *pkvd);
	int ObjectCaps(void) { return CBaseToggle::ObjectCaps() & ~FCAP_ACROSS_TRANSITION; }

public:
	void EXPORT TeleportTouch(CBaseEntity *pOther);
	void EXPORT MultiTouch(CBaseEntity *pOther);
	void EXPORT HurtTouch (CBaseEntity *pOther);
	void EXPORT CDAudioTouch(CBaseEntity *pOther);
	void ActivateMultiTrigger(CBaseEntity *pActivator);
	void EXPORT MultiWaitOver(void);
	void EXPORT CounterUse(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);
	void EXPORT ToggleUse(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value);
	void InitTrigger(void);
};

class CControlPoint : public CBaseTrigger
{
public:
	CControlPoint::CControlPoint(void)
	{
		m_iSubEntity = 0;
		m_szName[0] = 0;
		m_iTrigOnRedCap = 0;
		m_iTrigOnBluCap = 0;
		m_iTrigOnCap = 0;
		m_iTimeAdded = 0;
		m_iHUDPosition = 0;
		m_flCaptureRate = 0;
		m_bStartLock = FALSE;
		m_iIndex = 0;
		m_iState = 0;
		m_iCapTeam = 0;
		m_iCapPlayers = 0;		
		m_flProgress = 0;
		m_flCapRate = 0;
		m_iClientState = 0;
		m_iClientCapTeam = 0;
		m_iClientCapPlayers = 0;
		m_flClientCapRate = 0;
		m_flClientSendState = 0;
	}
public:
	void Spawn(void);
	void PostSpawn(void);
	void KeyValue(KeyValueData *pkvd);
	void Restart(void);
	int Classify(void){ return CLASS_CONTROLPOINT; }
public:
	void ControlPointTouch(CBaseEntity *pOther);
	void ControlPointThink(void);
	void ControlPointCapturing(int iCapTeam);
	void ControlPointCaptured(int iNewTeam);
	void ControlPointDefend(int iCapTeam);
	void UpdateHUD(void);
public:
	//Map Static
	CBaseEntity *m_pSubEntity;
	string_t m_iSubEntity;
	string_t m_iTrigOnRedCap;
	string_t m_iTrigOnBluCap;
	string_t m_iTrigOnCap;
	char m_szName[32];
	float m_flCaptureRate;
	int m_iOriginTeam;
	int m_iHUDPosition;
	int m_iTimeAdded;
	BOOL m_bStartLock;
	int m_iIndex;
public:
	//Dynamic
	int m_iState;
	int m_iCapTeam;
	int m_iCapPlayers;
	float m_flProgress;
	float m_flCapRate;//for client predict
	int m_iClientState;//for client predict
	int m_iClientCapTeam;//for client predict
	int m_iClientCapPlayers;//for client predict
	float m_flClientCapRate;//for client predict
	float m_flClientSendState;//for client predict
};

enum
{
	CP_IDLE = 0,
	CP_CAPTURING,
	CP_BLOCKED,
	CP_BACK,
	CP_LOCKED
};

#define MAP_NOBUILDABLE 1
#define MAP_CONTROLPOINT 2

#endif