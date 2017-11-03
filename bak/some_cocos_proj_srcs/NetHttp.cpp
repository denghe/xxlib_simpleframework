#include "Precompile.h"
#include "NetHttp.h"

MAKE_INSTANCE_CPP( NetHttp );

NetHttp::NetHttp()
{
    auto hc = network::HttpClient::getInstance();
    hc->send( nullptr );
    sleepMS( 0 );                       // for what ?
    hc->setTimeoutForConnect( 10 );
    hc->setTimeoutForRead( 10 );
}

NetHttp::~NetHttp()
{
    network::HttpClient::destroyInstance();
}

// 发起一个下载任务，以 tag 为 key, 往 url post 数据并保存下载的文本。函数返回 false 表示 tag 重复
// 当下载完成　或　超时　后，将删除本次下载任务，并向 lua 抛事件
bool NetHttp::request( string const& tag, string const& url, char const* postData, ResponseHandlerType cb )
{
    log("HttpRequest: %s", url.c_str());

    if( _works.find( tag ) != _works.end() ) return false;
    _works.insert( make_pair( tag, cb ) );
    auto request = new network::HttpRequest();
    request->setUrl( url.c_str() );
    request->setRequestType( postData ? network::HttpRequest::Type::POST : network::HttpRequest::Type::GET );
    request->setResponseCallback( [ this ]( network::HttpClient* client, network::HttpResponse* response ) { onResponsed( client, response ); } );
    if( postData )request->setRequestData( postData, strlen( postData ) );
    request->setTag( tag.c_str() );
    network::HttpClient::getInstance()->send( request );
    request->release();
    return true;
}
bool NetHttp::request( string const& tag, string const& url, char const* postData, LUA_FUNCTION cb )
{
    return request( tag, url, postData, [ cb ]( string const& tag, HttpRequestResults rr, int returnCode, vector<char>* returnData )
    {
        if( cb == 0 ) return;
        LuaHelper::push( (int)rr );
        LuaHelper::push( tag );
        if( rr == HttpRequestResults::Success )
        {
            LuaHelper::push( string( returnData->data(), returnData->size() ) );
        }
        else
        {
            LuaHelper::push( Utils::toString( returnCode ) );
        }
        LuaHelper::executeFunction( cb, 3 );
    } );
}

void NetHttp::onResponsed( network::HttpClient* client, network::HttpResponse* response )
{
    if( !response ) return;
    int  n = (int)response->getResponseCode();   // 200, 404, ...
    auto req = response->getHttpRequest();
    auto tag = std::string( req->getTag() );
    auto it = _works.find( tag );
    if( it == _works.end() ) return;
    auto f = it->second;
    _works.erase( it );
    if( response->isSucceed() )
    {
        f( tag, HttpRequestResults::Success, n, response->getResponseData() );
    }
    else
    {
        f( tag, HttpRequestResults::Fail, n, nullptr );
    }
}

void NetHttp::cancel( const string & tag )
{
    auto it = _works.find( tag );
    if( it == _works.end() ) return;
    _works.erase( it );
}
