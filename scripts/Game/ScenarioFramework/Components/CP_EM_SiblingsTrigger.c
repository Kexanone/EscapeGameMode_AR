/*
	This slot trigger automatically triggers all its siblings in the hierarchy when activated
*/
[EntityEditorProps(category: "GameScripted/ScriptWizard", description: "ScriptWizard generated script file.")]
class CP_EM_SiblingsTriggerClass : CP_SlotBaseClass
{
}

class CP_EM_SiblingsTrigger : CP_SlotBase
{
	override void Init(CP_Area pArea = null, CP_EActivationType EActivation = CP_EActivationType.SAME_AS_PARENT, bool bInit = true)
	{
		super.Init(pArea, EActivation, bInit);
		
		if (!SCR_BaseTriggerEntity.Cast(m_pEntity))
			return;
		
		ScriptInvoker pOnActivateInvoker = SCR_BaseTriggerEntity.Cast(m_pEntity).GetOnActivate();
		if (pOnActivateInvoker)
			pOnActivateInvoker.Insert(OnActivate);
		
		ScriptInvoker pOnDeactivateInvoker = SCR_BaseTriggerEntity.Cast(m_pEntity).GetOnDeactivate();
		if (pOnDeactivateInvoker)
			pOnDeactivateInvoker.Insert(OnActivate);		//registering OnDeactivate to OnActivate - we need both changes 
	};
	
	void OnActivate(IEntity pObject)
	{
		IEntity cpArea = GetOwner().GetParent();
		IEntity child = cpArea.GetChildren();
		while (child)
		{
			CP_LayerBase pLayer = CP_LayerBase.Cast(child.FindComponent(CP_LayerBase));
			
			if (pLayer && child != GetOwner())
			{
				pLayer.Init(null, CP_EActivationType.ON_TRIGGER_ACTIVATION, false);
			};

			child = child.GetSibling();
		};
	};
};
