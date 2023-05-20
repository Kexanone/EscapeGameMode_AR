/*
	This component deletes the initial spawn point if a player is far enough from it
*/

class EM_InitialSpawnPointDeleterComponentClass : SCR_BaseGameModeComponentClass
{
};

class EM_InitialSpawnPointDeleterComponent : SCR_BaseGameModeComponent
{
	[Attribute("InitialSpawnPoint", UIWidgets.EditBox, "Name of the initial spawn point to delete", "")]
	protected string m_sSpawnPointName;
	
	[Attribute("60", UIWidgets.EditBox, "Timeout for checking in seconds", "")]
	protected int m_iHandlerTimeout;
	
	[Attribute("300", UIWidgets.EditBox, "How far a player has to be in meters for deleting the spawn point", "")]
	protected float m_fEventRadius;
	
	protected IEntity m_InitialSpawnPoint;
	protected vector m_startPosition;

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
		
		m_InitialSpawnPoint = GetGame().GetWorld().FindEntityByName(m_sSpawnPointName);
		
		if (!m_InitialSpawnPoint)
			return;
		
		m_startPosition = m_InitialSpawnPoint.GetOrigin();
				
		GetGame().GetCallqueue().CallLater(Handler, m_iHandlerTimeout*1000, true);
	};
	
	void Handler()
	{
		foreach (IEntity player : EM_Utils.GetPlayers(true))
		{
			if (vector.DistanceXZ(player.GetOrigin(), m_startPosition) > m_fEventRadius)
			{
				SCR_EntityHelper.DeleteEntityAndChildren(m_InitialSpawnPoint);
				
				GetGame().GetCallqueue().Remove(Handler);
				return;
			};
		};
	};
};
