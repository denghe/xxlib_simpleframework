package main

import (
	"fmt"
	"reflect"
	"unsafe"
)


/**********************************************************************************************************************/
// Nullables
/**********************************************************************************************************************/


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
type NullableString struct {
	Value string
	HasValue bool
}



/**********************************************************************************************************************/
// IObject
/**********************************************************************************************************************/


type IObject interface {
	GetPackageId() uint16
	ToBBuffer(bb *BBuffer)
	FromBBuffer(bb *BBuffer)
}



/**********************************************************************************************************************/
// typeIdCreatorMappings
/**********************************************************************************************************************/



var typeIdCreatorMappings = map[uint16] func() IObject {}

func RegisterInternals() {
	typeIdCreatorMappings[2] = func() IObject {
		return &BBuffer{}
	}
}

func Register(typeId uint16, maker func() IObject) {
	typeIdCreatorMappings[typeId] = maker
}

func CreateByTypeId(typeId uint16) IObject {
	maker, found := typeIdCreatorMappings[typeId]
	if found {
		return maker()
	}
	panic(-3)
}



/**********************************************************************************************************************/
// BBuffer
/**********************************************************************************************************************/



type BBuffer struct {
	Buf []uint8
	Offset,
	ReadLengthLimit,
	DataLenBak,
	OffsetRoot int
	ptrStore map[IObject] int
	idxStore map[int] IObject
}


func (zs *BBuffer) DataLen() int {
	return len(zs.Buf)
}
func (zs *BBuffer) Clear() {
	zs.Buf = zs.Buf[:0]
	zs.Offset = 0
	zs.ReadLengthLimit = 0
	// todo
}

func (zs *BBuffer) GetPackageId() uint16 {
	return 2
}
func (zs *BBuffer) ToBBuffer(bb *BBuffer) {
	if zs == nil {
		bb.Buf = append(bb.Buf, uint8(0))
		return
	}
	bb.Buf = append(bb.Buf, uint8(2))		// typeId
	bb.WriteLength(len(zs.Buf))
	bb.Buf = append(bb.Buf, zs.Buf...)
}
func (zs *BBuffer) FromBBuffer(bb *BBuffer) {
	bufLen := bb.ReadLength()
	if bb.ReadLengthLimit != 0 && bufLen > bb.ReadLengthLimit {
		panic(-1)
	}
	zs.Clear()
	zs.Buf = append(zs.Buf, bb.Buf[bb.Offset:bb.Offset + bufLen]...)
	bb.Offset += bufLen
}

// for all fix uint
func (zs *BBuffer) WriteVarUInt64(v uint64) {
	for v >= 1<<7 {
		zs.Buf = append(zs.Buf, uint8(v & 0x7f | 0x80))
		v >>= 7
	}
	zs.Buf = append(zs.Buf, uint8(v))
}
func (zs *BBuffer) ReadVarUInt64() (r uint64) {
	offset := zs.Offset
	for shift := uint(0); shift < 64; shift += 7 {
		b := uint64(zs.Buf[offset])
		offset++
		r |= (b & 0x7F) << shift
		if (b & 0x80) == 0 {
			zs.Offset = offset
			return r
		}
	}
	panic(-1)
}


// for all fix int
func (zs *BBuffer) WriteVarInt64(v int64) {
	zs.WriteVarUInt64( uint64(uint64(v << 1) ^ uint64(v >> 63)) )
}
func (zs *BBuffer) ReadVarInt64() int64 {
	v := zs.ReadVarUInt64()
	return (int64)(v >> 1) ^ (-(int64)(v & 1))
}

// for all len
func (zs *BBuffer) WriteLength(v int) {
	zs.WriteVarUInt64( uint64(v) )
}
func (zs *BBuffer) ReadLength() int {
	return int(zs.ReadVarUInt64())
}



func (zs *BBuffer) WriteFloat32(v float32) {
	f := *(*uint32)(unsafe.Pointer(&v))
	zs.Buf = append(zs.Buf,
		uint8(f),
		uint8(f >> 8),
		uint8(f >> 16),
		uint8(f >> 24))
}
func (zs *BBuffer) ReadFloat32() (r float32) {
	v := uint32(zs.Buf[zs.Offset]) |
		uint32(zs.Buf[zs.Offset + 1]) << 8 |
		uint32(zs.Buf[zs.Offset + 2]) << 16 |
		uint32(zs.Buf[zs.Offset + 3]) << 24
	r = *(*float32)(unsafe.Pointer(&v))
	zs.Offset += 4
	return
}

func (zs *BBuffer) WriteFloat64(v float64) {
	d := *(*uint64)(unsafe.Pointer(&v))
	zs.Buf = append(zs.Buf,
		uint8(d),
		uint8(d >> 8),
		uint8(d >> 16),
		uint8(d >> 24),
		uint8(d >> 32),
		uint8(d >> 40),
		uint8(d >> 48),
		uint8(d >> 56))
}
func (zs *BBuffer) ReadFloat64() (r float64) {
	v := uint32(zs.Buf[zs.Offset]) |
		uint32(zs.Buf[zs.Offset + 1]) << 8 |
		uint32(zs.Buf[zs.Offset + 2]) << 16 |
		uint32(zs.Buf[zs.Offset + 3]) << 24 |
		uint32(zs.Buf[zs.Offset + 4]) << 32 |
		uint32(zs.Buf[zs.Offset + 5]) << 40 |
		uint32(zs.Buf[zs.Offset + 6]) << 48 |
		uint32(zs.Buf[zs.Offset + 7]) << 56
	r = *(*float64)(unsafe.Pointer(&v))
	zs.Offset += 8
	return
}

func (zs *BBuffer) WriteBool(v bool) {
	if v {
		zs.Buf = append(zs.Buf, uint8(1))
	} else {
		zs.Buf = append(zs.Buf, uint8(0))
	}
}
func (zs *BBuffer) ReadBool() (r bool) {
	r = zs.Buf[zs.Offset] > 0
	zs.Offset++
	return
}

func (zs *BBuffer) WriteInt8(v int8) {
	zs.Buf = append(zs.Buf, uint8(v))
}
func (zs *BBuffer) ReadInt8() (r int8) {
	r = int8(zs.Buf[zs.Offset])
	zs.Offset++
	return
}

func (zs *BBuffer) WriteInt16(v int16) {
	zs.WriteVarInt64(int64(v))
}
func (zs *BBuffer) ReadInt16() int16 {
	return int16(zs.ReadVarInt64())
}

func (zs *BBuffer) WriteInt32(v int32) {
	zs.WriteVarInt64(int64(v))
}
func (zs *BBuffer) ReadInt32() int32 {
	return int32(zs.ReadVarInt64())
}

func (zs *BBuffer) WriteInt64(v int64) {
	zs.WriteVarInt64(v)
}
func (zs *BBuffer) ReadInt64() int64 {
	return zs.ReadVarInt64()
}

func (zs *BBuffer) WriteUInt8(v uint8) {
	zs.Buf = append(zs.Buf, v)
}
func (zs *BBuffer) ReadUInt8() (r uint8) {
	r = zs.Buf[zs.Offset]
	zs.Offset++
	return
}

func (zs *BBuffer) WriteUInt16(v uint16) {
	zs.WriteVarUInt64(uint64(v))
}
func (zs *BBuffer) ReadUInt16() uint16 {
	return uint16(zs.ReadVarUInt64())
}

func (zs *BBuffer) WriteUInt32(v uint32) {
	zs.WriteVarUInt64(uint64(v))
}
func (zs *BBuffer) ReadUInt32() uint32 {
	return uint32(zs.ReadVarUInt64())
}

func (zs *BBuffer) WriteUInt64(v uint64) {
	zs.WriteVarUInt64(v)
}
func (zs *BBuffer) ReadUInt64() uint64 {
	return zs.ReadVarUInt64()
}

func (zs *BBuffer) WriteNullableBool(v NullableBool) {
	if v.HasValue {
		zs.Buf = append(zs.Buf, uint8(1))
		zs.WriteBool(v.Value)
	} else {
		zs.Buf = append(zs.Buf, uint8(0))
	}
}
func (zs *BBuffer) ReadNullableBool() (r NullableBool) {
	r.HasValue = zs.ReadBool()
	if r.HasValue {
		r.Value = zs.ReadBool()
	}
	return
}
func (zs *BBuffer) WriteNullableFloat32(v NullableFloat32) {
	if v.HasValue {
		zs.Buf = append(zs.Buf, uint8(1))
		zs.WriteFloat32(v.Value)
	} else {
		zs.Buf = append(zs.Buf, uint8(0))
	}
}
func (zs *BBuffer) ReadNullableFloat32() (r NullableFloat32) {
	r.HasValue = zs.ReadBool()
	if r.HasValue {
		r.Value = zs.ReadFloat32()
	}
	return
}

func (zs *BBuffer) WriteNullableFloat64(v NullableFloat64) {
	if v.HasValue {
		zs.Buf = append(zs.Buf, uint8(1))
		zs.WriteFloat64(v.Value)
	} else {
		zs.Buf = append(zs.Buf, uint8(0))
	}
}
func (zs *BBuffer) ReadNullableFloat64() (r NullableFloat64) {
	r.HasValue = zs.ReadBool()
	if r.HasValue {
		r.Value = zs.ReadFloat64()
	}
	return
}

func (zs *BBuffer) WriteNullableString(v NullableString) {
	if v.HasValue {
		zs.WriteUInt16(1)	// typeId
		zs.WriteLength(len(v.Value))
		zs.Buf = append(zs.Buf, v.Value...)
	} else {
		zs.Buf = append(zs.Buf, uint8(0))
	}
}
func (zs *BBuffer) ReadNullableString() (r NullableString) {
	typeId := zs.ReadUInt16()
	if typeId == 0 {
		return NullableString{}
	}
	bufLen := zs.ReadLength()
	r.Value = string(zs.Buf[zs.Offset:zs.Offset + bufLen])
	zs.Offset += bufLen
	r.HasValue = true
	return
}

func (zs *BBuffer) WriteNullableUInt8(v NullableUInt8) {
	if v.HasValue {
		zs.Buf = append(zs.Buf, uint8(1), v.Value)
	} else {
		zs.Buf = append(zs.Buf, uint8(0))
	}
}
func (zs *BBuffer) ReadNullableUInt8() (r NullableUInt8) {
	r.HasValue = zs.ReadBool()
	if r.HasValue {
		r.Value = zs.ReadUInt8()
	}
	return
}

func (zs *BBuffer) WriteNullableUInt16(v NullableUInt16) {
	if v.HasValue {
		zs.Buf = append(zs.Buf, uint8(1))
		zs.WriteVarUInt64(uint64(v.Value))
	} else {
		zs.Buf = append(zs.Buf, uint8(0))
	}
}
func (zs *BBuffer) ReadNullableUInt16() (r NullableUInt16) {
	r.HasValue = zs.ReadBool()
	if r.HasValue {
		r.Value = zs.ReadUInt16()
	}
	return
}

func (zs *BBuffer) WriteNullableUInt32(v NullableUInt32) {
	if v.HasValue {
		zs.Buf = append(zs.Buf, uint8(1))
		zs.WriteVarUInt64(uint64(v.Value))
	} else {
		zs.Buf = append(zs.Buf, uint8(0))
	}
}
func (zs *BBuffer) ReadNullableUInt32() (r NullableUInt32) {
	r.HasValue = zs.ReadBool()
	if r.HasValue {
		r.Value = zs.ReadUInt32()
	}
	return
}

func (zs *BBuffer) WriteNullableUInt64(v NullableUInt64) {
	if v.HasValue {
		zs.Buf = append(zs.Buf, uint8(1))
		zs.WriteVarUInt64(v.Value)
	} else {
		zs.Buf = append(zs.Buf, uint8(0))
	}
}
func (zs *BBuffer) ReadNullableUInt64() (r NullableUInt64) {
	r.HasValue = zs.ReadBool()
	if r.HasValue {
		r.Value = zs.ReadUInt64()
	}
	return
}

func (zs *BBuffer) WriteNullableInt8(v NullableInt8) {
	if v.HasValue {
		zs.Buf = append(zs.Buf, uint8(1), uint8(v.Value))
	} else {
		zs.Buf = append(zs.Buf, uint8(0))
	}
}
func (zs *BBuffer) ReadNullableInt8() (r NullableInt8) {
	r.HasValue = zs.ReadBool()
	if r.HasValue {
		r.Value = zs.ReadInt8()
	}
	return
}

func (zs *BBuffer) WriteNullableInt16(v NullableInt16) {
	if v.HasValue {
		zs.Buf = append(zs.Buf, uint8(1))
		zs.WriteVarInt64(int64(v.Value))
	} else {
		zs.Buf = append(zs.Buf, uint8(0))
	}
}
func (zs *BBuffer) ReadNullableInt16() (r NullableInt16) {
	r.HasValue = zs.ReadBool()
	if r.HasValue {
		r.Value = zs.ReadInt16()
	}
	return
}

func (zs *BBuffer) WriteNullableInt32(v NullableInt32) {
	if v.HasValue {
		zs.Buf = append(zs.Buf, uint8(1))
		zs.WriteVarInt64(int64(v.Value))
	} else {
		zs.Buf = append(zs.Buf, uint8(0))
	}
}
func (zs *BBuffer) ReadNullableInt32() (r NullableInt32) {
	r.HasValue = zs.ReadBool()
	if r.HasValue {
		r.Value = zs.ReadInt32()
	}
	return
}

func (zs *BBuffer) WriteNullableInt64(v NullableInt64) {
	if v.HasValue {
		zs.Buf = append(zs.Buf, uint8(1))
		zs.WriteVarInt64(v.Value)
	} else {
		zs.Buf = append(zs.Buf, uint8(0))
	}
}
func (zs *BBuffer) ReadNullableInt64() (r NullableInt64) {
	r.HasValue = zs.ReadBool()
	if r.HasValue {
		r.Value = zs.ReadInt64()
	}
	return
}





func (zs *BBuffer) BeginWrite() {
	zs.ptrStore = make(map[IObject] int)
	zs.OffsetRoot = len(zs.Buf)
}
func (zs *BBuffer) BeginRead() {
	zs.idxStore = make(map[int] IObject)
	zs.OffsetRoot = zs.Offset
}

func (zs *BBuffer) WriteRoot(v IObject) {
	zs.BeginWrite()
	zs.WriteIObject(v)
}
func (zs *BBuffer) ReadRoot() IObject {
	zs.BeginRead()
	return zs.ReadIObject()
}
func (zs *BBuffer) WriteIObject(v IObject) {
	//if *(*unsafe.Pointer)(unsafe.Pointer(uintptr(unsafe.Pointer(&v)) + uintptr(unsafe.Sizeof(int(0))))) == nil {
	if v == nil || reflect.ValueOf(v).IsNil() {
		zs.Buf = append(zs.Buf, uint8(0))
		return
	}
	zs.WriteUInt16(v.GetPackageId())
	offset, found := zs.ptrStore[v]
	if !found {
		offset = len(zs.Buf) - zs.OffsetRoot
		zs.ptrStore[v] = offset
	}
	zs.WriteLength(offset)
	if !found {
		v.ToBBuffer(zs)
	}
}
func (zs *BBuffer) ReadIObject() (r IObject) {
	typeId := zs.ReadUInt16()
	if typeId == 0 {
		return nil
	}
	if typeId == 1 {
		panic(-4)	// need ReadNullableString
	}
	offset := zs.Offset - zs.OffsetRoot
	ptrOffset := zs.ReadLength()
	if ptrOffset == offset {
		r = CreateByTypeId(typeId)
		zs.idxStore[ptrOffset] = r
		r.FromBBuffer(zs)
	} else {
		var found bool
		r, found = zs.idxStore[ptrOffset]
		if !found {
			panic(-5)
		}
		if r.GetPackageId() != typeId {
			panic(-6)
		}
	}
	return
}
















































/**********************************************************************************************************************/
// tests
/**********************************************************************************************************************/


// 模拟生成物


type PKG_Foo struct {
	Id int32
	Name NullableString
	Age NullableInt32
	Parent PKG_Foo_Interface
}

// for base class convert simulate
type PKG_Foo_Interface interface {
	GetFoo() *PKG_Foo
}
func (zs *PKG_Foo) GetFoo() *PKG_Foo {
	return zs
}
func (zs *BBuffer) Write_PKG_Foo(v PKG_Foo_Interface) {
	if v == nil {
		zs.WriteIObject(nil)
	} else {
		zs.WriteIObject(v.(IObject))
	}
}
func (zs *BBuffer) Read_PKG_Foo() PKG_Foo_Interface {
	v := zs.ReadIObject()
	if v == nil {
		return nil
	} else {
		return v.(PKG_Foo_Interface)
	}
}
func (zs *BBuffer) TryReadRoot_PKG_Foo() (r *PKG_Foo, ok bool) {
	defer func() {
		//recover()
	}()
	v := zs.ReadRoot()
	if v != nil {
		r = v.(PKG_Foo_Interface).GetFoo()
	}
	ok = true
	return
}


// IObject implement
func (zs *PKG_Foo) GetPackageId() uint16 {
	return uint16(3)
}
func (zs *PKG_Foo) ToBBuffer(bb *BBuffer) {
	bb.WriteInt32(zs.Id)
	bb.WriteNullableString(zs.Name)
	bb.WriteNullableInt32(zs.Age)
	bb.Write_PKG_Foo(zs.Parent)
}
func (zs *PKG_Foo) FromBBuffer(bb *BBuffer) {
	zs.Id = bb.ReadInt32()
	zs.Name = bb.ReadNullableString()
	zs.Age = bb.ReadNullableInt32()
	zs.Parent = bb.Read_PKG_Foo()
}




type List_Int32 []int32

// for base class convert simulate
type List_Int32_Interface interface {
	GetList_Int32() *List_Int32
}
func (zs *List_Int32) GetList_Int32() *List_Int32 {
	return zs
}
func (zs *BBuffer) Write_List_Int32(v List_Int32_Interface) {
	if v == nil {
		zs.WriteIObject(nil)
	} else {
		zs.WriteIObject(v.(IObject))
	}
}
func (zs *BBuffer) Read_List_Int32() List_Int32_Interface {
	v := zs.ReadIObject()
	if v == nil {
		return nil
	} else {
		return v.(List_Int32_Interface)
	}
}
func (zs *BBuffer) TryReadRoot_List_Int32() (r *List_Int32, ok bool) {
	defer func() {
		//recover()
	}()
	v := zs.ReadRoot()
	if v != nil {
		r = v.(List_Int32_Interface).GetList_Int32()
	}
	ok = true
	return
}


// IObject implement
func (zs *List_Int32) GetPackageId() uint16 {
	return uint16(4)
}
func (zs *List_Int32) ToBBuffer(bb *BBuffer) {
	bb.WriteLength(len(*zs))
	for _, v := range *zs {
		bb.WriteInt32(v)
	}
}
func (zs *List_Int32) FromBBuffer(bb *BBuffer) {
	count := bb.ReadLength()
	if bb.ReadLengthLimit != 0 && count > bb.ReadLengthLimit {
		panic(-1)
	}
	*zs = (*zs)[:0]
	if count == 0 {
		return
	}
	for i := 0; i < count; i++ {
		*zs = append(*zs, bb.ReadInt32())
	}
}

// util func
func (zs *List_Int32) SwapRemoveAt(idx int) {
	count := len(*zs)
	if idx + 1 < count {
		(*zs)[idx] = (*zs)[count - 1]
	}
	*zs = (*zs)[:count - 1]
}






type PKG_FooExt1 struct {
	PKG_Foo
	ChildIds List_Int32_Interface
}

// for base class convert simulate
type PKG_FooExt1_Interface interface {
	GetFooExt1() *PKG_FooExt1
}
func (zs *PKG_FooExt1) GetFooExt1() *PKG_FooExt1 {
	return zs
}
func (zs *BBuffer) Write_PKG_FooExt1(v PKG_FooExt1_Interface) {
	if v == nil {
		zs.WriteIObject(nil)
	} else {
		zs.WriteIObject(v.(IObject))
	}
}
func (zs *BBuffer) Read_PKG_FooExt1() PKG_FooExt1_Interface {
	v := zs.ReadIObject()
	if v == nil {
		return nil
	} else {
		return v.(PKG_FooExt1_Interface)
	}
}
func (zs *BBuffer) TryReadRoot_PKG_FooExt1() (r *PKG_FooExt1, ok bool) {
	defer func() {
		//recover()
	}()
	v := zs.ReadRoot()
	if v != nil {
		r = v.(PKG_FooExt1_Interface).GetFooExt1()
	}
	ok = true
	return
}


// IObject implement
func (zs *PKG_FooExt1) GetPackageId() uint16 {
	return uint16(5)
}
func (zs *PKG_FooExt1) ToBBuffer(bb *BBuffer) {
	zs.PKG_Foo.ToBBuffer(bb)
	bb.Write_List_Int32(zs.ChildIds)
}
func (zs *PKG_FooExt1) FromBBuffer(bb *BBuffer) {
	zs.PKG_Foo.FromBBuffer(bb)
	zs.ChildIds = bb.Read_List_Int32()
}








type List_PKG_FooExt1 []PKG_FooExt1_Interface

// for base class convert simulate
type List_PKG_FooExt1_Interface interface {
	GetList_PKG_FooExt1() *List_PKG_FooExt1
}
func (zs *List_PKG_FooExt1) GetList_PKG_FooExt1() *List_PKG_FooExt1 {
	return zs
}

func (zs *BBuffer) Write_List_PKG_FooExt1(v List_PKG_FooExt1_Interface) {
	if v == nil {
		zs.WriteIObject(nil)
	} else {
		zs.WriteIObject(v.(IObject))
	}
}
func (zs *BBuffer) Read_List_PKG_FooExt1() List_PKG_FooExt1_Interface {
	v := zs.ReadIObject()
	if v == nil {
		return nil
	} else {
		return v.(List_PKG_FooExt1_Interface)
	}
}
func (zs *BBuffer) TryReadRoot_List_PKG_FooExt1() (r *List_PKG_FooExt1, ok bool) {
	defer func() {
		//recover()
	}()
	v := zs.ReadRoot()
	if v != nil {
		r = v.(List_PKG_FooExt1_Interface).GetList_PKG_FooExt1()
	}
	ok = true
	return
}


// IObject implement
func (zs *List_PKG_FooExt1) GetPackageId() uint16 {
	return uint16(6)
}
func (zs *List_PKG_FooExt1) ToBBuffer(bb *BBuffer) {
	bb.WriteLength(len(*zs))
	for _, v := range *zs {
		bb.Write_PKG_FooExt1(v)
	}
}
func (zs *List_PKG_FooExt1) FromBBuffer(bb *BBuffer) {
	count := bb.ReadLength()
	if bb.ReadLengthLimit != 0 && count > bb.ReadLengthLimit {
		panic(-1)
	}
	*zs = (*zs)[:0]
	if count == 0 {
		return
	}
	for i := 0; i < count; i++ {
		*zs = append(*zs, bb.Read_PKG_FooExt1())
	}
}

// util func
func (zs *List_PKG_FooExt1) SwapRemoveAt(idx int) {
	count := len(*zs)
	if idx + 1 < count {
		(*zs)[idx] = (*zs)[count - 1]
	}
	*zs = (*zs)[:count - 1]
}










func PKG_RegisterAll() {
	RegisterInternals()
	Register(3, func() IObject {
		return &PKG_Foo{}
	})
	Register(4, func() IObject {
		return &List_Int32{}
	})
	Register(5, func() IObject {
		return &PKG_FooExt1{}
	})
	Register(6, func() IObject {
		return &List_PKG_FooExt1{}
	})
	// ... more
}


// todo: 测试枚举模拟. 参考 pb3


func main() {
	PKG_RegisterAll()
	bb := BBuffer{}

	foo := &PKG_Foo{
		10,
		NullableString{"asdf", true},
		NullableInt32{0, false},
		nil,
	}
	//foo.Parent = foo
	bb.WriteRoot(foo)

	fmt.Println(foo)
	fmt.Println(bb)

	if foo2, ok := bb.TryReadRoot_PKG_Foo(); ok {
		fmt.Println(foo2)
	}


	fooExt1 := &PKG_FooExt1{*foo, &List_Int32{ 1,2,3,4 }}
	fooExt1.Parent = fooExt1
	bb.Clear()
	bb.WriteRoot(fooExt1)

	fmt.Println(fooExt1)
	fmt.Println(bb)

	if fooExt2, ok := bb.TryReadRoot_PKG_FooExt1(); ok {
		fmt.Println(fooExt2)
	}


	fooExts := &List_PKG_FooExt1{}
	*fooExts = append(*fooExts, fooExt1)
	*fooExts = append(*fooExts, fooExt1)
	*fooExts = append(*fooExts, fooExt1)

	bb.Clear()
	bb.WriteRoot(fooExts)
	fmt.Println(fooExts)
	fmt.Println(bb)

	if fooExts2, ok := bb.TryReadRoot_List_PKG_FooExt1(); ok {
		fmt.Println(fooExts2)
		fmt.Println((*fooExts2)[0])
	} else {
		fmt.Println(ok)
	}
}
