/*
	This component executes special events in a loop:
	- Artillery barrage
*/

class EM_SpecialEventsComponentClass : ScriptComponentClass
{
};

class EM_SpecialEventsComponent : ScriptComponent
{
	// -- TO DO: Make events an attribute
	[Attribute("10", UIWidgets.EditBox, "Minimum timeout for next special event in minutes", "")]
	protected int m_iMinTimeout;
	[Attribute("15", UIWidgets.EditBox, "Maximum timeout for next special event in minutes", "")]
	protected int m_iMaxTimeout;
	// -- TO DO: Make sub configs for each special event
	[Attribute("{35E0DB90D652D713}Prefabs/Systems/Artillery/ArtilleryBarrage_Huge.et", UIWidgets.ResourcePickerThumbnail, "Artillery module prefab", "et")]
	protected ResourceName m_ArtyResName;
	[Attribute("{93FC2145F05A1EEE}Prefabs/Systems/Sound/sound_airraidsiren.et", UIWidgets.ResourcePickerThumbnail, "Siren module prefab", "et")]
	protected ResourceName m_SirenResName;
	
	protected IEntity m_targetEntity;
	protected vector m_targetPos;
	private ref RandomGenerator m_randomGenerator = new RandomGenerator;

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
		
		//GetGame().GetCallqueue().CallLater(SelectEvent, m_randomGenerator.RandInt(m_iMinTimeout*60000, m_iMaxTimeout*60000));

	};
	
	void SelectEvent()
	{
		int eventId = m_randomGenerator.RandInt(1, 4);
		switch (eventId)
		{
			case 1: {ArtilleryEvent(); break; };
			default: {};
		}
		GetGame().GetCallqueue().CallLater(SelectEvent, m_randomGenerator.RandInt(m_iMinTimeout*60000, m_iMaxTimeout*60000));
	};
	
	void ArtilleryEvent()
	{	
		m_targetEntity = EM_Utils.GetRandomPlayer(true);
		
		if (!m_targetEntity)
			return;
		
		m_targetPos = m_targetEntity.GetOrigin();
		
		for (int i; i < 3; i++)
		{
			GetGame().GetCallqueue().CallLater(SpawnSiren, i * m_randomGenerator.RandInt(1000, 3000));
		}
		GetGame().GetCallqueue().CallLater(SpawnArtilleryBarrage, 25000);
	};
	
	void SpawnSiren()
	{
		if (m_targetEntity)
		{
			m_targetPos = m_targetEntity.GetOrigin();
		};
		
		vector pos = m_randomGenerator.GenerateRandomPointInRadius(1000, 1500, m_targetPos);
		float y = SCR_TerrainHelper.GetTerrainY(pos);
		pos[1] = y;
		IEntity siren = EM_Utils.SpawnEntity(m_SirenResName, pos);
		GetGame().GetCallqueue().CallLater(SCR_EntityHelper.DeleteEntityAndChildren, 73100, false, siren);
	};
	
	void SpawnArtilleryBarrage()
	{
		if (m_targetEntity)
		{
			m_targetPos = m_targetEntity.GetOrigin();
		};
		/*
		Bacon_FakeArtillery arty = Bacon_FakeArtillery.Cast(EM_Utils.SpawnEntity(m_ArtyResName, m_targetPos));
		arty.EditorAttributeSetInteger("m_numberOfProjectiles", 18);
		arty.EditorAttributeSetInteger("m_rofRPM", 30);
		arty.EditorAttributeSetInteger("m_areaRadius", 200);
		arty.EditorAttributeSetActive(true);
		*/
	};
};
