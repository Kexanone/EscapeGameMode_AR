modded class SCR_DeployMenuMain : SCR_DeployMenuBase
{
	protected Widget m_EM_wTicketsBox;
	protected TextWidget m_EM_wTicketsLabel;
	protected TextWidget m_EM_wTicketsCount;
	
	override void OnMenuOpen()
	{
		super.OnMenuOpen();
		m_EM_wTicketsBox = GetRootWidget().FindAnyWidget("EM_TicketsBox");
		m_EM_wTicketsLabel = TextWidget.Cast(GetRootWidget().FindAnyWidget("EM_TicketsLabel"));
		m_EM_wTicketsCount = TextWidget.Cast(GetRootWidget().FindAnyWidget("EM_TicketsCount"));
	};

	override void OnMenuUpdate(float tDelta)
	{
		super.OnMenuUpdate(tDelta);
		
		EM_SpawnTicketsComponent spawnTickets = EM_SpawnTicketsComponent.Cast(GetGame().GetGameMode().FindComponent(EM_SpawnTicketsComponent));
		
		if (spawnTickets)
		{
			int count = spawnTickets.GetTicketsCount();
			
			if (count <= 0)
			{
				m_EM_wTicketsLabel.SetColor(Color.Red);
				m_EM_wTicketsCount.SetColor(Color.Red);
			};
			
			m_EM_wTicketsBox.SetVisible(true);
			m_EM_wTicketsCount.SetText(count.ToString());
		};
	};
};