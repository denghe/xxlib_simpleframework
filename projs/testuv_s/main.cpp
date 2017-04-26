#include <iostream>
#include <uv.h>

using namespace std;
int main()
{
    auto loop = uv_default_loop(); //uv_loop_t loop; //uv_loop_init(&loop);

    uv_tcp_t server;
    uv_tcp_init(loop, &server);

    sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", 12345, &addr);

    // 结构体层次结构: uv_handle_t : uv_stream_t : uv_tcp_t

    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*)&server, 128, [](uv_stream_t* server, int status)
    {
        if (status < 0)
        {
            cout << "Listen error " << uv_strerror(status) << endl;
            return;
        }
        auto client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
        uv_tcp_init(server->loop, client);

        if (uv_accept(server, (uv_stream_t*)client) == 0)
        {
            uv_read_start((uv_stream_t*)client, [](uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
            {
                cout << "suggested_size = " << suggested_size << endl;
                buf->base = (char*)malloc(suggested_size);
                buf->len = suggested_size;
            }, [](uv_stream_t* client, ssize_t nread, const uv_buf_t* buf)
            {
                if (nread < 0)
                {
                    if (nread != UV_EOF)
                    {
                        cout << "Read error " << uv_err_name(nread) << endl;
                    }
                    uv_close((uv_handle_t*)client, nullptr);
                }
                else if (nread > 0)
                {
                    cout << "nread = " << nread << endl;
                    // todo: dump buf

                    auto req = (uv_write_t *)malloc(sizeof(uv_write_t));
                    uv_buf_t wrbuf = uv_buf_init(buf->base, nread);
                    uv_write(req, client, &wrbuf, 1, [](uv_write_t *req, int status)
                    {
                        if (status)
                        {
                            cout << "Write error \n" << uv_strerror(status) << endl;
                        }
                        free(req);

                        // todo: need uv_read_start here ??
                    });
                }
            });
        }
        else
        {
            uv_close((uv_handle_t*)client, nullptr);
        }
    });
    if (r)
    {
        cout << "Listen error " << uv_strerror(r) << endl;
        return 1;
    }

    uv_run(loop, UV_RUN_DEFAULT); //uv_loop_close(&loop);
    return 0;
}
