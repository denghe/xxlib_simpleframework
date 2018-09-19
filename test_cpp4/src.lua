require "PKG_class"

local bb = BBuffer.Create()
local foo = PKG_Foo.Create()
foo.refFoo.Reset( foo )
foo.refFoos = List_Ref_PKG_Foo_.Create()
foo.refFoos[1] = MakeRef( foo )
bb:WriteRoot( foo )
print( bb )
bb:Clear();
foo.Release();
bb:WriteRoot( foo )
print( bb )
