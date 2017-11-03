/**
 * @module ww
 */
var ww = ww || {};

/**
 * @class ExtSprite
 */
ww.ExtSprite = {

/**
 * @method setGray
 * @param {bool}
 */
setGray : function () {},

/**
 * @method isGray
 * @return A value converted from C/C++ "bool"
 */
isGray : function () {},

/**
 * @method setBlur
 * @param {bool}
 */
setBlur : function () {},

/**
 * @method isBlur
 * @return A value converted from C/C++ "bool"
 */
isBlur : function () {},

/**
 * @method createWithSpriteFrameName
 * @return A value converted from C/C++ "ExtSprite*"
 * @param {const char*}
 */
createWithSpriteFrameName : function () {},

/**
 * @method createWithSpriteFrame
 * @return A value converted from C/C++ "ExtSprite*"
 * @param {cocos2d::SpriteFrame*}
 */
createWithSpriteFrame : function () {},

};

/**
 * @class ExtButton
 */
ww.ExtButton = {

/**
 * @method setZoomOnTouchDown
 * @param {bool}
 */
setZoomOnTouchDown : function () {},

/**
 * @method getState
 * @return A value converted from C/C++ "const ButtonStates&"
 */
getState : function () {},

/**
 * @method setBg
 * @param {cocos2d::Node*}
 */
setBg : function () {},

/**
 * @method unregisterEnableHandler
 */
unregisterEnableHandler : function () {},

/**
 * @method setEnabled
 * @param {bool}
 */
setEnabled : function () {},

/**
 * @method getBg
 * @return A value converted from C/C++ "cocos2d::Node*"
 */
getBg : function () {},

/**
 * @method getFg
 * @return A value converted from C/C++ "cocos2d::Node*"
 */
getFg : function () {},

/**
 * @method isEnabled
 * @return A value converted from C/C++ "bool"
 */
isEnabled : function () {},

/**
 * @method isPushed
 * @return A value converted from C/C++ "bool"
 */
isPushed : function () {},

/**
 * @method unregisterHighlightHandler
 */
unregisterHighlightHandler : function () {},

/**
 * @method isSwallow
 * @return A value converted from C/C++ "bool"
 */
isSwallow : function () {},

/**
 * @method setFg
 * @param {cocos2d::Node*}
 */
setFg : function () {},

/**
 * @method setHighlighted
 * @param {bool}
 */
setHighlighted : function () {},

/**
 * @method setSwallow
 * @param {bool}
 */
setSwallow : function () {},

/**
 * @method isZoomOnTouchDown
 * @return A value converted from C/C++ "bool"
 */
isZoomOnTouchDown : function () {},

/**
 * @method unregisterTouchEventHandler
 */
unregisterTouchEventHandler : function () {},

/**
 * @method isHighlighted
 * @return A value converted from C/C++ "bool"
 */
isHighlighted : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "ExtButton*"
 * @param {cocos2d::Node*}
 * @param {cocos2d::Node*}
 * @param {bool}
 * @param {int}
 */
create : function () {},

};

/**
 * @class ExtClippingNode
 */
ww.ExtClippingNode = {

/**
 * @method setTouchMoved
 * @param {bool}
 */
setTouchMoved : function () {},

/**
 * @method getClippingRegion
 * @return A value converted from C/C++ "cocos2d::Rect"
 */
getClippingRegion : function () {},

/**
 * @method isTouchMoved
 * @return A value converted from C/C++ "bool"
 */
isTouchMoved : function () {},

/**
 * @method setClippingRegion
 * @param {const cocos2d::Rect&}
 */
setClippingRegion : function () {},

/**
 * @method convertDistanceFromPointToInch
 * @return A value converted from C/C++ "float"
 * @param {float}
 */
convertDistanceFromPointToInch : function () {},

};

/**
 * @class ExtScrollView
 */
ww.ExtScrollView = {

/**
 * @method isClippingToBounds
 * @return A value converted from C/C++ "bool"
 */
isClippingToBounds : function () {},

/**
 * @method getMinContainerOffset
 * @return A value converted from C/C++ "cocos2d::Point"
 */
getMinContainerOffset : function () {},

/**
 * @method unregisterActionEventHandler
 */
unregisterActionEventHandler : function () {},

/**
 * @method setContentOffsetInDuration
 * @param {cocos2d::Point}
 * @param {float}
 */
setContentOffsetInDuration : function () {},

/**
 * @method setZoomScaleInDuration
 * @param {float}
 * @param {float}
 */
setZoomScaleInDuration : function () {},

/**
 * @method setContainer
 * @param {cocos2d::Node*}
 */
setContainer : function () {},

/**
 * @method getContainer
 * @return A value converted from C/C++ "cocos2d::Node*"
 */
getContainer : function () {},

/**
 * @method getDirection
 * @return A value converted from C/C++ "ScrollViewDirection"
 */
getDirection : function () {},

/**
 * @method getZoomScale
 * @return A value converted from C/C++ "float"
 */
getZoomScale : function () {},

/**
 * @method updateInset
 */
updateInset : function () {},

/**
 * @method pause
 * @param {cocos2d::Object*}
 */
pause : function () {},

/**
 * @method setDirection
 * @param {ScrollViewDirection}
 */
setDirection : function () {},

/**
 * @method isEnabled
 * @return A value converted from C/C++ "bool"
 */
isEnabled : function () {},

/**
 * @method setBounceable
 * @param {bool}
 */
setBounceable : function () {},

/**
 * @method setContentOffset
 * @param {cocos2d::Point}
 * @param {bool}
 */
setContentOffset : function () {},

/**
 * @method isDragging
 * @return A value converted from C/C++ "bool"
 */
isDragging : function () {},

/**
 * @method isBounceable
 * @return A value converted from C/C++ "bool"
 */
isBounceable : function () {},

/**
 * @method getMaxContainerOffset
 * @return A value converted from C/C++ "cocos2d::Point"
 */
getMaxContainerOffset : function () {},

/**
 * @method stopScroll
 */
stopScroll : function () {},

/**
 * @method setEnabled
 * @param {bool}
 */
setEnabled : function () {},

/**
 * @method getContentOffset
 * @return A value converted from C/C++ "cocos2d::Point"
 */
getContentOffset : function () {},

/**
 * @method stoppedAnimatedScroll
 * @param {cocos2d::CCNode*}
 */
stoppedAnimatedScroll : function () {},

/**
 * @method resume
 * @param {cocos2d::Object*}
 */
resume : function () {},

/**
 * @method setClippingToBounds
 * @param {bool}
 */
setClippingToBounds : function () {},

/**
 * @method setViewSize
 * @param {cocos2d::Size}
 */
setViewSize : function () {},

/**
 * @method getViewSize
 * @return A value converted from C/C++ "cocos2d::Size"
 */
getViewSize : function () {},

/**
 * @method performedAnimatedScroll
 * @param {float}
 */
performedAnimatedScroll : function () {},

/**
 * @method isTouchMoved
 * @return A value converted from C/C++ "bool"
 */
isTouchMoved : function () {},

/**
 * @method isNodeVisible
 * @return A value converted from C/C++ "bool"
 * @param {cocos2d::Node*}
 */
isNodeVisible : function () {},

/**
 * @method create
 * @return A value converted from C/C++ "ExtScrollView*"
 * @param {cocos2d::Size}
 * @param {cocos2d::Node*}
 */
create : function () {},

};

/**
 * @class RichText
 */
ww.RichText = {

/**
 * @method AddColor
 * @return A value converted from C/C++ "bool"
 * @param {int}
 * @param {int8}
 * @param {int8}
 * @param {int8}
 */
AddColor : function () {},

/**
 * @method creNode
 * @return A value converted from C/C++ "cocos2d::Node*"
 * @param {const char*}
 * @param {float}
 * @param {float}
 * @param {int}
 * @param {const char*}
 * @param {float}
 */
creNode : function () {},

};

/**
 * @class NetPacket
 */
ww.NetPacket = {

/**
 * @method dump
 * @return A value converted from C/C++ "std::string"
 */
dump : function () {},

/**
 * @method orfree
 * @return A value converted from C/C++ "int"
 * @param {int}
 */
orfree : function () {},

/**
 * @method clear
 */
clear : function () {},

/**
 * @method wfree
 * @return A value converted from C/C++ "int"
 */
wfree : function () {},

/**
 * @method dws
 * @param {const char*}
 */
dws : function () {},

/**
 * @method prepare
 */
prepare : function () {},

/**
 * @method dr1
 * @return A value converted from C/C++ "int8"
 */
dr1 : function () {},

/**
 * @method bufLen
 * @return A value converted from C/C++ "int"
 */
bufLen : function () {},

/**
 * @method dr2
 * @return A value converted from C/C++ "int16"
 */
dr2 : function () {},

/**
 * @method dr4
 * @return A value converted from C/C++ "int32"
 */
dr4 : function () {},

/**
 * @method owfree
 * @return A value converted from C/C++ "int"
 * @param {int}
 */
owfree : function () {},

/**
 * @method dwf
 * @param {float}
 */
dwf : function () {},

/**
 * @method clearRo
 */
clearRo : function () {},

/**
 * @method dwd
 * @param {double}
 */
dwd : function () {},

/**
 * @method dru1
 * @return A value converted from C/C++ "uint8"
 */
dru1 : function () {},

/**
 * @method dru2
 * @return A value converted from C/C++ "uint16"
 */
dru2 : function () {},

/**
 * @method dru4
 * @return A value converted from C/C++ "uint32"
 */
dru4 : function () {},

/**
 * @method pkgLen
 * @return A value converted from C/C++ "int"
 */
pkgLen : function () {},

/**
 * @method dataLen
 * @return A value converted from C/C++ "int"
 */
dataLen : function () {},

/**
 * @method dwu1
 * @param {uint8}
 */
dwu1 : function () {},

/**
 * @method dwu2
 * @param {uint16}
 */
dwu2 : function () {},

/**
 * @method dwu4
 * @param {uint32}
 */
dwu4 : function () {},

/**
 * @method dw2
 * @param {int16}
 */
dw2 : function () {},

/**
 * @method dw1
 * @param {int8}
 */
dw1 : function () {},

/**
 * @method dw4
 * @param {int32}
 */
dw4 : function () {},

/**
 * @method clearWo
 */
clearWo : function () {},

/**
 * @method rfree
 * @return A value converted from C/C++ "int"
 */
rfree : function () {},

/**
 * @method drs
 * @return A value converted from C/C++ "std::string"
 */
drs : function () {},

/**
 * @method drd
 * @return A value converted from C/C++ "double"
 */
drd : function () {},

/**
 * @method drf
 * @return A value converted from C/C++ "float"
 */
drf : function () {},

};

/**
 * @class NetEngine
 */
ww.NetEngine = {

/**
 * @method getNetType
 * @return A value converted from C/C++ "NetTypes"
 */
getNetType : function () {},

/**
 * @method closeAll
 */
closeAll : function () {},

/**
 * @method makeWorker
 * @return A value converted from C/C++ "bool"
 * @param {NetClient*}
 * @param {const char*}
 * @param {uint16}
 * @param {int}
 */
makeWorker : function () {},

/**
 * @method shutdown
 */
shutdown : function () {},

/**
 * @method initInstance
 */
initInstance : function () {},

/**
 * @method getInstance
 * @return A value converted from C/C++ "NetEngine*"
 */
getInstance : function () {},

/**
 * @method freeInstance
 */
freeInstance : function () {},

/**
 * @method NetEngine
 * @constructor
 */
NetEngine : function () {},

};

/**
 * @class NetClient
 */
ww.NetClient = {

/**
 * @method getState
 * @return A value converted from C/C++ "SocketStates"
 */
getState : function () {},

/**
 * @method isAlive
 * @return A value converted from C/C++ "bool"
 */
isAlive : function () {},

/**
 * @method clear
 */
clear : function () {},

/**
 * @method isClosed
 * @return A value converted from C/C++ "bool"
 */
isClosed : function () {},

/**
 * @method setParms
 * @param {std::string}
 * @param {uint16}
 * @param {int}
 */
setParms : function () {},

/**
 * @method isDead
 * @return A value converted from C/C++ "bool"
 */
isDead : function () {},

/**
 * @method close
 */
close : function () {},

/**
 * @method unregisterNetEventHandler
 */
unregisterNetEventHandler : function () {},

/**
 * @method NetClient
 * @constructor
 */
NetClient : function () {},

};

/**
 * @class NetHttp
 */
ww.NetHttp = {

/**
 * @method cancel
 * @param {const std::string&}
 */
cancel : function () {},

/**
 * @method getInstance
 * @return A value converted from C/C++ "NetHttp*"
 */
getInstance : function () {},

};

/**
 * @class FileIndexRow
 */
ww.FileIndexRow = {

/**
 * @method getState
 * @return A value converted from C/C++ "FileStates"
 */
getState : function () {},

/**
 * @method dump
 */
dump : function () {},

/**
 * @method getType
 * @return A value converted from C/C++ "FileTypes"
 */
getType : function () {},

/**
 * @method getSize
 * @return A value converted from C/C++ "int"
 */
getSize : function () {},

/**
 * @method getFullName
 * @return A value converted from C/C++ "std::string"
 */
getFullName : function () {},

};

/**
 * @class FileIndex
 */
ww.FileIndex = {

/**
 * @method load
 */
load : function () {},

/**
 * @method getRow
 * @return A value converted from C/C++ "FileIndexRow*"
 * @param {const std::string&}
 */
getRow : function () {},

/**
 * @method getNames
 * @return A value converted from C/C++ "std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >"
 */
getNames : function () {},

};

/**
 * @class FileManager
 */
ww.FileManager = {

/**
 * @method load
 * @return A value converted from C/C++ "bool"
 * @param {const std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >&}
 */
load : function () {},

/**
 * @method loadIndex
 * @return A value converted from C/C++ "bool"
 * @param {const std::string&}
 * @param {const std::string&}
 */
loadIndex : function () {},

/**
 * @method getIndex
 * @return A value converted from C/C++ "FileIndex*"
 */
getIndex : function () {},

/**
 * @method unload
 * @return A value converted from C/C++ "bool"
 * @param {const std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >&}
 */
unload : function () {},

/**
 * @method download
 * @param {const std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > >&}
 */
download : function () {},

/**
 * @method getFileIndexRow
 * @return A value converted from C/C++ "FileIndexRow*"
 * @param {const std::string&}
 */
getFileIndexRow : function () {},

/**
 * @method getInstance
 * @return A value converted from C/C++ "FileManager*"
 */
getInstance : function () {},

};

/**
 * @class Hosts
 */
ww.Hosts = {

/**
 * @method unregisterGameLoopEventHandler
 * @return A value converted from C/C++ "bool"
 * @param {const std::string&}
 */
unregisterGameLoopEventHandler : function () {},

/**
 * @method getTime
 * @return A value converted from C/C++ "int"
 */
getTime : function () {},

/**
 * @method getUDID
 * @return A value converted from C/C++ "const std::string&"
 */
getUDID : function () {},

/**
 * @method initInstance
 */
initInstance : function () {},

/**
 * @method getInstance
 * @return A value converted from C/C++ "Hosts*"
 */
getInstance : function () {},

/**
 * @method freeInstance
 */
freeInstance : function () {},

/**
 * @method Hosts
 * @constructor
 */
Hosts : function () {},

};
