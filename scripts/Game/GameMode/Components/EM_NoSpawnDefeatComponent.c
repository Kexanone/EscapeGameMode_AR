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
		
		m_GameMode.GetOnPlayerKilled().Insert(GetOnPlayerKilled);
		m_GameMode.GetOnPlayerDisconnected().Insert(OnPlayerDisconnected);
	};

	void GetOnPlayerKilled(int playerId, IEntity player, IEntity killer)
	{
		Handler(player);
	};
	
	void OnPlayerDisconnected(int playerId, KickCauseCode cause, int timeout)
	{
		IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
		Handler(player);
	};
	
	void Handler(IEntity player, bool __triggerDefeat = false)
	{
		if (!IsDefeatImminent(player))
			return;
		
		if (__triggerDefeat)
		{
			m_GameMode.EndGameMode(SCR_GameModeEndData.CreateSimple(m_iGameOverType));
		}
		else
		{
			// We'll check 5 seconds later and if defeat is still imminent, we'll trigger the defeat
			GetGame().GetCallqueue().CallLater(Handler, 5000, false, null, true);
		};
	};
	
	bool IsDefeatImminent(IEntity player)
	{
		if (m_RespawnSystem.IsRespawnEnabled())
		{
			foreach (SCR_SpawnPoint spawnpoint : SCR_SpawnPoint.GetSpawnPoints())
			{
				if (!SCR_PlayerSpawnPoint.Cast(spawnpoint))
					return false; // There's still a non-player spawnpoint available
			};
		};
					
		array<IEntity> alivePlayers = EM_Utils.GetPlayers(true);

		if (player) // When called by OnPlayerDisconnected, the player might still be registered as alive, so we remove the player
			alivePlayers.RemoveItem(player);
					
		if (!alivePlayers.IsEmpty())
			return false; // There are still players alive
		
		return true;
	};
};
