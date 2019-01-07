#pragma warning disable 0169, 0414
using TemplateLibrary;



class Account
{
    int id;
    string name;
    int? vipLevel;
}

class Player : Account
{
    Scene owner;
    // ...
}

class Scene
{
    List<Ref<Player>> players;
}








//class Container
//{
//    List<Foo> foos;
//}

//class Foo
//{
//    [Ref(false, true)]
//    List<Foo> childs;
//}

//class FooEx : Foo
//{
//    [Ref]
//    Foo parent;
//}









//class Foo
//{
//    [Column]
//    int id;

//    [Column]
//    int? age;

//    [Column]
//    string info;

//    List<Foo> childs;

//    object o;

//    List<object> os;
//}

//class FooEx : Foo
//{
//    [Ref]
//    Foo foo;
//}



//[MySql]
//partial interface MySqlFuncs
//{
//    [Desc("模拟从表查数据填充到 Foo. 顺序和数量一致应该与 Foo 中标有 Column 的类成员一致")]
//    [Sql(@"select {0}, {1}, {2}")]
//    Foo SelectFoo(int id, int? age, string info);


//    [Sql(@"
//select {0}, {1}, {2};
//select {0};
//select {0}, {1}, {2};
//select {1};
//")]
//    Tuple<Foo, int, List<Foo>, int?> SelectFooFoo(int id, int? age, string info);

//}



//namespace Tables
//{
//    class node
//    {
//        [Column]
//        int id;

//        [Column]
//        int? pid;
//    }
//}

//class Node : Tables.node
//{
//    Node parent;
//    List<Node> childs;
//}

//partial interface MySqlFuncs
//{
//    [Sql(@"
//select 1, null
//union all
//select 2, 1
//union all
//select 3, 1
//union all
//select 4, 2;
//")]
//    List<Tables.node> SelectNodes();
//}





//class DataSet
//{
//    List<Table> tables;
//}

//class Table
//{
//    DataSet parent;
//    string name;
//    List<TableColumn> columns;
//    List<TableRow> rows;
//}

//class TableColumn
//{
//    string name;
//}

//class TableRow
//{
//    List<TableRowValue> values;
//}

//class TableRowValue { }
//class TableRowValue_Int : TableRowValue { int value; }
//class TableRowValue_NullableInt : TableRowValue { int? value; }
//class TableRowValue_String : TableRowValue { string value; }
