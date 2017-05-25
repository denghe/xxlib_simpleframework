#pragma warning disable 0169, 0414
using TemplateLibrary;

public enum E : byte
{
    A, B, C
}
class Bar
{
}

class Bar1 : Bar
{
    int f6;
}

class Bar2 : Bar
{
    string f10;
}

class FooBase
{
}

class Foo : FooBase
{
    byte f1;
    sbyte f2;
    ushort f3;
    short f4;
    uint f5;
    int f6;
    ulong f7;
    long f8;
    //DateTime f9;
    string f10;
    BBuffer f11;
    E f12;
    List<E> f13;
    List<Bar> f14;
    List<List<E>> f15;
    List<List<Bar>> f16;
    List<List<List<E>>> f17;
    List<List<List<Bar>>> f18;
    FooBase f19;
}
