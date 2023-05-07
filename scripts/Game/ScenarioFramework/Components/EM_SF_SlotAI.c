[EntityEditorProps(category: "GameScripted/ScriptWizard", description: "ScriptWizard generated script file.")]
class EM_SF_SlotAIClass : SCR_ScenarioFrameworkSlotAIClass
{
}

class EM_SF_SlotAI : SCR_ScenarioFrameworkSlotAI
{
	[Attribute("Column", UIWidgets.EditBox, "Name of the group formation", "")]
	protected string m_sFormationName;

	override protected void SetWPGroup()
	{
		super.SetWPGroup();
		
		AIFormationComponent formationHandler = AIFormationComponent.Cast(m_AIGroup.FindComponent(AIFormationComponent));
		
		if (!formationHandler)
			return;
		
		formationHandler.SetFormation(m_sFormationName);
	};
};
