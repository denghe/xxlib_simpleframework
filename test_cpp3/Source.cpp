#include <uv.h>
#include <assert.h>

int main(int argc, char* argv[])
{
	auto loop = uv_default_loop();

	uv_udp_t udp;
	uv_udp_init(loop, &udp);

	sockaddr_in6 addr;
	uv_ip6_addr("::", 12345, &addr);

	uv_udp_bind(&udp, (sockaddr*)&addr, UV_UDP_REUSEADDR);

	uv_udp_recv_start(&udp,
		[](uv_handle_t* handle,
			size_t suggested_size,
			uv_buf_t* buf)
	{
		buf->base = (char*)malloc(suggested_size);
		buf->len = decltype(buf->len)(suggested_size);
	},
		[](uv_udp_t* handle,
			ssize_t nread,
			const uv_buf_t* buf,
			const struct sockaddr* addr,
			unsigned flags)
	{
		if (nread > 0)
		{
			assert(addr->sa_family == AF_INET6);
			auto ip = (sockaddr_in6*)addr;
			char ipBuf[128];
			uv_ip6_name(ip, ipBuf, _countof(ipBuf));
			printf("recv [%s:%d]%s\n", ipBuf, ntohs(ip->sin6_port), buf->base);
		}
		free(buf->base);
	});

	uv_run(loop, UV_RUN_DEFAULT);
	return 0;
}
