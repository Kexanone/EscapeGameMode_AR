class Bacon_DamagingTriggerEntityClass: SCR_BaseTriggerEntityClass {
};

class Bacon_DamagingTriggerEntity: SCR_BaseTriggerEntity {
	[Attribute("1.0", desc: "Damage value base")]
	protected float m_damageValue;
	
	[Attribute("0", desc: "Damage value randomization")]
	protected float m_damageRandomize;
	
	[Attribute("0", UIWidgets.ComboBox, "Damage type", "", ParamEnumArray.FromEnum(EDamageType) )]
	private EDamageType m_damageType;
	
	protected override bool ScriptedEntityFilterForQuery(IEntity ent) {
		// only entities with damage controller 
		DamageManagerComponent damageManager = DamageManagerComponent.Cast(ent.FindComponent(DamageManagerComponent));
		if (damageManager)
			return true;

		return false;
	};
	
	override void OnActivate(IEntity ent) {
		float m_damage = m_damageValue;
		
		if (m_damageRandomize > 0) {
			m_damage = m_damageValue + Math.RandomFloat(-m_damageRandomize, m_damageRandomize);
		};
		
		DamageManagerComponent dmc = DamageManagerComponent.Cast(ent.FindComponent(DamageManagerComponent));
		if (!dmc)
			return;
		
		vector hitPosDirNorm[3];
		HitZone hitZone = dmc.GetDefaultHitZone();
		dmc.HandleDamage(m_damageType, m_damage, hitPosDirNorm, ent, hitZone, ent, null, -1, -1);
	};
};