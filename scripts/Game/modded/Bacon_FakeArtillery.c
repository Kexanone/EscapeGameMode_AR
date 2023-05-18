enum EM_DistributionMode {
	UNIFORM,
	CENTER_BIASED
};

modded class Bacon_FakeArtillery: GenericEntity {
	protected EM_DistributionMode m_iDistributionMode = EM_DistributionMode.UNIFORM;
		
	override void SpawnNewProjectile() {
		ref EntitySpawnParams spawnParams = new EntitySpawnParams();
		spawnParams.TransformMode = ETransformMode.WORLD;
		
		vector ownerTransform[4];
		GetWorldTransform(ownerTransform);
		
		ref Resource projectileResource = Resource.Load(m_projectilePrefab);
		if (!projectileResource) {
			Print("Unable to load projectile prefab");
			return;
		};
		
		vector targetPos;
		
		switch (m_iDistributionMode)
		{
			case EM_DistributionMode.UNIFORM:
			{
				targetPos = m_RandomGenerator.GenerateRandomPointInRadius(0, m_areaRadius, ownerTransform[3], true);
				break;
			};
			
			case EM_DistributionMode.CENTER_BIASED:
			{
				float angle = Math.RandomFloat(0, 360);
				float radius = Math.RandomFloat(0, m_areaRadius);
				targetPos = ownerTransform[3];
				targetPos[0] = targetPos[0] + radius * Math.Cos(angle);
				targetPos[2] = targetPos[2] + radius * Math.Sin(angle);
				break;
			};
		};
		
		ownerTransform[3] = targetPos;
		SCR_TerrainHelper.SnapToTerrain(ownerTransform, null, true);
		
		spawnParams.Transform = ownerTransform;
		
		IEntity spawnedProjectile = GetGame().SpawnEntityPrefab(projectileResource, GetWorld(), spawnParams);
	};
	
	EM_DistributionMode EditorAttributeGetDistributionMode() {
		return m_iDistributionMode;
	};
	
	void EditorAttributeSetDistributionMode(EM_DistributionMode value) {
		m_iDistributionMode = value;
	};
};