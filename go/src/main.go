package main

import (
	"fmt"
	"unsafe"
)

type NullableUInt8 struct {
	Value uint8
	HasValue bool
}
type NullableUInt16 struct {
	Value uint16
	HasValue bool
}
type NullableUInt32 struct {
	Value uint32
	HasValue bool
}
type NullableUInt64 struct {
	Value uint64
	HasValue bool
}
type NullableInt8 struct {
	Value int8
	HasValue bool
}
type NullableInt16 struct {
	Value int16
	HasValue bool
}
type NullableInt32 struct {
	Value int32
	HasValue bool
}
type NullableInt64 struct {
	Value int64
	HasValue bool
}
type NullableBool struct {
	Value bool
	HasValue bool
}
type NullableFloat32 struct {
	Value float32
	HasValue bool
}
type NullableFloat64 struct {
	Value float64
	HasValue bool
}



type BBuffer struct {
	Buf []uint8
}
func (bb *BBuffer) DataLen() int {
	return len(bb.Buf)
}
func (bb *BBuffer) Clear() {
	bb.Buf = bb.Buf[:0]
}

// for all fix uint
func (bb *BBuffer) WriteVarUInt64(v uint64) {
	for v >= 1<<7 {
		bb.Buf = append(bb.Buf, uint8(v & 0x7f | 0x80))
		v >>= 7
	}
	bb.Buf = append(bb.Buf, uint8(v))
}

// for all fix int
func (bb *BBuffer) WriteVarInt64(v int64) {
	bb.WriteVarUInt64( uint64(uint64(v << 1) ^ uint64(v >> 63)) )
}

// for all >=0 int32/64
func (bb *BBuffer) WriteVarLen(v int) {
	bb.WriteVarUInt64( uint64(v) )
}



func (bb *BBuffer) WriteFloat32(v float32) {
	f := *(*uint32)(unsafe.Pointer(&v))
	bb.Buf = append(bb.Buf,
		uint8(f),
		uint8(f >> 8),
		uint8(f >> 16),
		uint8(f >> 24))
}

func (bb *BBuffer) WriteFloat64(v float64) {
	d := *(*uint64)(unsafe.Pointer(&v))
	bb.Buf = append(bb.Buf,
		uint8(d),
		uint8(d >> 8),
		uint8(d >> 16),
		uint8(d >> 24),
		uint8(d >> 32),
		uint8(d >> 40),
		uint8(d >> 48),
		uint8(d >> 56))
}
func (bb *BBuffer) WriteBool(v bool) {
	if v {
		bb.Buf = append(bb.Buf, uint8(1))
	} else {
		bb.Buf = append(bb.Buf, uint8(0))
	}
}

func (bb *BBuffer) WriteInt8(v int8) {
	bb.Buf = append(bb.Buf, uint8(v))
}
func (bb *BBuffer) WriteInt16(v int16) {
	bb.WriteVarInt64(int64(v))
}
func (bb *BBuffer) WriteInt32(v int32) {
	bb.WriteVarInt64(int64(v))
}
func (bb *BBuffer) WriteInt64(v int64) {
	bb.WriteVarInt64(v)
}

func (bb *BBuffer) WriteUInt8(v uint8) {
	bb.Buf = append(bb.Buf, v)
}
func (bb *BBuffer) WriteUInt16(v uint16) {
	bb.WriteVarUInt64(uint64(v))
}
func (bb *BBuffer) WriteUInt32(v uint32) {
	bb.WriteVarUInt64(uint64(v))
}
func (bb *BBuffer) WriteUInt64(v uint64) {
	bb.WriteVarUInt64(v)
}


func (bb *BBuffer) WriteNullableBool(v NullableBool) {
	if v.HasValue {
		bb.Buf = append(bb.Buf, uint8(1))
		bb.WriteBool(v.Value)
	} else {
		bb.Buf = append(bb.Buf, uint8(0))
	}
}
func (bb *BBuffer) WriteNullableFloat32(v NullableFloat32) {
	if v.HasValue {
		bb.Buf = append(bb.Buf, uint8(1))
		bb.WriteFloat32(v.Value)
	} else {
		bb.Buf = append(bb.Buf, uint8(0))
	}
}
func (bb *BBuffer) WriteNullableFloat64(v NullableFloat64) {
	if v.HasValue {
		bb.Buf = append(bb.Buf, uint8(1))
		bb.WriteFloat64(v.Value)
	} else {
		bb.Buf = append(bb.Buf, uint8(0))
	}
}
func (bb *BBuffer) WriteNullableString(v *string) {
	if v != nil {
		bb.Buf = append(bb.Buf, uint8(1))
		bb.WriteVarLen(len(*v))
		bb.Buf = append(bb.Buf, *v...)
	} else {
		bb.Buf = append(bb.Buf, uint8(0))
	}
}
func (bb *BBuffer) WriteNullableBBuffer(v *BBuffer) {
	if v != nil {
		bb.Buf = append(bb.Buf, uint8(1))
		bb.WriteVarLen(len(v.Buf))
		bb.Buf = append(bb.Buf, v.Buf...)
	} else {
		bb.Buf = append(bb.Buf, uint8(0))
	}
}

func (bb *BBuffer) WriteNullableUInt8(v NullableUInt8) {
	if v.HasValue {
		bb.Buf = append(bb.Buf, uint8(1), v.Value)
	} else {
		bb.Buf = append(bb.Buf, uint8(0))
	}
}
func (bb *BBuffer) WriteNullableUInt16(v NullableUInt16) {
	if v.HasValue {
		bb.Buf = append(bb.Buf, uint8(1))
		bb.WriteVarUInt64(uint64(v.Value))
	} else {
		bb.Buf = append(bb.Buf, uint8(0))
	}
}
func (bb *BBuffer) WriteNullableUInt32(v NullableUInt32) {
	if v.HasValue {
		bb.Buf = append(bb.Buf, uint8(1))
		bb.WriteVarUInt64(uint64(v.Value))
	} else {
		bb.Buf = append(bb.Buf, uint8(0))
	}
}
func (bb *BBuffer) WriteNullableUInt64(v NullableUInt64) {
	if v.HasValue {
		bb.Buf = append(bb.Buf, uint8(1))
		bb.WriteVarUInt64(v.Value)
	} else {
		bb.Buf = append(bb.Buf, uint8(0))
	}
}

func (bb *BBuffer) WriteNullableInt8(v NullableInt8) {
	if v.HasValue {
		bb.Buf = append(bb.Buf, uint8(1), uint8(v.Value))
	} else {
		bb.Buf = append(bb.Buf, uint8(0))
	}
}
func (bb *BBuffer) WriteNullableInt16(v NullableInt16) {
	if v.HasValue {
		bb.Buf = append(bb.Buf, uint8(1))
		bb.WriteVarInt64(int64(v.Value))
	} else {
		bb.Buf = append(bb.Buf, uint8(0))
	}
}
func (bb *BBuffer) WriteNullableInt32(v NullableInt32) {
	if v.HasValue {
		bb.Buf = append(bb.Buf, uint8(1))
		bb.WriteVarInt64(int64(v.Value))
	} else {
		bb.Buf = append(bb.Buf, uint8(0))
	}
}
func (bb *BBuffer) WriteNullableInt64(v NullableInt64) {
	if v.HasValue {
		bb.Buf = append(bb.Buf, uint8(1))
		bb.WriteVarInt64(v.Value)
	} else {
		bb.Buf = append(bb.Buf, uint8(0))
	}
}



type IWriteTo interface {
	WriteTo(bb *BBuffer)
}

// 模拟生成物
type PKG_Foo struct {
	Id int32
	Name *string
	Age NullableInt32
}
func (zs *PKG_Foo) WriteTo(bb *BBuffer) {
	if zs == nil {
		bb.Buf = append(bb.Buf, uint8(0))
		return
	}
	bb.Buf = append(bb.Buf, uint8(1))
	bb.WriteInt32(zs.Id)
	bb.WriteNullableString(zs.Name)
	bb.WriteNullableInt32(zs.Age)
}

type List_IWriteTo []IWriteTo
func (zs *List_IWriteTo) SwapRemoveAt(idx int) {
	count := len(*zs)
	if idx + 1 < count {
		(*zs)[idx] = (*zs)[count - 1]
	}
	*zs = (*zs)[:count - 1]
}
func (zs *List_IWriteTo) Add_PKG_Foo(v *PKG_Foo) {
	*zs = append(*zs, v)
}
func (zs *List_IWriteTo) Add_PKG_FooEx(v *PKG_FooEx) {
	*zs = append(*zs, v)
}
func (zs *List_IWriteTo) At_PKG_Foo(idx int) *PKG_Foo {
	return (*zs)[idx].(*PKG_Foo)
}
func (zs *List_IWriteTo) At_PKG_FooEx(idx int) *PKG_FooEx {
	return (*zs)[idx].(*PKG_FooEx)
}
func (zs *List_IWriteTo) WriteTo(bb *BBuffer) {
	if zs == nil {
		bb.Buf = append(bb.Buf, uint8(0))
		return
	}
	bb.Buf = append(bb.Buf, uint8(1))
	bb.WriteVarLen(len(*zs))
	for _, v := range *zs {
		v.WriteTo(bb)
	}
}

type PKG_Foos struct {
	Foos *List_IWriteTo
}
func (zs *PKG_Foos) WriteTo(bb *BBuffer) {
	if zs == nil {
		bb.Buf = append(bb.Buf, uint8(0))
		return
	}
	zs.Foos.WriteTo(bb)
}

type PKG_FooEx struct {
	PKG_Foo
	Weight int32
}
func (zs *PKG_FooEx) WriteTo(bb *BBuffer) {
	if zs == nil {
		bb.Buf = append(bb.Buf, uint8(0))
		return
	}
	zs.PKG_Foo.WriteTo(bb)
	bb.WriteInt32(zs.Weight)
}

func main() {
	bb := BBuffer{}
	name := "asdf"
	foo := PKG_Foo{ 10, &name, NullableInt32{20, true } }
	fooex := PKG_FooEx {foo, 30 }
	foos := PKG_Foos { &List_IWriteTo{} }
	foos.Foos.Add_PKG_FooEx( &fooex )
	foos.Foos.Add_PKG_Foo( nil )
	foos.Foos.Add_PKG_Foo( nil )
	foos.Foos.Add_PKG_Foo( nil )
	foos.Foos.Add_PKG_Foo( &foo )
	foos.WriteTo(&bb)
	fmt.Println(bb)
	bb.Clear()

	for _, f := range *foos.Foos {
		switch f.(type) {
		case *PKG_Foo:
			o := f.(*PKG_Foo)
			if o == nil {
				fmt.Println("PKG_Foo(nil)")
			} else {
				fmt.Println("PKG_Foo")
			}
		case *PKG_FooEx:
			o := f.(*PKG_FooEx)
			if o == nil {
				fmt.Println("PKG_FooEx(nil)")
			} else {
				fmt.Println("PKG_FooEx")
			}
		case nil:
			fmt.Println("nil")
		}
	}
}
