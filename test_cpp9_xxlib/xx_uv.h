#pragma once
#include "xx.h"

// todo: 从 C# 翻抄

namespace xx
{
	class UvTcpListener;

	class UvLoop
	{
	public:
		MemPool mp;
		List<Ptr<UvTcpListener>> listeners;
		void* ptr;
		UvLoop();
		~UvLoop();
		void Run(int mode);
		void Stop();
		bool alive();
	};

	class UvTcpListener : public Object
	{
	public:
		UvLoop* loop;
		size_t index_at_container;
		// peers
		void* ptr;
		void* addrPtr;
		UvTcpListener(UvLoop* loop);
		~UvTcpListener();
		static void OnAcceptCB(void* server, int status);
		void Listen(int backlog = 128);
		void Bind(char const* const& ipv4, int port);
	};
}
