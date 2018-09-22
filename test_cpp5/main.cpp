#pragma execution_character_set("utf-8")
//#include "../pkg/PKG_class.h"
#include "uv.h"

// todo: 封 DNS 解析到 xxuv

void on_resolved(uv_getaddrinfo_t *resolver, int status, struct addrinfo *res)
{
	if (status == -1) {
		fprintf(stderr, "getaddrinfo callback error\n");
		return;
	}

	auto ai = res;
	do
	{
		char addr[17] = { '\0' };
		uv_ip4_name((struct sockaddr_in*) ai->ai_addr, addr, 16);
		fprintf(stderr, "%s\n", addr);
		ai = ai->ai_next;
	} while (ai);

	//uv_connect_t *connect_req = (uv_connect_t*)malloc(sizeof(uv_connect_t));
	//uv_tcp_t *socket = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
	//uv_tcp_init(loop, socket);

	//connect_req->data = (void*)socket;
	//uv_tcp_connect(connect_req, socket, *(struct sockaddr_in*) res->ai_addr, on_connect);

	uv_freeaddrinfo(res);
}

int main(int argc, char const* const* argv)
{
	auto loop = uv_default_loop();

	struct addrinfo hints;
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;// IPPROTO_TCP;
	hints.ai_flags = 0;

	uv_getaddrinfo_t resolver;
	int r = uv_getaddrinfo(loop, &resolver, on_resolved, "www.baidu.com", "80", &hints);

	if (r)
	{
		fprintf(stderr, "getaddrinfo call error\n");
		return 1;
	}
	return uv_run(loop, UV_RUN_DEFAULT);
}
