#include <iostream>
#include <chrono>
#include <thread>
#include <uv.h>
#include <xx_mempool.h>
#include <xx_bbqueue.h>

using namespace std;
int main()
{
    this_thread::sleep_for(chrono::milliseconds(100));

    auto loop = uv_default_loop();

    uv_tcp_t socket;
    uv_tcp_init(loop, &socket);

    sockaddr_in addr;
    uv_ip4_addr("127.0.0.1", 12345, &addr);

    uv_connect_t conn;
    uv_tcp_connect(&conn, &socket, (sockaddr*)&addr, [](uv_connect_t* conn, int status)
    {
        if (status < 0)
        {
            cout << "uv_tcp_connect error " << uv_strerror(status) << endl;
            return;
        }

		xx::MemPool mp;
		xx::BBQueue_v bbq(mp);
		xx::List_v<uv_buf_t> bufs(mp);
		auto bb = bbq->PopLastBB();
		bb->Write("abcd");
		bbq->Push(bb);
		bbq->PopTo(*bufs, 100);

        //uv_buf_t bufs[1];
        //bufs[0].len = (int)strlen(message) + 1;
        //bufs[0].base = message;

        uv_stream_t* tcp = conn->handle;

		//uv_read_start(tcp, [](uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
		//{
		//	cout << "suggested_size = " << suggested_size << endl;
		//	buf->base = (char*)malloc(suggested_size);
		//	buf->len = (ULONG)suggested_size;
		//}, [](uv_stream_t* client, ssize_t nread, const uv_buf_t* buf)
		//{
		//	if (nread < 0)
		//	{
		//		if (nread != UV_EOF)
		//		{
		//			cout << "Read error " << uv_err_name((int)nread) << endl;
		//		}
		//	}
		//	else if (nread > 0)
		//	{
		//		cout << "nread = " << nread << " content = " << buf->base << endl;
		//		// todo: dump buf
		//	}
		//	uv_close((uv_handle_t*)client, nullptr);
		//	// todo: close conn ?
		//});

        auto write_req = (uv_write_t *)malloc(sizeof(uv_write_t));
        uv_write(write_req, tcp, bufs->buf, bufs->dataLen, [](uv_write_t *write_req, int status)
        {
            if (status < 0)
            {
                cout << "uv_write error " << uv_strerror(status) << endl;
                return;
            }
            
            free(write_req);
        });
    });

    uv_run(loop, UV_RUN_DEFAULT);
    return 0;
}
