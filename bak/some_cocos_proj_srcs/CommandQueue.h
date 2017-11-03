#ifndef __COMMANDQUEUE_H__
#define __COMMANDQUEUE_H__

// push 上锁 的 lambda 队列, 延迟到当前游戏帧结束前 依次执行, 执行时不锁
class CommandQueue
{
public:
    MAKE_INSTANCE_H( CommandQueue );

    typedef function<void()> FT;

    // frame update 时调用, 依次执行 _fs 中的函数
    void update();

    // 压入一个回调( 这个不生成 lua bind )
    void push( FT f );

    // 压入一个 lua 函数回调
    void push( LUA_FUNCTION f );

private:
    // 啥都不干
    CommandQueue();

    // 啥都不干
    ~CommandQueue();

    vector<FT>              _fs;
    mutex                   _m;
};


#endif

