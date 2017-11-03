
--------------------------------
-- @module ExtClippingNode
-- @extend Node
-- @parent_module ww

--------------------------------
-- @function [parent=#ExtClippingNode] setTouchMoved 
-- @param self
-- @param #bool bool
        
--------------------------------
-- @function [parent=#ExtClippingNode] getClippingRegion 
-- @param self
-- @return rect_table#rect_table ret (return value: rect_table)
        
--------------------------------
-- @function [parent=#ExtClippingNode] isTouchMoved 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#ExtClippingNode] setClippingRegion 
-- @param self
-- @param #rect_table rect
        
--------------------------------
-- @overload self         
-- @overload self, rect_table         
-- @function [parent=#ExtClippingNode] create
-- @param self
-- @param #rect_table rect
-- @return ExtClippingNode#ExtClippingNode ret (retunr value: ExtClippingNode)

--------------------------------
-- @function [parent=#ExtClippingNode] visit 
-- @param self
-- @param #cc.Renderer renderer
-- @param #mat4_table mat4
-- @param #unsigned int int
        
return nil
