#include "../pkg/PKG_class.h"
int main()
{
	PKG::AllTypesRegister();
	xx::MemPool mp_, *mp = &mp_;

	PKG::DataSet_p ds;
	ds.MPCreate(mp);
	ds->tables.MPCreate(mp);

	PKG::Table_p t;
	t.MPCreate(mp);
	t->parent = ds;
	t->columns.MPCreate(mp);
	t->rows.MPCreate(mp);
	t->name.MPCreate(mp, "table_1");
	ds->tables->Add(t);

	PKG::TableColumn_p tc;
	tc.MPCreate(mp);
	tc->name.MPCreate(mp, "column_1");
	t->columns->Add(tc);

	PKG::TableRow_p tr;
	tr.MPCreate(mp);
	tr->values.MPCreate(mp);
	t->rows->Add(tr);

	PKG::TableRowValue_NullableInt_p trv1;
	trv1.MPCreate(mp);
	trv1->value = 123;
	tr->values->Add(trv1);
	PKG::TableRowValue_NullableInt_p trv2;
	trv1.MPCreate(mp);
	trv1->value.reset();	// null
	tr->values->Add(trv1);

	std::cout << ds << std::endl;

	xx::BBuffer_p bb;
	bb.MPCreate(mp);
	bb->WriteRoot(ds);

	std::cout << bb << std::endl;

	return 0;
}
