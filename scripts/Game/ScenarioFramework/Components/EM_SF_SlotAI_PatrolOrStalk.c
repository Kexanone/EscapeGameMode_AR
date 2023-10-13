[EntityEditorProps(category: "GameScripted/ScriptWizard", description: "ScriptWizard generated script file.")]
class EM_SF_SlotAIPatrolOrStalkClass : SCR_ScenarioFrameworkSlotBaseClass
{
};

class EM_SF_SlotAIPatrolOrStalk : SCR_ScenarioFrameworkSlotBase
{
	[Attribute("0.45", UIWidgets.EditBox, "Probability for group to stalk instead of patrol", "")]
	protected float m_fStalkProbability;
	
	[Attribute("150", UIWidgets.EditBox, "Radius in meters within the patrol waypoints are generated", category: "Patrol")]
	protected float m_fRadius;
		
	[Attribute("50", UIWidgets.EditBox, "Minimum distance in meters between two consecutive waypoints", category: "Patrol")]
	protected float m_fMinDistance;
	
	[Attribute("3", UIWidgets.EditBox, "Number of waypoints", category: "Patrol")]
	protected float m_iNumWaypoints;
	
	override void Init(SCR_ScenarioFrameworkArea area = null, SCR_ScenarioFrameworkEActivationType activation = SCR_ScenarioFrameworkEActivationType.SAME_AS_PARENT)
	{
		if (activation == SCR_ScenarioFrameworkEActivationType.SAME_AS_PARENT)
		{
			ScriptInvoker invoker;
			if (area)
				invoker = area.GetOnAreaInit();

			if (invoker)
				invoker.Insert(SetTask);
		}

		if (m_eActivationType != activation)
			return;

		SCR_AIGroup.IgnoreSpawning(true);
		super.Init(area, activation);
		if (activation != SCR_ScenarioFrameworkEActivationType.SAME_AS_PARENT)
			SetTask();
	}
	
	protected void SetTask()
	{
		if (!m_Entity)
		{
			Print("CP: Trying to add waypoints to non existing entity! Did you select the object to spawn?");
			SCR_AIGroup.IgnoreSpawning(false);
			return;
		};
		
		SCR_AIGroup group = SCR_AIGroup.Cast(m_Entity);
		group.SpawnUnits();
		
		EM_DynamicEntityManagerComponent manager = EM_DynamicEntityManagerComponent.Cast(GetGame().GetGameMode().FindComponent(EM_DynamicEntityManagerComponent));
		
		Math.Randomize(-1);
		if (Math.RandomFloat(0, 1) >= m_fStalkProbability)
		{
			EM_AITasks.Patrol(group, GetOwner().GetOrigin(), m_fRadius, m_iNumWaypoints, m_fMinDistance);
			
			if (manager)
				manager.AddEntity(group);
		}
		else
		{
			EM_AITasks.Stalk(group);
			
			if (manager)
				manager.AddEntity(EM_DynamicStalkerWrapper(group));
		};
	};
};
