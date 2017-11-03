
--------------------------------
-- @module FontManager
-- @parent_module ww

--------------------------------
-- @function [parent=#FontManager] addFont 
-- @param self
-- @param #string str
-- @param #int int
-- @param #int int
-- @param #color4b_table color4b
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#FontManager] clear 
-- @param self
        
--------------------------------
-- @function [parent=#FontManager] getColorCount 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#FontManager] getFontCount 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#FontManager] addColor 
-- @param self
-- @param #color3b_table color3b
-- @param #string str
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#FontManager] getDefaultColor 
-- @param self
-- @return color3b_table#color3b_table ret (return value: color3b_table)
        
--------------------------------
-- @overload self, string         
-- @overload self, int         
-- @function [parent=#FontManager] getColor
-- @param self
-- @param #int int
-- @return color3b_table#color3b_table ret (retunr value: color3b_table)

--------------------------------
-- @function [parent=#FontManager] createLabel 
-- @param self
-- @param #int int
-- @param #string str
-- @param #int int
-- @return Label#Label ret (return value: cc.Label)
        
--------------------------------
-- @function [parent=#FontManager] createSystemFontLabel 
-- @param self
-- @param #string str
-- @param #int int
-- @param #int int
-- @return Label#Label ret (return value: cc.Label)
        
--------------------------------
-- @function [parent=#FontManager] getInstance 
-- @param self
-- @return FontManager#FontManager ret (return value: FontManager)
        
return nil
