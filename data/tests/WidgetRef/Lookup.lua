root=
{
	class="RootWidget",
	id="root",
	children=
	{
		{
			id="label1",
			class="Label",
			text="Hello",
			constraints=
			{
				{
					type="growth",
					parent="window1",
					attribute="Attribute::WIDTH",
					relation="Relation::EQ",
					ratio=1.0,
					child="label1",
				}
			}
		}
	}
}
