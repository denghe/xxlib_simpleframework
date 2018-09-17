#pragma warning disable 0169, 0414
using TemplateLibrary;

[TypeIdMappings]
interface ITypeIdMappings
{
    Foo _3 { get; }

    List<Foo> _4 { get; }

    List<Ref<Foo>> _5 { get; }

}
