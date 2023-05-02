[EntityEditorProps(category: "GameScripted/ScriptWizard", description: "ScriptWizard generated script file.")]
class CP_EM_SlotAIClass : CP_SlotAIClass
{
}

class CP_EM_SlotAI : CP_SlotAI
{
	[Attribute("Column", UIWidgets.EditBox, "Name of the group formation", "")]
	protected string m_sFormationName;

	override protected void CreateAIGroup()
	{
		EntitySpawnParams paramsPatrol = new EntitySpawnParams();
 		paramsPatrol.TransformMode = ETransformMode.WORLD;
		paramsPatrol.Transform[3] = m_pEntity.GetOrigin();
		m_pAIGroup = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(Resource.Load(m_sGroupPrefab) , null, paramsPatrol));
		FactionAffiliationComponent facComp= FactionAffiliationComponent.Cast(m_pEntity.FindComponent(FactionAffiliationComponent));
		if (!facComp)
			return;
		m_pAIGroup.SetFaction(facComp.GetAffiliatedFaction());
		m_pAIGroup.AddAIEntityToGroup(m_pEntity, 0);
		
		AIFormationComponent formationHandler = AIFormationComponent.Cast(m_pAIGroup.FindComponent(AIFormationComponent));
		
		if (!formationHandler)
			return;
		
		formationHandler.SetFormation(m_sFormationName);
	};
};
