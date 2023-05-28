[EntityEditorProps(category: "GameScripted/ScriptWizard", description: "ScriptWizard generated script file.")]
class EM_SF_SlotDynamicEntityClass : SCR_ScenarioFrameworkSlotBaseClass
{
};

class EM_SF_SlotDynamicEntity : SCR_ScenarioFrameworkSlotBase
{
	override IEntity SpawnAsset()
	{
		IEntity entity = super.SpawnAsset();
		EM_DynamicEntityManagerComponent manager = EM_DynamicEntityManagerComponent.Cast(GetGame().GetGameMode().FindComponent(EM_DynamicEntityManagerComponent));
		
		if (manager)
			manager.AddEntity(entity);
		
		return entity;
	};
};
