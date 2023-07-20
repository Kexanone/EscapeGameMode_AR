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

#ifdef WORKBENCH
	[Attribute(defvalue: "1", desc: "Show the debug shapes in Workbench", category: "Debug")];
	protected bool m_bShowDebugShapesInWorkbench;
#endif

	override void OnPostInit(IEntity owner)
	{
		// Run handler only on server
		if (!GetGame().InPlayMode() || !Replication.IsServer())
			return;
		
		GetGame().GetCallqueue().CallLater(Handler, m_iHandlerTimeout*1000, true);
	};
	
	void Handler()
	{
		foreach (IEntity player : EM_Utils.GetPlayers(true))
		{
			Vehicle vehicle = Vehicle.Cast(CompartmentAccessComponent.GetVehicleIn(player));
						
			if (!vehicle)
				continue;
			
			// Workaround: Temporarily trigger victory when a player is in helo no matter the position
			if (R3D_HeliBase.Cast(vehicle) || !Math2D.IsPointInPolygonXZ(m_BorderPolygon, vehicle.GetOrigin()))
			{
				m_pGameMode.EndGameMode(SCR_GameModeEndData.CreateSimple(m_iGameOverType));
				return;
			};
		};
	};
	
#ifdef WORKBENCH
	protected void DrawDebugShape(bool draw)
	{
		if (!draw)
			return;
		
		int cnt = m_BorderPolygon.Count();
		
		if (!cnt)
			return;
		
		vector p[100];
		
		for (int i = 0; i < cnt; i++)
		{
			vector pos =  m_BorderPolygon[i];
			pos[1] = Math.Max(SCR_TerrainHelper.GetTerrainY(pos), 0) + 5;
			p[i] = pos;
		};
		
		p[cnt] = p[0];
		
		Shape.CreateLines(
			Color.DARK_GREEN, // ARGB(32, 0x99, 0xF3, 0x12),
			ShapeFlags.ONCE | ShapeFlags.NOZWRITE,
			p,
			Math.Min(cnt + 1, 100)
		);
	};
	
	override void _WB_AfterWorldUpdate(IEntity owner, float timeSlice)
	{
		DrawDebugShape(m_bShowDebugShapesInWorkbench);
	};
	
	override bool _WB_OnKeyChanged(IEntity owner, BaseContainer src, string key, BaseContainerList ownerContainers, IEntity parent)
	{
		if (key == "m_bShowDebugShapesInWorkbench")
			DrawDebugShape(m_bShowDebugShapesInWorkbench);
		
		return false;
	};
#endif
};
