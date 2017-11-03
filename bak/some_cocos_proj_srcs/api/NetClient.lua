
--------------------------------
-- @module NetClient
-- @extend Ref
-- @parent_module ww

--------------------------------
-- @function [parent=#NetClient] getState 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#NetClient] isAlive 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#NetClient] clear 
-- @param self
        
--------------------------------
-- @overload self, NetPacket         
-- @overload self, char, int         
-- @function [parent=#NetClient] send
-- @param self
-- @param #char char
-- @param #int int
-- @return bool#bool ret (retunr value: bool)

--------------------------------
-- @function [parent=#NetClient] isClosed 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @function [parent=#NetClient] setParms 
-- @param self
-- @param #string str
-- @param #unsigned short short
-- @param #int int
        
--------------------------------
-- @function [parent=#NetClient] isDead 
-- @param self
-- @return bool#bool ret (return value: bool)
        
--------------------------------
-- @overload self, string, unsigned short, int         
-- @overload self         
-- @function [parent=#NetClient] connect
-- @param self
-- @param #string str
-- @param #unsigned short short
-- @param #int int
-- @return bool#bool ret (retunr value: bool)

--------------------------------
-- @function [parent=#NetClient] close 
-- @param self
        
--------------------------------
-- @function [parent=#NetClient] unregisterNetEventHandler 
-- @param self
        
--------------------------------
-- @function [parent=#NetClient] create 
-- @param self
-- @return NetClient#NetClient ret (return value: NetClient)
        
return nil
