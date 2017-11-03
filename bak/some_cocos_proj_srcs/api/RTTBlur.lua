
--------------------------------
-- @module RTTBlur
-- @extend Node
-- @parent_module ww

--------------------------------
-- @function [parent=#RTTBlur] removeRTTNode 
-- @param self
-- @param #cc.Node node
        
--------------------------------
-- @function [parent=#RTTBlur] enabledAutoRedraw 
-- @param self
-- @param #bool bool
        
--------------------------------
-- @function [parent=#RTTBlur] getPixelFormat 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#RTTBlur] addRTTNode 
-- @param self
-- @param #cc.Node node
        
--------------------------------
-- @function [parent=#RTTBlur] clearRTTNodes 
-- @param self
        
--------------------------------
-- @function [parent=#RTTBlur] setSampleNum 
-- @param self
-- @param #float float
        
--------------------------------
-- @function [parent=#RTTBlur] setBlurRadius 
-- @param self
-- @param #float float
        
--------------------------------
-- @function [parent=#RTTBlur] isAutoRedraw 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#RTTBlur] getSampleNum 
-- @param self
-- @return float#float ret (return value: float)
        
--------------------------------
-- @function [parent=#RTTBlur] setPixelFormat 
-- @param self
-- @param #int pixelformat
        
--------------------------------
-- @function [parent=#RTTBlur] setRTTNodes 
-- @param self
-- @param #array_table array
        
--------------------------------
-- @function [parent=#RTTBlur] getBlurRadius 
-- @param self
-- @return float#float ret (return value: float)
        
--------------------------------
-- @overload self, float, float, int         
-- @overload self         
-- @function [parent=#RTTBlur] create
-- @param self
-- @param #float float
-- @param #float float
-- @param #int pixelformat
-- @return RTTBlur#RTTBlur ret (retunr value: RTTBlur)

--------------------------------
-- @function [parent=#RTTBlur] setContentSize 
-- @param self
-- @param #size_table size
        
return nil
