#ifndef _PURCHASE_H_
#define _PURCHASE_H_

class Purchase;

class BuyInfo : public Ref
{
    std::string _productId;
    std::string _verifyURL;
    int _charId;
    int _accountId;
    int _regionId;
    std::string _accountName;
    std::string _transactionId;
    std::string _transactionBase64;
    std::string _exchangeURL;

    friend Purchase;
public:
    BuyInfo();
    ~BuyInfo();
    BuyInfo( std::string const& productId
             , std::string const& verifyURL
             , std::string const& accountName
             , int charId
             , int accountId
             , int regionId
             , std::string const& transactionId
             , std::string const& transactionBase64
             , std::string const& exchangeURL
             );
    BuyInfo( BuyInfo const& other );
    BuyInfo( BuyInfo&& other );
    BuyInfo& operator=( BuyInfo const& other );
    BuyInfo& operator=( BuyInfo&& other );
    bool operator==( BuyInfo const& other );

    /************************************************************************/
    // 下列函数 lua 下可用
    /************************************************************************/

    static BuyInfo* create();

    std::string const& getProductId() const;
    void setProductId( std::string val );
    std::string const& getVerifyURL() const;
    void setVerifyURL( std::string val );
    int getCharId() const;
    void setCharId( int val );
    int getAccountId() const;
    void setAccountId( int val );
    int getRegionId() const;
    void setRegionId( int val );
    std::string const& getAccountName() const;
    void setAccountName( std::string val );
    std::string const& getTransactionId() const;
    void setTransactionId( std::string val );
    std::string const& getTransactionBase64() const;
    void setTransactionBase64( std::string val );
    std::string const& getExchangeURL() const;
    void setExchangeURL( std::string val );
};





class Product : public Ref
{
    std::string _pid;
    std::string _title;
    std::string _desc;
    std::string _price;

    friend Purchase;
public:
    Product();
    ~Product();
    Product( std::string const& pid
             , std::string const& title
             , std::string const& desc
             , std::string const& price );
    Product( Product const& other );
    Product( Product&& other );
    Product& operator=( Product const& other );
    Product& operator=( Product&& other );
    bool operator==( Product const& other );

    /************************************************************************/
    // 下列函数 lua 下可用
    /************************************************************************/

    static Product* create();

    std::string const& getPid() const;
    void setPid( std::string val );
    std::string const& getTitle() const;
    void setTitle( std::string val );
    std::string const& getDesc() const;
    void setDesc( std::string val );
    std::string const& getPrice() const;
    void setPrice( std::string val );
};





class Products
{
    friend Purchase;
    Products();
    ~Products();

public:
    std::vector<Product*> _data;

    /************************************************************************/
    // 下列函数 lua 下可用
    /************************************************************************/

    void insert( Product* p );
    void erase( std::string const& pid );
    int size() const;
    bool empty() const;
    void clear();
    Product* at( int idx ) const;
};







class Purchase
{
    MAKE_INSTANCE_H( Purchase );
public:
    typedef std::function<void( GenericEvent<PurchaseEventTypes> const& )> EventHandlerType;

    // 注册事件回调
    void registerEventHandler( EventHandlerType f );

    // 更新函数，扫各种处理结果，激发各种事件回调
    void update();

    // 插入一个事件
    template<typename ET>
    void pushEvent( ET&& e )
    {
        _events.push( std::forward<ET>( e ) );
    }


    // for IOS
    // 本函数实现为平台代码
    // 本函数不直接使用
    // 将 事务id 和内容 发至 web server 校验
    // web server 会先查数据库，队列，根据状态路由
    // 如果未校验，则发到 apple server 校验，通过后通知 GS 发放
    // 当 game server 处理完成后，客户端应该会需要收到一个推送, 里面有 tid, 用于关闭交易
    void verify();

    /************************************************************************/
    // 下列函数 lua 下可用
    /************************************************************************/

    // 本函数实现为平台代码
    // 设置购买上下文( ios 将启用交易队列监听 )
    // 于游戏进入主城后调用( 这样下列参数才有 )
    void start( std::string const& verifyURL,
                int charId,
                int accountId,
                int regionId,
                std::string const& accountName,
                std::string const& exchangeURL
                );


    // 本函数实现为平台代码
    // ios 将停止交易队列监听
    void stop();

    // 注册事件回调( 这个要手写 LUA )
    void registerEventHandler( LUA_FUNCTION f );

    // 添加产品到 _products（其 id 须与平台购物渠道配的相同）
    void add( std::string const& id
              , std::string const& title
              , std::string const& desc
              , std::string const& price );

    // 从 _products 移除产品
    void remove( std::string const& id );

    // 得到产品列表
    Products* getProducts();
    
    // 得到购买信息
    BuyInfo* getBuyInfo();

    // for IOS
    // 本函数实现为平台代码
    // 开始拉产品表( android, windows 不需要拉 )
    bool pull( std::vector<std::string> const& pids );

    // 本函数实现为平台代码
    // IOS: 使用系统SDK, 非阻塞调用，开始买（此时游戏须等待 BuyXxxx, VerifyXxxx 事件及 服务器通知）
    // 如果成功， bi 的 trans 相关将被填充
    bool buy( std::string const& pid );

    // for IOS
    // 本函数实现为平台代码
    // 关闭交易( 通常于收到游戏服通知后调用 ), 回收 BuyInfo( 如果上下文中存在的话 )
    void close( std::string const& transId );


    // 全平台
    // 玩家输入序列号，服务器发放相应的东西
    void exchange( std::string const& serial );

private:
    // 事件处理回调
    EventHandlerType _eventHandler;

    // 事件队列，将于 update 时发起
    ThreadSafeQueue<GenericEvent<PurchaseEventTypes>> _events;

    // 主要用于显示的产品表
    Products _products;

    // 购买上下文
    BuyInfo _buyInfo;
};

#endif

