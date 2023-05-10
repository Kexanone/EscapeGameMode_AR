/*
	This component ends the game in defeat when no player is alive and no spawns are available
*/

class EM_NoSpawnDefeatComponentClass : ScriptComponentClass
{
};

class EM_NoSpawnDefeatComponent : ScriptComponent
{
	[Attribute("0", desc: "Game over type", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EGameOverTypes))]
	protected EGameOverTypes m_iGameOverType;
	
	protected SCR_RespawnSystemComponent m_RespawnSystem;
	protected SCR_BaseGameMode m_GameMode;

	override void OnPostInit(IEntity owner)
	{
		if (!GetGame().InPlayMode())
			return;
		
		SetEventMask(owner, EntityEvent.INIT);
		owner.SetFlags(EntityFlags.ACTIVE, true);

		super.OnPostInit(owner);
	};
	
	override void EOnInit(IEntity owner)
	{
		super.EOnInit(owner);
		
		// Run handler only on authority		
		if (!EM_Utils.IsAuthority(owner))
			return;	
		
		m_RespawnSystem = SCR_RespawnSystemComponent.GetInstance();
		
		if (!m_RespawnSystem)
			return;

		m_GameMode = SCR_BaseGameMode.Cast(GetOwner());
		
		if (!m_GameMode)
			return;
		
		m_GameMode.GetOnPlayerKilled().Insert(Handler);
		m_GameMode.GetOnPlayerDisconnected().Insert(OnPlayerDisconnected);
	};
	
	void OnPlayerDisconnected(int playerId, KickCauseCode cause, int timeout)
	{
		Handler();
	};
	
	void Handler(void param1 = NULL, void param2 = NULL, void param3 = NULL)
	{
		if (m_RespawnSystem.IsRespawnEnabled())
		{
			foreach (SCR_SpawnPoint spawnpoint : SCR_SpawnPoint.GetSpawnPoints())
			{
				if (!SCR_PlayerSpawnPoint.Cast(spawnpoint))
					return; // There's still a non-player spawnpoint available
			};
		};
		
		if (!EM_Utils.GetPlayers(true).IsEmpty())
			return; // There are still players alive
		
		GetGame().GetCallqueue().Call(m_GameMode.EndGameMode, SCR_GameModeEndData.CreateSimple(m_iGameOverType));
	};
};
