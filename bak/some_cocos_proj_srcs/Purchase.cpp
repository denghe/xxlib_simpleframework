#include "Precompile.h"


BuyInfo::BuyInfo()
    : _charId( 0 )
    , _accountId( 0 )
{
}

BuyInfo::BuyInfo( std::string const& productId, std::string const& verifyURL, std::string const& accountName, int charId, int accountId, int regionId, std::string const& transactionId, std::string const& transactionBase64, std::string const& exchangeURL )
    : _productId( productId )
    , _verifyURL( verifyURL )
    , _accountName( accountName )
    , _charId( charId )
    , _accountId( accountId )
    , _regionId( regionId )
    , _transactionId( transactionId )
    , _transactionBase64( transactionBase64 )
    , _exchangeURL( exchangeURL )
{
}

BuyInfo::BuyInfo( BuyInfo const& other )
    : _productId( other._productId )
    , _verifyURL( other._verifyURL )
    , _accountName( other._accountName )
    , _charId( other._charId )
    , _accountId( other._accountId )
    , _regionId( other._regionId )
    , _transactionId( other._transactionId )
    , _transactionBase64( other._transactionBase64 )
    , _exchangeURL( other._exchangeURL )
{
}

BuyInfo::BuyInfo( BuyInfo&& other )
    : _productId( std::move( other._productId ) )
    , _verifyURL( std::move( other._verifyURL ) )
    , _accountName( std::move( other._accountName ) )
    , _charId( other._charId )
    , _accountId( other._accountId )
    , _regionId( other._regionId )
    , _transactionId( std::move( other._transactionId ) )
    , _transactionBase64( std::move( other._transactionBase64 ) )
    , _exchangeURL( std::move( other._exchangeURL ) )
{
    other._charId = 0;
    other._accountId = 0;
}

BuyInfo::~BuyInfo()
{
}

BuyInfo& BuyInfo::operator=( BuyInfo const& other )
{
    _productId = other._productId;
    _verifyURL = other._verifyURL;
    _accountName = other._accountName;
    _charId = other._charId;
    _accountId = other._accountId;
    _regionId = other._regionId;
    _transactionId = other._transactionId;
    _transactionBase64 = other._transactionBase64;
    _exchangeURL = other._exchangeURL;
    return *this;
}

BuyInfo& BuyInfo::operator=( BuyInfo&& other )
{
    _productId = std::move( other._productId );
    _verifyURL = std::move( other._verifyURL );
    _accountName = std::move( other._accountName );
    _charId = other._charId;
    _accountId = other._accountId;
    _regionId = other._regionId;
    _transactionId = std::move( other._transactionId );
    _transactionBase64 = std::move( other._transactionBase64 );
    _exchangeURL = std::move( other._exchangeURL );
    other._charId = 0;
    other._accountId = 0;
    other._regionId = 0;
    return *this;
}

bool BuyInfo::operator==( BuyInfo const& other )
{
    return _productId == other._productId;
}



std::string const& BuyInfo::getProductId() const
{
    return _productId;
}

void BuyInfo::setProductId( std::string val )
{
    _productId = val;
}

std::string const& BuyInfo::getVerifyURL() const
{
    return _verifyURL;
}

void BuyInfo::setVerifyURL( std::string val )
{
    _verifyURL = val;
}

int BuyInfo::getCharId() const
{
    return _charId;
}

void BuyInfo::setCharId( int val )
{
    _charId = val;
}

int BuyInfo::getAccountId() const
{
    return _accountId;
}

void BuyInfo::setAccountId( int val )
{
    _accountId = val;
}

int BuyInfo::getRegionId() const
{
    return _regionId;
}

void BuyInfo::setRegionId( int val )
{
    _regionId = val;
}

std::string const& BuyInfo::getAccountName() const
{
    return _accountName;
}

void BuyInfo::setAccountName( std::string val )
{
    _accountName = val;
}

std::string const& BuyInfo::getTransactionId() const
{
    return _transactionId;
}

void BuyInfo::setTransactionId( std::string val )
{
    _transactionId = val;
}

std::string const& BuyInfo::getTransactionBase64() const
{
    return _transactionBase64;
}

void BuyInfo::setTransactionBase64( std::string val )
{
    _transactionBase64 = val;
}

std::string const& BuyInfo::getExchangeURL() const
{
    return _exchangeURL;
}

void BuyInfo::setExchangeURL( std::string val )
{
    _exchangeURL = val;
}

BuyInfo* BuyInfo::create()
{
    auto rtv = new (nothrow)BuyInfo();
    if( !rtv ) return nullptr;
    rtv->autorelease();
    return rtv;
}

















Product::Product()
{
}

Product::Product( std::string const& pid
                  , std::string const& title
                  , std::string const& desc
                  , std::string const& price )
                  : _pid( pid )
                  , _title( title )
                  , _desc( desc )
                  , _price( price )
{
}

Product::Product( Product const& other )
    : _pid( other._pid )
    , _title( other._title )
    , _desc( other._desc )
    , _price( other._price )
{
}

Product::Product( Product&& other )
    : _pid( std::move( other._pid ) )
    , _title( std::move( other._title ) )
    , _desc( std::move( other._desc ) )
    , _price( std::move( other._price ) )
{
}

Product::~Product()
{
}

Product& Product::operator=( Product const& other )
{
    _pid = other._pid;
    _title = other._title;
    _desc = other._desc;
    _price = other._price;
    return *this;
}

Product& Product::operator=( Product&& other )
{
    _pid = std::move( other._pid );
    _title = std::move( other._title );
    _desc = std::move( other._desc );
    _price = std::move( other._price );
    return *this;
}

bool Product::operator==( Product const& other )
{
    return _pid == other._pid;
}

std::string const& Product::getPid() const
{
    return _pid;
}

void Product::setPid( std::string val )
{
    _pid = val;
}

std::string const& Product::getTitle() const
{
    return _title;
}

void Product::setTitle( std::string val )
{
    _title = val;
}

std::string const& Product::getDesc() const
{
    return _desc;
}

void Product::setDesc( std::string val )
{
    _desc = val;
}

std::string const& Product::getPrice() const
{
    return _price;
}

void Product::setPrice( std::string val )
{
    _price = val;
}

Product* Product::create()
{
    auto rtv = new (nothrow)Product();
    if( !rtv ) return nullptr;
    rtv->autorelease();
    return rtv;
}
























Products::Products()
{
}

Products::~Products()
{
    clear();
}

int Products::size() const
{
    return (int)_data.size();
}

bool Products::empty() const
{
    return _data.empty();
}

Product* Products::at( int idx ) const
{
    if( idx < 0 || idx >= (int)_data.size() )
    {
        return nullptr;
    }
    return _data[ idx ];
}

void Products::insert( Product* p )
{
    p->retain();
    _data.push_back( p );
}

void Products::erase( std::string const& pid )
{
    auto it = std::find_if( _data.begin(), _data.end(), [ &]( Product* p )
    {
        return  p->getPid() == pid;
    } );
    if( it != _data.end() )
    {
        ( *it )->release();
        _data.erase( it );
    }
}

void Products::clear()
{
    for( auto&p : _data )
    {
        p->release();
    }
    _data.clear();
}










MAKE_INSTANCE_CPP( Purchase );


void Purchase::registerEventHandler( EventHandlerType f )
{
    _eventHandler = f;
}

void Purchase::registerEventHandler( LUA_FUNCTION f )
{
    if( f ) _eventHandler = [ f ]( GenericEvent<PurchaseEventTypes> const& e )
    {
        LuaHelper::push( (int)e.eventType );
        for( auto p : e.parameters )
        {
            if( p.isNumeric )
            {
                LuaHelper::push( p.value_numeric );
            }
            else
            {
                LuaHelper::push( p.value_string );
            }
        }
        LuaHelper::executeFunction( f, e.parameters.size() + 1 );
    };
    else _eventHandler = nullptr;
}

void Purchase::update()
{
    if( _eventHandler )
    {
        GenericEvent<PurchaseEventTypes> e;
        while( _events.pop( e ) )
        {
            _eventHandler( e );
        }
    }
}

Products* Purchase::getProducts()
{
    return &_products;
}

BuyInfo* Purchase::getBuyInfo()
{
    return &_buyInfo;
}

void Purchase::add( std::string const& id, std::string const& title, std::string const& desc, std::string const& price )
{
    _products.insert( new Product( id, title, desc, price ) );
}

void Purchase::remove( std::string const& id )
{
    _products.erase( id );
}

void Purchase::exchange( std::string const& serial )
{
    std::string s;
    s += "sn=" + serial;
    s += "&" "cid=" + Utils::toString( _buyInfo.getCharId() );
    s += "&" "rid=" + Utils::toString( _buyInfo.getRegionId() );
    s += "&" "aid=" + Utils::toString( _buyInfo.getAccountId() );
    s += "&" "an=" + _buyInfo.getAccountName();

    CCLOG( "post data = %s", s.c_str() );

    NetHttp::getInstance()->request( serial, _buyInfo.getExchangeURL(), s.c_str(), []( string const& tag, HttpRequestResults rr, int returnCode, vector<char>* returnData )
    {
        auto p = Purchase::getInstance();
        if( rr == HttpRequestResults::Fail )
        {
            p->pushEvent( GenericEvent<PurchaseEventTypes>( PurchaseEventTypes::ExchangeError
                , (int)VerifyErrorTypes::ResponseError
                , returnCode ) );
        }
        else if( returnData->size() != 1 )
        {
            p->pushEvent( GenericEvent<PurchaseEventTypes>( PurchaseEventTypes::ExchangeError
                , (int)VerifyErrorTypes::ResponseFormatError
                , std::string( returnData->data(), returnData->size() ) ) );
        }
        else
        {
            switch( returnData->at( 0 ) )
            {
            case 's':
                p->pushEvent( PurchaseEventTypes::ExchangeSuccess );
                break;
            case 'u':
                p->pushEvent( PurchaseEventTypes::ExchangeUsed );
                break;
            case 'i':
                p->pushEvent( PurchaseEventTypes::ExchangeInvalid );
                break;
            case 'e':
                p->pushEvent( PurchaseEventTypes::ExchangeElapsed );
                break;
            default:
                p->pushEvent( GenericEvent<PurchaseEventTypes>( PurchaseEventTypes::ExchangeError
                    , std::string( returnData->data(), returnData->size() ) ) );
                break;
            }
        }
    } );
}

