[EntityEditorProps(category: "GameScripted/ScriptWizard", description: "ScriptWizard generated script file.")]
class CP_EM_SlotAIClass : SCR_ScenarioFrameworkSlotAIClass
{
}

class CP_EM_SlotAI : SCR_ScenarioFrameworkSlotAI
{
	[Attribute("Column", UIWidgets.EditBox, "Name of the group formation", "")]
	protected string m_sFormationName;

	override protected void CreateAIGroup()
	{
		EntitySpawnParams paramsPatrol = new EntitySpawnParams();
 		paramsPatrol.TransformMode = ETransformMode.WORLD;
		paramsPatrol.Transform[3] = m_Entity.GetOrigin();
		m_AIGroup = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(Resource.Load(m_sGroupPrefab) , null, paramsPatrol));
		FactionAffiliationComponent facComp= FactionAffiliationComponent.Cast(m_Entity.FindComponent(FactionAffiliationComponent));
		if (!facComp)
			return;
		m_AIGroup.SetFaction(facComp.GetAffiliatedFaction());
		m_AIGroup.AddAIEntityToGroup(m_Entity, 0);
		
		AIFormationComponent formationHandler = AIFormationComponent.Cast(m_AIGroup.FindComponent(AIFormationComponent));
		
		if (!formationHandler)
			return;
		
		formationHandler.SetFormation(m_sFormationName);
	};
};
