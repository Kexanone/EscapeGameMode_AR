[EntityEditorProps(category: "GameScripted/ScriptWizard", description: "ScriptWizard generated script file.")]
class CP_EM_SlotAIPatrolOrStalkClass : CP_SlotBaseClass
{
};

class CP_EM_SlotAIPatrolOrStalk : CP_SlotBase
{
	[Attribute("0.45", UIWidgets.EditBox, "Probability for group to stalk instead of patrol", "")]
	protected float m_fStalkProbability;
	
	[Attribute("150", UIWidgets.EditBox, "Radius in meters within the patrol waypoints are generated", category: "Patrol")]
	protected float m_fRadius;
		
	[Attribute("50", UIWidgets.EditBox, "Minimum distance in meters between two consecutive waypoints", category: "Patrol")]
	protected float m_fMinDistance;
	
	[Attribute("3", UIWidgets.EditBox, "Number of waypoints", category: "Patrol")]
	protected float m_iNumWaypoints;
	
	override void Init(CP_Area pArea = null, CP_EActivationType EActivation = CP_EActivationType.SAME_AS_PARENT, bool bInit = true)
	{
		
		if (EActivation == CP_EActivationType.SAME_AS_PARENT)
		{
			ScriptInvoker pInvoker;
			if (pArea)
				pInvoker = pArea.GetOnAreaInit();
			
			if (pInvoker)
				pInvoker.Insert(SetTask);
		}
				
		if (m_EActivationType != EActivation)
			return;
		
		SCR_AIGroup.IgnoreSpawning(true);
		super.Init(pArea, EActivation);
		if (EActivation != CP_EActivationType.SAME_AS_PARENT)
			SetTask();
	}
	
	protected void SetTask()
	{
		if (!m_pEntity)
		{
			Print("CP: Trying to add waypoints to non existing entity! Did you select the object to spawn?");
			SCR_AIGroup.IgnoreSpawning(false);
			return;
		};
		
		SCR_AIGroup group = SCR_AIGroup.Cast(m_pEntity);
		group.SpawnUnits();
		
		Math.Randomize(-1);
		if (Math.RandomFloat(0, 1) >= m_fStalkProbability)
		{
			EM_AITasks.Patrol(group, GetOwner().GetOrigin(), m_fRadius, m_iNumWaypoints, m_fMinDistance);
		}
		else
		{
			EM_AITasks.Stalk(group);
		};
	};
};