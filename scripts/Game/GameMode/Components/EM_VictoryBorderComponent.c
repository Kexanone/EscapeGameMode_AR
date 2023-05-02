/*
	This component ends the game in victory when a player left the specified border
*/

class EM_VictoryBorderComponentClass : ScriptComponentClass
{
};

class EM_VictoryBorderComponent : ScriptComponent
{
	[Attribute("10", UIWidgets.EditBox, "Timeout for border check in seconds", "")]
	protected int m_iHandlerTimeout;
	[Attribute(UIWidgets.Auto, "Polygon that forms the border")];
	protected ref array<vector> m_BorderPolygon;

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

		GetGame().GetCallqueue().CallLater(Handler, m_iHandlerTimeout*1000, true);
	};
	
	void Handler()
	{
		foreach (IEntity player : EM_Utils.GetPlayers(true))
		{
			if (!Math2D.IsPointInPolygonXZ(m_BorderPolygon, player.GetOrigin()))
			{
				EM_GameModeEscape gameMode = EM_GameModeEscape.Cast(GetOwner());
				if (!gameMode)
					return;
				
				GetGame().GetCallqueue().Remove(Handler);
				gameMode.Finish();
				return;
			};
		};
	};
};
