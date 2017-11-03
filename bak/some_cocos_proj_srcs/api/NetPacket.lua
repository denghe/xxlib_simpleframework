
--------------------------------
-- @module NetPacket
-- @extend Ref
-- @parent_module ww

--------------------------------
-- @function [parent=#NetPacket] dump 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- @function [parent=#NetPacket] orfree 
-- @param self
-- @param #int int
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#NetPacket] clear 
-- @param self
        
--------------------------------
-- @function [parent=#NetPacket] wfree 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#NetPacket] dws 
-- @param self
-- @param #char char
        
--------------------------------
-- @function [parent=#NetPacket] prepare 
-- @param self
        
--------------------------------
-- @function [parent=#NetPacket] dr1 
-- @param self
-- @return char#char ret (return value: char)
        
--------------------------------
-- @function [parent=#NetPacket] bufLen 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#NetPacket] dr2 
-- @param self
-- @return short#short ret (return value: short)
        
--------------------------------
-- @function [parent=#NetPacket] dr4 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#NetPacket] owfree 
-- @param self
-- @param #int int
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#NetPacket] dwf 
-- @param self
-- @param #float float
        
--------------------------------
-- @function [parent=#NetPacket] clearRo 
-- @param self
        
--------------------------------
-- @function [parent=#NetPacket] dwd 
-- @param self
-- @param #double double
        
--------------------------------
-- @function [parent=#NetPacket] dru1 
-- @param self
-- @return unsigned char#unsigned char ret (return value: unsigned char)
        
--------------------------------
-- @overload self, int         
-- @overload self         
-- @function [parent=#NetPacket] woffset
-- @param self
-- @param #int int

--------------------------------
-- @function [parent=#NetPacket] dru2 
-- @param self
-- @return unsigned short#unsigned short ret (return value: unsigned short)
        
--------------------------------
-- @function [parent=#NetPacket] dru4 
-- @param self
-- @return unsigned int#unsigned int ret (return value: unsigned int)
        
--------------------------------
-- @function [parent=#NetPacket] pkgLen 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#NetPacket] dwu1 
-- @param self
-- @param #unsigned char char
        
--------------------------------
-- @function [parent=#NetPacket] dwu2 
-- @param self
-- @param #unsigned short short
        
--------------------------------
-- @function [parent=#NetPacket] dwu4 
-- @param self
-- @param #unsigned int int
        
--------------------------------
-- @function [parent=#NetPacket] dw2 
-- @param self
-- @param #short short
        
--------------------------------
-- @function [parent=#NetPacket] dw1 
-- @param self
-- @param #char char
        
--------------------------------
-- @function [parent=#NetPacket] dw4 
-- @param self
-- @param #int int
        
--------------------------------
-- @function [parent=#NetPacket] clearWo 
-- @param self
        
--------------------------------
-- @function [parent=#NetPacket] rfree 
-- @param self
-- @return int#int ret (return value: int)
        
--------------------------------
-- @function [parent=#NetPacket] drs 
-- @param self
-- @return string#string ret (return value: string)
        
--------------------------------
-- @overload self, int         
-- @overload self         
-- @function [parent=#NetPacket] roffset
-- @param self
-- @param #int int

--------------------------------
-- @overload self, unsigned short         
-- @overload self         
-- @function [parent=#NetPacket] opcode
-- @param self
-- @param #unsigned short short

--------------------------------
-- @function [parent=#NetPacket] drd 
-- @param self
-- @return double#double ret (return value: double)
        
--------------------------------
-- @function [parent=#NetPacket] drf 
-- @param self
-- @return float#float ret (return value: float)
        
--------------------------------
-- @function [parent=#NetPacket] create 
-- @param self
-- @return NetPacket#NetPacket ret (return value: NetPacket)
        
return nil
