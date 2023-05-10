/*
	This component ends the game in victory when a player left the specified border
*/

class EM_VictoryBorderComponentClass : ScriptComponentClass
{
};

class EM_VictoryBorderComponent : ScriptComponent
{
	[Attribute("0", desc: "Game over type", uiwidget: UIWidgets.ComboBox, enums: ParamEnumArray.FromEnum(EGameOverTypes))]
	protected EGameOverTypes m_iGameOverType;
	[Attribute("10", UIWidgets.EditBox, "Timeout for border check in seconds", "")]
	protected int m_iHandlerTimeout;
	[Attribute(UIWidgets.Auto, "Polygon that forms the border")];
	protected ref array<vector> m_BorderPolygon;

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
		
		m_GameMode = SCR_BaseGameMode.Cast(GetOwner());
		
		if (!m_GameMode)
			return;

		GetGame().GetCallqueue().CallLater(Handler, m_iHandlerTimeout*1000, true);
	};
	
	void Handler()
	{
		foreach (IEntity player : EM_Utils.GetPlayers(true))
		{
			if (!Math2D.IsPointInPolygonXZ(m_BorderPolygon, player.GetOrigin()))
			{
				m_GameMode.EndGameMode(SCR_GameModeEndData.CreateSimple(m_iGameOverType));
				return;
			};
		};
	};
};
