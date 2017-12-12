#include <uv.h>
#include <assert.h>

int main(int argc, char* argv[])
{
	int r = 0;
	uv_loop_t loop;
	uv_timer_t timer;

	r = uv_loop_init(&loop);
	assert(!r);

	r = uv_timer_init(&loop, &timer);
	assert(!r);
	
	r = uv_timer_start(&timer, [](uv_timer_t* handle)
	{
		uv_stop(handle->loop);
	}
	, 1000, 1000);
	assert(!r);

	r = uv_run(&loop, UV_RUN_DEFAULT);
	assert(r == 1);

	r = uv_loop_close(&loop);
	assert(r == -4082);

	uv_close((uv_handle_t*)&timer, nullptr);

	r = uv_run(&loop, UV_RUN_DEFAULT);
	assert(!r);

	r = uv_loop_close(&loop);
	assert(!r);

	return r;
}
