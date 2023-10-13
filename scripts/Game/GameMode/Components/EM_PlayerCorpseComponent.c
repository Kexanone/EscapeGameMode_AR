/*
	This component ensures that there's only at maximum one corpse of each player present
*/

class EM_PlayerCorpseComponentClass : SCR_BaseGameModeComponentClass
{
};

class EM_PlayerCorpseComponent : SCR_BaseGameModeComponent
{
	protected ref map<int,IEntity> m_Corpses = new map<int,IEntity>();

	override void OnPlayerKilled(int playerId, IEntity playerEntity, IEntity killerEntity, notnull Instigator killer)
	{
		IEntity corpse = m_Corpses.Get(playerId);
		
		if (corpse)
		{
			SCR_EntityHelper.DeleteEntityAndChildren(corpse);
		};
		
		m_Corpses.Set(playerId, playerEntity);
	};
	
	override void OnPlayerDisconnected(int playerId, KickCauseCode cause, int timeout)
	{
		IEntity corpse = m_Corpses.Get(playerId);
		
		if (corpse)
		{
			SCR_EntityHelper.DeleteEntityAndChildren(corpse);
		};
		
		m_Corpses.Remove(playerId);
	};
};
