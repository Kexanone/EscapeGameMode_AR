class EM_AIMakeStalker : AITaskScripted
{
	override ENodeResult EOnTaskSimulate(AIAgent owner, float dt)
	{
		SCR_AIGroup group = SCR_AIGroup.Cast(owner);
		EM_StalkerComponent stalkerManager = EM_StalkerComponent.Cast(GetGame().GetGameMode().FindComponent(EM_StalkerComponent));
		
		if (!stalkerManager)
			return ENodeResult.FAIL;
		
		stalkerManager.AddStalker(group);
		return ENodeResult.SUCCESS;
	}

	protected override bool VisibleInPalette()
	{
		return true;
	}	
	
	protected override string GetOnHoverDescription()
	{
		return "Randomly returns a player to be stalked";
	}
};
