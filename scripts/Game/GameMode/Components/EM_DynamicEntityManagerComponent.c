/*
	When an entity is added to this component via AddEntity, it will despawn once all players are too far away
	and respawn when players are close again
*/

class EM_DynamicEntityManagerComponentClass : SCR_BaseGameModeComponentClass
{
};

class EM_DynamicEntityManagerComponent : SCR_BaseGameModeComponent
{
	[Attribute("1200", UIWidgets.EditBox, "Distance in meters at which entity will despawn", "")]
	protected float m_fDespawnDistance;
	
	[Attribute("1000", UIWidgets.EditBox, "Distance in meters at which entity will respawn", "")]
	protected float m_fRespawnDistance;
	
	[Attribute("5", UIWidgets.EditBox, "Handler timeout in seconds", "")]
	protected float m_fHandlerTimeout;
	
	protected ref array<ref EM_DynamicEntityWrapper> m_DynamicEntities = {};
	protected ref array<ref EM_DynamicEntityWrapper> m_EntitiesToAdd = {};
	protected ref array<ref EM_DynamicEntityWrapper> m_EntitiesToRemove = {};
	protected ref array<IEntity> m_Players;
	protected float m_fRespawnDistanceSq;
	protected float m_fDespawnDistanceSq;
	protected int m_iEntityCheckedForProximity = 0;
	protected int m_iDynamicEntitiesCnt = 0;
	protected float m_fStepTimeout;
	protected float m_fStepTimer = 0;

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
		
		m_fRespawnDistanceSq = Math.Pow(m_fRespawnDistance, 2);
		m_fDespawnDistanceSq = Math.Pow(m_fDespawnDistance, 2);
		
		UpdateHandlerLists();
		SetEventMask(owner, EntityEvent.FRAME);
	};
	
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		m_fStepTimer += timeSlice;
		
		if (m_fStepTimer < m_fStepTimeout)
			return;
		
		m_fStepTimer = 0;
		
		if (m_iEntityCheckedForProximity < m_iDynamicEntitiesCnt)
		{
			EM_DynamicEntityWrapper entity = m_DynamicEntities[m_iEntityCheckedForProximity];
			HandleDynamicEntity(entity);
			m_iEntityCheckedForProximity++;
		}
		else
		{
			UpdateHandlerLists();
		};
	};
	
	void UpdateHandlerLists()
	{
		m_Players = EM_Utils.GetPlayers();
		
		// Remove entities marked for deletion
		foreach (EM_DynamicEntityWrapper entity : m_EntitiesToRemove)
		{
			m_DynamicEntities.RemoveItem(entity);
		};
		
		m_EntitiesToRemove = {};
		
		// Add entities marked for addition
		foreach (EM_DynamicEntityWrapper entity : m_EntitiesToAdd)
		{
			m_DynamicEntities.Insert(entity);
		};

		m_EntitiesToAdd = {};
		
		m_iEntityCheckedForProximity = 0;
		m_iDynamicEntitiesCnt = m_DynamicEntities.Count();
		
		// Update step timeout based on new list's size
		if (m_iDynamicEntitiesCnt > 0)
		{		
			m_fStepTimeout = m_fHandlerTimeout / m_iDynamicEntitiesCnt;
		}
		else
		{
			m_fStepTimeout = m_fHandlerTimeout;
		};
	};
	
	void HandleDynamicEntity(EM_DynamicEntityWrapper entity)
	{
		if (entity.IsSpawned())
		{
			bool markedForDespawning = false;
			
			foreach (IEntity player : m_Players)
			{
				float distanceSq = entity.DistanceSqXZ(player.GetOrigin());
				
				if (distanceSq <= m_fRespawnDistanceSq)
				{
					markedForDespawning = false;
					break;
				};
				
				if (distanceSq > m_fDespawnDistanceSq)
					markedForDespawning = true;
			};
			
			if (markedForDespawning)
			{					
				if (!entity.IsAlive())
					RemoveEntity(entity);
					
				entity.Despawn();
			};
		}
		else
		{
			foreach (IEntity player : m_Players)
			{
				if (entity.DistanceSqXZ(player.GetOrigin()) <= m_fRespawnDistanceSq)
				{
					entity.Spawn();
					break;
				};
			};
		};
	};
	
	void AddEntity(IEntity entity)
	{
		AIGroup group = AIGroup.Cast(entity);
		
		if (group)
		{
			m_EntitiesToAdd.Insert(EM_DynamicAIGroupWrapper(group));
		}
		else
		{
			m_EntitiesToAdd.Insert(EM_DynamicEntityWrapper(entity));
		};
	};
	
	void AddEntity(EM_DynamicEntityWrapper entity)
	{
		m_EntitiesToAdd.Insert(entity);
	};
	
	void RemoveEntity(EM_DynamicEntityWrapper entity)
	{
		m_EntitiesToRemove.Insert(entity);
	};
};

class EM_DynamicEntityWrapper : Managed
{
	IEntity m_SpawnedEntity;
	protected ResourceName m_sPrefabName;
	protected vector m_Transform[4];
	
	void EM_DynamicEntityWrapper(IEntity entity)
	{
		m_SpawnedEntity = entity;
		m_sPrefabName = entity.GetPrefabData().GetPrefabName();
	};
	
	void Despawn()
	{
		if (!m_SpawnedEntity)
			return;
		
		m_SpawnedEntity.GetTransform(m_Transform);
		SCR_EntityHelper.DeleteEntityAndChildren(m_SpawnedEntity);
		m_SpawnedEntity = null;
	};
	
	void Spawn()
	{
		if (m_SpawnedEntity)
			return;
		
		EntitySpawnParams params = EntitySpawnParams();
		params.TransformMode = ETransformMode.WORLD;
		params.Transform = m_Transform;
		Resource res = Resource.Load(m_sPrefabName);
		m_SpawnedEntity = GetGame().SpawnEntityPrefab(res, null, params);	
	};
	
	bool IsSpawned()
	{
		return m_SpawnedEntity;
	};
	
	// Returns null if entity is despawned
	IEntity GetEntity()
	{
		return m_SpawnedEntity;
	};
	
	bool IsAlive()
	{
		if (m_SpawnedEntity)
		{
			return SCR_AIDamageHandling.IsAlive(m_SpawnedEntity);
		}
		else
		{
			return false;
		};
	};
	
	float DistanceSqXZ(vector position)
	{
		if (m_SpawnedEntity)
		{
			return vector.DistanceSqXZ(m_SpawnedEntity.GetOrigin(), position);
		}
		else
		{
			return vector.DistanceSqXZ(m_Transform[3], position);
		};
	};
};

class EM_DynamicAIGroupBaseWrapper : EM_DynamicEntityWrapper
{
	protected int m_iInitialAgentCount;
	protected SCR_AIGroup m_SpawnedGroup;
	protected Faction m_Faction;
	protected ref array<ref EM_DynamicEntityWrapper> m_dynamicUnits = {};
	static private ResourceName m_sGroupBasePrefabName = "{000CD338713F2B5A}Prefabs/AI/Groups/Group_Base.et";
	
	void EM_DynamicAIGroupBaseWrapper(IEntity entity)
	{
		m_SpawnedGroup = SCR_AIGroup.Cast(entity);
		m_Faction = m_SpawnedGroup.GetFaction();
		// Override group prefab by its base to make unit allocation dynamic
		m_sPrefabName = m_sGroupBasePrefabName;
	};
	
	override void Despawn()
	{
		if (!m_SpawnedGroup)
			return;
		
		array<AIAgent> agents = {};
		m_SpawnedGroup.GetAgents(agents);		
		m_dynamicUnits = {};
		
		foreach (AIAgent agent : agents)
		{
			IEntity unit = agent.GetControlledEntity();
			
			if (!unit)
				continue;
			
			EM_DynamicEntityWrapper dynamicUnit = EM_DynamicEntityWrapper(unit);
			m_dynamicUnits.Insert(dynamicUnit);
			dynamicUnit.Despawn();
		};
		
		m_SpawnedGroup = null;
		super.Despawn();
	};
	
	override void Spawn()
	{
		super.Spawn();
		m_SpawnedGroup = SCR_AIGroup.Cast(m_SpawnedEntity);
		
		if (!m_SpawnedGroup)
			return;
		
		foreach (EM_DynamicEntityWrapper dynamicUnit : m_dynamicUnits)
		{
			dynamicUnit.Spawn();
			m_SpawnedGroup.AddAIEntityToGroup(dynamicUnit.GetEntity(), 0);
		};
		
		m_SpawnedGroup.SetFaction(m_Faction);
	};
	
	override bool IsAlive()
	{
		if (!m_SpawnedGroup)
			return false;
		
		array<AIAgent> agents = {};
		m_SpawnedGroup.GetAgents(agents);
		
		return !agents.IsEmpty();
	};
};

class EM_DynamicAIGroupWrapper : EM_DynamicAIGroupBaseWrapper
{
	protected string m_sFormationName;
	protected ref array<AIWaypoint> m_Waypoints = {};
	
	override void Despawn()
	{
		if (!m_SpawnedGroup)
			return;
		
		m_SpawnedGroup.GetWaypoints(m_Waypoints);
		
		AIFormationDefinition formationDef = m_SpawnedGroup.GetFormation();
				
		if (!formationDef)
			return;
		
		m_sFormationName = formationDef.GetName();
		
		super.Despawn();
	};
	
	override void Spawn()
	{
		super.Spawn();
		
		if (!m_SpawnedGroup)
			return;
		
		foreach (AIWaypoint wp : m_Waypoints)
		{
			m_SpawnedGroup.AddWaypoint(wp);
		};
		
		AIFormationComponent formationHandler = AIFormationComponent.Cast(m_SpawnedGroup.FindComponent(AIFormationComponent));
		
		if (!formationHandler)
			return;
		
		formationHandler.SetFormation(m_sFormationName);
	};
};

class EM_DynamicStalkerWrapper : EM_DynamicAIGroupBaseWrapper
{
	static EM_StalkerComponent stalkerComp;
	
	void EM_DynamicStalkerWrapper(IEntity entity)
	{
		if (!stalkerComp)
			stalkerComp = EM_StalkerComponent.Cast(GetGame().GetGameMode().FindComponent(EM_StalkerComponent));
	};
	
	override void Despawn()
	{
		if (!m_SpawnedGroup)
			return;
		
		if (stalkerComp)
			stalkerComp.RemoveStalker(m_SpawnedGroup);
				
		super.Despawn();
	};
	
	override void Spawn()
	{
		super.Spawn();
		
		if (!m_SpawnedGroup)
			return;
		
		if (stalkerComp)
			stalkerComp.AddStalker(m_SpawnedGroup);		
	};
};
