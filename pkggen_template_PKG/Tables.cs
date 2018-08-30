#pragma warning disable 0169, 0414
using TemplateLibrary;

class Foo
{
    [Column]
    int id;

    [Column]
    int? age;

    [Column]
    string info;

    List<Foo> childs;
}

class FooEx : Foo
{
}


class DataSet
{
    List<Table> tables;
}

class Table
{
    DataSet parent;
    string name;
    List<TableColumn> columns;
    List<TableRow> rows;
}

class TableColumn
{
    string name;
}

class TableRow
{
    List<TableRowValue> values;
}

class TableRowValue { }
class TableRowValue_Int : TableRowValue { int value; }
class TableRowValue_NullableInt : TableRowValue { int? value; }
class TableRowValue_String : TableRowValue { string value; }
