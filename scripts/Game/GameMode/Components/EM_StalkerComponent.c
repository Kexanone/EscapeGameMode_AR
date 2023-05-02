/*
	AI groups assigned to this component via AddStalker will stalk players
*/

class EM_StalkerComponentClass : ScriptComponentClass
{
};

class EM_StalkerComponent : ScriptComponent
{
	[Attribute("10", UIWidgets.EditBox, "Timeout for position update in seconds", "")]
	protected int m_iUpdateTimeout;
	
	[Attribute("0 0 0", UIWidgets.Coords, "Initial position for stalker waypoint", "")]
	protected vector m_InitialStalkedPosition;
		
	protected ref array<SCR_AIGroup> m_Stalkers = {};
	protected SCR_AIWaypoint m_stalkerWaypoint;
	protected IEntity m_StalkedPlayer;
	private ref RandomGenerator m_randomGenerator = new RandomGenerator;

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
		
		m_stalkerWaypoint = SCR_AIWaypoint.Cast(EM_Utils.SpawnEntity("{750A8D1695BD6998}Prefabs/AI/Waypoints/AIWaypoint_Move.et", m_InitialStalkedPosition));
				
		GetGame().GetCallqueue().CallLater(Stalk, m_iUpdateTimeout*1000, true);
	};
	
	void AddStalker(SCR_AIGroup group)
	{
		m_Stalkers.Insert(group);
	};
	
	void ClearStalkers()
	{
		m_Stalkers.Clear();
	};
	
	void Stalk()
	{
		IEntity player = GetStalkedPlayer();
		
		// Dismiss all stalkers if no players are available
		if (!player)
		{
			ClearStalkers();
			return;
		};
				
		m_stalkerWaypoint.SetOrigin(player.GetOrigin());
		
		foreach (SCR_AIGroup group : m_Stalkers)
		{
			if (!group)
			{
				m_Stalkers.RemoveItem(group);
				continue;
			};
				
			
			if (group.GetCurrentWaypoint() != m_stalkerWaypoint)
			{
				group.AddWaypoint(m_stalkerWaypoint);
				array<AIWaypoint> wps = {};
				group.GetWaypoints(wps);
			};
		};
	};
	
	IEntity GetStalkedPlayer()
	{
		if (!m_StalkedPlayer || !(SCR_AIIsAlive.IsAlive(m_StalkedPlayer)))
		{
			m_StalkedPlayer = EM_Utils.GetRandomPlayer(true);
		};
		
		return m_StalkedPlayer;
	};
};
