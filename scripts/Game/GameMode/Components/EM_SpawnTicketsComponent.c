/*
	This component implements spawn tickets and disables respawn if no tickets are left
*/

class EM_SpawnTicketsComponentClass : ScriptComponentClass
{
};

class EM_SpawnTicketsComponent : ScriptComponent
{
	[Attribute("-1", UIWidgets.EditBox, "", "")]
	protected int m_iInitialTicketsCount;

	[Attribute("1", desc: "Scenario header setting will overwrite these values.")];
	protected bool m_bAllowHeaderSettings;
	
	[RplProp()]
	protected int m_iTicketsCount;
	
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
		
		// Initialie tickets
		m_iTicketsCount = m_iInitialTicketsCount;
		if (m_bAllowHeaderSettings)
		{
			EM_MissionHeaderEscape header = EM_MissionHeaderEscape.Cast(GetGame().GetMissionHeader());
			
			if (header)
				m_iTicketsCount = header.m_iRespawnTickets;
		};
		
		// Tickets disabled
		if (m_iTicketsCount < 0)
			return;

		// Run handler only on authority		
		if (!EM_Utils.IsAuthority(owner))
			return;
		
		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetOwner());
		
		if (!gameMode)
			return;
		
		gameMode.GetOnPlayerSpawned().Insert(OnPlayerSpawned);
	};
	
	void OnPlayerSpawned(int playerId, IEntity controlledEntity)
	{
		ConsumeTicket();
	};
	
	void ConsumeTicket()
	{
		m_iTicketsCount -= 1;
		Replication.BumpMe();
		
		if (m_iTicketsCount == 0)
			DisableSpawn();
	};
	
	void DisableSpawn()
	{
		SCR_RespawnSystemComponent respawnSystem = SCR_RespawnSystemComponent.GetInstance();
		
		if (!respawnSystem)
			return;
		
		respawnSystem.ServerSetEnableRespawn(false);
	};
	
	int GetTicketsCount()
	{
		return m_iTicketsCount;
	};
};
