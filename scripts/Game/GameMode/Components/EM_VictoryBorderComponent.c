/*
	This component ends the game in victory when a player inside a vehicle left the specified border
*/

class EM_VictoryBorderComponentClass : SCR_BaseGameModeComponentClass
{
};

class EM_VictoryBorderComponent : SCR_BaseGameModeComponent
{
	[Attribute("0", desc: "Game over type", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EGameOverTypes))]
	protected EGameOverTypes m_iGameOverType;
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
			IEntity vehicle = IEntity.Cast(player.GetParent());
						
			if (!vehicle)
				continue;
	
			Print(vehicle.GetOrigin().ToString());
			if (!Math2D.IsPointInPolygonXZ(m_BorderPolygon, vehicle.GetOrigin()))
			{
				m_pGameMode.EndGameMode(SCR_GameModeEndData.CreateSimple(m_iGameOverType));
				return;
			};
		};
	};
};
