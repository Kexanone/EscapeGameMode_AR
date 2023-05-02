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
		
		int i = 1;
		while (i <= count)
		{
			vector pos = m_RandomGenerator.GenerateRandomPointInRadius(0, radius, position);
			float y = SCR_TerrainHelper.GetTerrainY(pos);
			pos[1] = y; 
			if (vector.Distance(prevPos, pos) >= minDistance)
			{
				group.AddWaypoint(AIWaypoint.Cast(EM_Utils.SpawnEntity("{22A875E30470BD4F}Prefabs/AI/Waypoints/AIWaypoint_Patrol.et", pos)));
				prevPos = pos;
				i++;
			};
		};
		AIWaypointCycle wpCycle = AIWaypointCycle.Cast(EM_Utils.SpawnEntity("{35BD6541CBB8AC08}Prefabs/AI/Waypoints/AIWaypoint_Cycle.et", prevPos));
		group.AddWaypoint(wpCycle);
		array<AIWaypoint> wpList = {};
		group.GetWaypoints(wpList);
		wpCycle.SetWaypoints(wpList);		
	};
};