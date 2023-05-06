[EntityEditorProps(category: "GameScripted/EscapeGameMode", description: "Escape game mode script.", color: "0 0 255 255")]
class EM_GameModeEscapeClass: SCR_BaseGameModeClass
{
};

class EM_GameModeEscape : SCR_BaseGameMode
{
	// -- TO DO: Implement respawn ticket handler
	[Attribute("-1", UIWidgets.EditBox, "", "")]
	protected int m_iRespawnTickets;
	
	protected override void OnPlayerDisconnected(int playerId, KickCauseCode cause, int timeout)
	{
		super.OnPlayerDisconnected(playerId, cause, timeout);
		
		if (!CheckMissionFailure())
			return;
	
		// Mission failed, we get them next time!
		Finish(false);
	};
	
	override void OnPlayerKilled(int playerId, IEntity player, IEntity killer)
	{
		super.OnPlayerKilled(playerId, player, killer);
		
		if (!CheckMissionFailure())
			return;
		
		// Mission failed, we get them next time!
		Finish(false);
	};
	
	bool CheckMissionFailure()
	{
		// False if initial spawn point still exists
		if (GetGame().GetWorld().FindEntityByName("InitialSpawnPoint"))
			return false;
		
		// False if there is still a player alive
		PlayerManager playerManager = GetGame().GetPlayerManager();
		if (!playerManager)
			return false;
		array<int> playerIds = {};
		playerManager.GetPlayers(playerIds);
		foreach(int otherPlayerId : playerIds)
		{
			IEntity otherPlayer = playerManager.GetPlayerControlledEntity(otherPlayerId);
			
			if (!otherPlayer)
				continue;
			
			if (SCR_AIDamageHandling.IsAlive(otherPlayer))
			{
				return false; // There's still a player alive
			};
		};
		return true;
	};
	
	// -- TO DO: Make custom game mode end screen
	void Finish(bool victory = true)
	{
		int winnerId = -1;
		FactionManager factionManager = GetGame().GetFactionManager();
		if (victory)
		{
			winnerId = factionManager.GetFactionIndex(factionManager.GetFactionByKey("US"));
		}
		else
		{
			winnerId = factionManager.GetFactionIndex(factionManager.GetFactionByKey("USSR"));
		};
		
		EndGameMode(SCR_GameModeEndData.CreateSimple(SCR_GameModeEndData.ENDREASON_EDITOR_FACTION_VICTORY, -1, winnerId));
		if (RplSession.Mode() == RplMode.Dedicated)
		{
			GetGame().GetCallqueue().CallLater(QuitGame, 30000);	
		}
	};
	
	protected void QuitGame()
	{
		GetGame().RequestClose();
	};
};
