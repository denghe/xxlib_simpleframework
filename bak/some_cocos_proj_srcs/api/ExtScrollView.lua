
--------------------------------
-- @module ExtScrollView
-- @extend Layer,ActionTweenDelegate
-- @parent_module ww

--------------------------------
-- @function [parent=#ExtScrollView] isClippingToBounds 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#ExtScrollView] getMinContainerOffset 
-- @param self
-- @return vec2_table#vec2_table ret (return value: vec2_table)
        
--------------------------------
-- @function [parent=#ExtScrollView] unregisterActionEventHandler 
-- @param self
        
--------------------------------
-- @function [parent=#ExtScrollView] setContentOffsetInDuration 
-- @param self
-- @param #vec2_table vec2
-- @param #float float
        
--------------------------------
-- @function [parent=#ExtScrollView] setZoomScaleInDuration 
-- @param self
-- @param #float float
-- @param #float float
        
--------------------------------
-- @function [parent=#ExtScrollView] setContainer 
-- @param self
-- @param #cc.Node node
        
--------------------------------
-- @function [parent=#ExtScrollView] setZoomScaleLimit 
-- @param self
-- @param #float float
-- @param #float float
        
--------------------------------
-- @function [parent=#ExtScrollView] getContainer 
-- @param self
-- @return Node#Node ret (return value: cc.Node)
        
--------------------------------
-- @function [parent=#ExtScrollView] setCanMovedOut 
-- @param self
-- @param #bool bool
        
--------------------------------
-- @function [parent=#ExtScrollView] getDirection 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#ExtScrollView] getZoomScale 
-- @param self
-- @return float#float ret (return value: float)
        
--------------------------------
-- @function [parent=#ExtScrollView] pause 
-- @param self
-- @param #cc.Ref ref
        
--------------------------------
-- @function [parent=#ExtScrollView] setDirection 
-- @param self
-- @param #int scrollviewdirection
        
--------------------------------
-- @function [parent=#ExtScrollView] isEnabled 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#ExtScrollView] setBounceable 
-- @param self
-- @param #bool bool
        
--------------------------------
-- @function [parent=#ExtScrollView] setContentOffset 
-- @param self
-- @param #vec2_table vec2
-- @param #bool bool
        
--------------------------------
-- @function [parent=#ExtScrollView] isDragging 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#ExtScrollView] isBounceable 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#ExtScrollView] getMaxContainerOffset 
-- @param self
-- @return vec2_table#vec2_table ret (return value: vec2_table)
        
--------------------------------
-- @function [parent=#ExtScrollView] setEnabled 
-- @param self
-- @param #bool bool
        
--------------------------------
-- @function [parent=#ExtScrollView] getContentOffset 
-- @param self
-- @return vec2_table#vec2_table ret (return value: vec2_table)
        
--------------------------------
-- @function [parent=#ExtScrollView] getCanMovedOut 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#ExtScrollView] resume 
-- @param self
-- @param #cc.Ref ref
        
--------------------------------
-- @function [parent=#ExtScrollView] setClippingToBounds 
-- @param self
-- @param #bool bool
        
--------------------------------
-- @function [parent=#ExtScrollView] setViewSize 
-- @param self
-- @param #size_table size
        
--------------------------------
-- @function [parent=#ExtScrollView] getViewSize 
-- @param self
-- @return size_table#size_table ret (return value: size_table)
        
--------------------------------
-- @function [parent=#ExtScrollView] isTouchMoved 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#ExtScrollView] isNodeVisible 
-- @param self
-- @param #cc.Node node
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @overload self, float, bool         
-- @overload self, float         
-- @function [parent=#ExtScrollView] setZoomScale
-- @param self
-- @param #float float
-- @param #bool bool

--------------------------------
-- @function [parent=#ExtScrollView] create 
-- @param self
-- @param #size_table size
-- @param #cc.Node node
-- @return ExtScrollView#ExtScrollView ret (return value: ExtScrollView)
        
--------------------------------
-- @function [parent=#ExtScrollView] convertDistanceFromPointToInch 
-- @param self
-- @param #float float
-- @return float#float ret (return value: float)
        
--------------------------------
-- @function [parent=#ExtScrollView] addChild 
-- @param self
-- @param #cc.Node node
-- @param #int int
-- @param #int int
        
--------------------------------
-- @function [parent=#ExtScrollView] setContentSize 
-- @param self
-- @param #size_table size
        
--------------------------------
-- @function [parent=#ExtScrollView] visit 
-- @param self
-- @param #cc.Renderer renderer
-- @param #mat4_table mat4
-- @param #unsigned int int
        
--------------------------------
-- @function [parent=#ExtScrollView] getContentSize 
-- @param self
-- @return size_table#size_table ret (return value: size_table)
        
return nil
