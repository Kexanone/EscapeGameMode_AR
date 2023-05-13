/*
	This component ensures that there's only at maximum one corpse of each player present
*/

class EM_PlayerCorpseComponentClass : ScriptComponentClass
{
};

class EM_PlayerCorpseComponent : ScriptComponent
{
	protected ref map<int,IEntity> m_Corpses = new map<int,IEntity>();

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

		SCR_BaseGameMode gameMode = SCR_BaseGameMode.Cast(GetOwner());
		
		if (!gameMode)
			return;
		
		gameMode.GetOnPlayerKilled().Insert(OnPlayerKilled);
		gameMode.GetOnPlayerDisconnected().Insert(OnPlayerDisconnected);
	};
	
	void OnPlayerKilled(int playerId, IEntity player, IEntity killer)
	{
		IEntity corpse = m_Corpses.Get(playerId);
		
		if (corpse)
		{
			SCR_EntityHelper.DeleteEntityAndChildren(corpse);
		};
		
		m_Corpses.Set(playerId, player);
	};
	
	void OnPlayerDisconnected(int playerId, KickCauseCode cause, int timeout)
	{
		IEntity corpse = m_Corpses.Get(playerId);
		
		if (corpse)
		{
			SCR_EntityHelper.DeleteEntityAndChildren(corpse);
		};
		
		m_Corpses.Remove(playerId);
	};
};
