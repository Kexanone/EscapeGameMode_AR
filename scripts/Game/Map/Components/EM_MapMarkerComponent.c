class EM_MapMarkerComponentClass : ScriptComponentClass
{
};

class EM_MapMarkerComponent : ScriptComponent
{
	[Attribute("", UIWidgets.EditBox, "Quad name of the icon", "")]
	protected string m_sImageQuadName;
	[Attribute("0,0,0,0", UIWidgets.ColorPicker, desc: "Color of the marker")]
	protected ref Color m_color;
	[Attribute("1", UIWidgets.EditBox, "Size of the marker", "")]
	protected float m_fSize;
	[Attribute("", UIWidgets.EditBox, "Marker text", "")]
	protected string m_sMarkerText;
		
	protected SCR_MapEntity m_MapEntity;
	protected ref MapItem m_MapMarker;
	
	override void OnPostInit(IEntity owner)
	{
		if (!GetGame().InPlayMode())
			return;
		
		SetEventMask(owner, EntityEvent.INIT);
		owner.SetFlags(EntityFlags.ACTIVE, true);

		super.OnPostInit(owner);
	};
	
	override void EOnInit(IEntity owner)
	{
		m_MapEntity = SCR_MapEntity.GetMapInstance();
		if (!m_MapEntity)
			return;
		
		m_MapMarker = m_MapEntity.CreateCustomMapItem();
		vector transform[4];
		owner.GetTransform(transform);
		m_MapMarker.SetPos(transform[3][0], transform[3][2]);
		m_MapMarker.SetImageDef(m_sImageQuadName);
		m_MapMarker.SetBaseType(0);
		MapDescriptorProps props = m_MapMarker.GetProps();
		props.SetFrontColor(m_color);
		props.SetOutlineColor(m_color);
		props.SetIconSize(m_fSize, m_fSize, m_fSize);
		props.Activate(true);
		m_MapMarker.SetProps(props);

		super.EOnInit(owner);
	};
};
