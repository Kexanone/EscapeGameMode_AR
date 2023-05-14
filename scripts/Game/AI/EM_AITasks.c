class EM_AITasks
{
	static private ref RandomGenerator m_RandomGenerator = new RandomGenerator;
	
	static void Stalk(SCR_AIGroup group)
	{
		EM_StalkerComponent stalkerHandler = EM_StalkerComponent.Cast(GetGame().GetGameMode().FindComponent(EM_StalkerComponent));
		
		if (!stalkerHandler)
			return;
		
		stalkerHandler.AddStalker(group);
	};
	
	static void Patrol(SCR_AIGroup group, vector position, float radius = 100, int count = 3, float minDistance = 50)
	{
		AIFormationComponent formationHandler = AIFormationComponent.Cast(group.FindComponent(AIFormationComponent));
		
		if (!formationHandler)
			return;
		
		formationHandler.SetFormation("Column");
		
		vector prevPos = group.GetLeaderEntity().GetOrigin();
		prevPos[1] = SCR_TerrainHelper.GetTerrainY(prevPos);
		
		array<AIWaypoint> wpList = {};
		int i = 1;
		
		while (i <= count)
		{
			vector pos = m_RandomGenerator.GenerateRandomPointInRadius(0, radius, position);
			pos[1] = SCR_TerrainHelper.GetTerrainY(pos);
			if (vector.Distance(prevPos, pos) >= minDistance)
			{
				AIWaypoint wp = AIWaypoint.Cast(EM_Utils.SpawnEntity("{C0A9A9B589802A5B}PrefabsEditable/Auto/AI/Waypoints/E_AIWaypoint_Patrol.et", pos));
				group.AddWaypoint(wp);
				wpList.Insert(wp);
				prevPos = pos;
				i++;
			};
		};
		
		AIWaypointCycle wpCycle = AIWaypointCycle.Cast(EM_Utils.SpawnEntity("{35BD6541CBB8AC08}Prefabs/AI/Waypoints/AIWaypoint_Cycle.et", prevPos));
		wpCycle.SetWaypoints(wpList);
		group.AddWaypoint(wpCycle);
	};
};