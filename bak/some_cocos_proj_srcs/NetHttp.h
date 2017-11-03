#ifndef _NETHTTP_H_
#define _NETHTTP_H_

class NetHttp : public Ref
{
public:
    MAKE_INSTANCE_H( NetHttp );

    typedef function<void( string const& tag, HttpRequestResults rr, int returnCode, vector<char>* returnData )> ResponseHandlerType;

    // start a Request
    bool request( string const& tag, string const& url, char const* postData, ResponseHandlerType cb );
    bool request( string const& tag, string const& url, char const* postData, LUA_FUNCTION cb );

    // cancel a Request
    void cancel( const string & tag );

private:
    NetHttp();
    ~NetHttp();

    // for HttpResponse callback
    void onResponsed( network::HttpClient* client, network::HttpResponse* response );

    unordered_map<string, ResponseHandlerType> _works;
};

#endif
