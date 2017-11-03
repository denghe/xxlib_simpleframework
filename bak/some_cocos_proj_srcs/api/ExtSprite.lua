
--------------------------------
-- @module ExtSprite
-- @extend Sprite

--------------------------------
-- @function [parent=#ExtSprite] setGray 
-- @param self
-- @param #bool bool
        
--------------------------------
-- @function [parent=#ExtSprite] isGray 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#ExtSprite] setBlur 
-- @param self
-- @param #bool bool
        
--------------------------------
-- @function [parent=#ExtSprite] isBlur 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- overload function: create(char, rect_table)
--          
-- overload function: create(char)
--          
-- overload function: create()
--          
-- @function [parent=#ExtSprite] create
-- @param self
-- @param #char char
-- @param #rect_table rect
-- @return ExtSprite#ExtSprite ret (retunr value: ExtSprite)

--------------------------------
-- overload function: createWithTexture(cc.Texture2D, rect_table)
--          
-- overload function: createWithTexture(cc.Texture2D)
--          
-- @function [parent=#ExtSprite] createWithTexture
-- @param self
-- @param #cc.Texture2D texture2d
-- @param #rect_table rect
-- @return ExtSprite#ExtSprite ret (retunr value: ExtSprite)

--------------------------------
-- overload function: init_shader(char, char, char)
--          
-- overload function: init_shader(bool)
--          
-- @function [parent=#ExtSprite] init_shader
-- @param self
-- @param #char char
-- @param #char char
-- @param #char char

--------------------------------
-- @function [parent=#ExtSprite] createWithSpriteFrameName 
-- @param self
-- @param #char char
-- @return ExtSprite#ExtSprite ret (return value: ExtSprite)
        
--------------------------------
-- @function [parent=#ExtSprite] createWithSpriteFrame 
-- @param self
-- @param #cc.SpriteFrame spriteframe
-- @return ExtSprite#ExtSprite ret (return value: ExtSprite)
        
return nil
