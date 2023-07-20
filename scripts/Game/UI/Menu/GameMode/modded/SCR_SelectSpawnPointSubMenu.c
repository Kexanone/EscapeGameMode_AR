/*
modded class SCR_SelectSpawnPointSubMenu : SCR_RespawnSubMenuBase
{
	protected Widget m_wTicketsCount;
	protected TextWidget m_wTickets;

	override void GetWidgets()
	{
		super.GetWidgets();
		m_wTicketsCount = GetRootWidget().FindAnyWidget("TicketsCount");
		if (m_wTicketsCount)
		{
			m_wTickets = TextWidget.Cast(m_wTicketsCount.FindAnyWidget("Tickets"));
		};
	};

	override void OnMenuUpdate(SCR_SuperMenuBase parentMenu, float tDelta)
	{
		super.OnMenuUpdate(parentMenu, tDelta);
		
		EM_SpawnTicketsComponent spawnTickets = EM_SpawnTicketsComponent.Cast(GetGame().GetGameMode().FindComponent(EM_SpawnTicketsComponent));
		
		if (spawnTickets)
		{
			m_wTicketsCount.SetVisible(true);
			m_wTickets.SetText(spawnTickets.GetTicketsCount().ToString());
		};
	};
};
*/