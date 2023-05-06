[ComponentEditorProps(category: "GameScripted/GameMode/Components", description: "Handles respawn timers for players.")]
class EM_RespawnTimerComponentClass: SCR_RespawnTimerComponentClass
{
};

class EM_RespawnTimerComponent : SCR_RespawnTimerComponent
{
	[Attribute("1", desc: "Scenario header setting will overwrite these values.", category: "Respawn Timers")];
	protected bool m_bAllowHeaderSettings;

	override void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		
		if (!m_bAllowHeaderSettings)
			return;
		
		EM_MissionHeaderEscape header = EM_MissionHeaderEscape.Cast(GetGame().GetMissionHeader());
		
		if (!header)
			return;
		
		m_fRespawnTime = header.m_fRespawnTime;
	};
};
