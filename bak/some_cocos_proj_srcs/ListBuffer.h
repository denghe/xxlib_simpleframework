#ifndef _LIST_BUFFER_H_
#define _LIST_BUFFER_H_


/*

简述：网络数据包队列堆叠暂存容器。

内存块结构：
---------------------
|size | next | data  |
|____________________|
4[+4]  4[/8]    n

工作流程：
变量：包，容器，状态( 1:填充包头; 2:填充数据 )
1. 容器.push( socket.receive() )
2. if 状态 == 1 and 容器.size < 6
   or 状态 == 2 and 容器.size < 包.长 then goto 1.
3. if 状态 == 1 {
    包.长 = 容器.copy( 4 bytes )
    if 容器.size >= 包长 {
        goto 4
    }
    else {
        状态 = 2
        goto 1
    }
4. 包.数据 = 容器.copy( 包.长 )
    容器.pop( 包.长 )
    状态 = 1
    处理( 包数据 )   这步有可能是将　包 压入一个队列，然后于engine每帧的某时机取出处理
    goto 1
 
备注：原代码中用到　MCL_ROUND_SIZE　的地方都已改为“round2n”　即　内存分配长度取　刚好大于 x 的 2^n 值，以增加内存块利用率
例如， round2n( 1 ) == 2,  round2n( 9 ) == 16, round2n( 36 ) = 64,  round2n( 4097 ) = 8192

 */
class ListBuffer
{
public:
    ListBuffer( int block_min_size = (int)NetConfigs::ListBufferMinBlockSize );
    ~ListBuffer();

    void push( const char *buf, int len );
    int copy( char *buf, int len );
    int pop( int len );
    void clear();

    int size() const;
    int empty() const;

private:
    struct Block
    {
        int     size;
        Block * next;
        char * data() const;
    };

    void append_block( int len );
    void free_block( Block * block );

    Block *     _header;
    int         _header_ptr;

    Block *     _tail;
    int         _tail_ptr;

    int         _block_min_size;
    int         _size;
};

#endif
