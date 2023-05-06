class EM_MissionHeaderEscape : SCR_MissionHeader
{
	[Attribute("-1", UIWidgets.EditBox, "Number of respawn tickets (disabled when set to -1)", "")]
	int m_iRespawnTickets;
	
	[Attribute("30", UIWidgets.EditBox, "Number of respawn tickets (disabled when set to -1)", "")]
	float m_fRespawnTime;
};