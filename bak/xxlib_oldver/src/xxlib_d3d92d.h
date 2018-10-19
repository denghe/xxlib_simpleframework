#pragma once
#include "xxlib_list.h"
#include "xxlib_charsutils.h"
#include "xxlib_bbuffer.h"
#include "xxlib_structs.h"
#define WIN32_LEAN_AND_MEAN
#include <d3d9.h>
#include <d3dx9.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <mmsystem.h>	// timeBeginPeriod at winmm.lib
#include <cassert>
#include <codecvt>

#pragma region TPJson

// 结合 ajson 实现 texture packer 的 json 输出文件的高速解析
#include <string>
#include "ajson.hpp"

namespace xxlib
{
	namespace TPJson
	{
		struct Rect
		{
			int x;
			int y;
			int w;
			int h;
		};

		struct Size
		{
			int w;
			int h;
		};

		struct Frame
		{
			std::string filename;
			Rect frame;
			bool rotated;
			bool trimmed;
			Rect spriteSourceSize;
			Size sourceSize;
		};

		struct Meta
		{
			std::string app;
			std::string version;
			std::string image;
			std::string format;
			Size size;
			std::string scale;
			std::string smartupdate;
		};

		struct Root
		{
			std::vector<Frame> frames;
			Meta meta;
		};

	}
}

AJSON(xxlib::TPJson::Rect, x, y, w, h)
AJSON(xxlib::TPJson::Size, w, h)
AJSON(xxlib::TPJson::Frame, filename, frame, rotated, trimmed, spriteSourceSize, sourceSize)
AJSON(xxlib::TPJson::Meta, app, version, image, format, size, scale, smartupdate)
AJSON(xxlib::TPJson::Root, frames, meta)

#pragma endregion

namespace xxlib
{

#pragma region Base Structs

	struct Vertex
	{
		float x, y, z;
		uint32_t c;
		float u, v;
		static const int fvf = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	};
	template<> inline uint32_t BBCalc(Vertex const &in) { return BBCalc(in.x, in.y, in.z, in.c, in.u, in.v); }
	template<> inline uint32_t BBWriteTo(char *dstBuf, Vertex const &in) { return BBWriteTo(dstBuf, in.x, in.y, in.z, in.c, in.u, in.v); }
	template<> inline int BBReadFrom(char const *srcBuf, uint32_t const &dataLen, uint32_t &offset, Vertex &out) { return BBReadFrom(srcBuf, dataLen, offset, out.x, out.y, out.z, out.c, out.u, out.v); }
	template<> inline uint32_t StrCalc(Vertex const &in) { return StrCalc(in.x, in.y, in.z, in.c, in.u, in.v) + 38; }
	template<> inline uint32_t StrWriteTo(char *dstBuf, Vertex const &in) { return StrWriteTo(dstBuf, "{ x = ", in.x, ", y = ", in.y, ", z = ", in.z, ", c = ", in.c, ", u = ", in.u, ", v = ", in.v, " }"); }

	typedef std::array<Vertex, 4> Quad;



	struct SpriteFrame
	{
		Rect<float> rect;
		std::array<Position<float>, 4> uvs;
		String* name;						// 存一份在这里备查. 因为此物必然放 Dict 的 Key 故此处不需要回收( 经由 dict 回收 )
	};

#pragma endregion

#pragma region Window

	struct Texture;
	struct Window : public MPObject
	{
		// base config
		uint32_t				w = 0, h = 0;				// 必须设宽高
		uint32_t				batchQuadCount = 4000;		// 32768;
		bool					enableZBuffer = false;
		bool					enableVSync = false;

		// window register
		HINSTANCE				hInstance = nullptr;
		HWND					hWnd = nullptr;
		WNDCLASSEX				winclass;
		wchar_t const*			className = L"XX2D__WNDCLASS";
		wchar_t const*			title = L"xx2dtest1";

		// direct 3d
		LPDIRECT3D9				pd3d = nullptr;
		LPDIRECT3DDEVICE9		pd3dd = nullptr;
		LPDIRECT3DINDEXBUFFER9  ib = nullptr;
		LPDIRECT3DVERTEXBUFFER9 vb = nullptr;

		List<Texture*>*			textures = nullptr;
		Dict<String*, Texture*, true, false>* textureDict = nullptr;	// 文件名与贴图的映射

		// direct input
		LPDIRECTINPUT8			di = nullptr;
		LPDIRECTINPUTDEVICE8	didk = nullptr;	// keyboard
		LPDIRECTINPUTDEVICE8	didm = nullptr;	// mouse

		// log helper
		Utf8Converter			utf8converter;
		String*					logHelper = nullptr;
		template<typename...Args>
		void Log(Args const&...args)
		{
			logHelper->Clear();
			logHelper->Append(args...);
			logHelper->C_str();
			OutputDebugString((wchar_t*)utf8converter.Convert(logHelper->buf));
		}

		// angle -- sin cos map
		static constexpr float pi = 3.14159f;
		inline static std::array<Position<float>, 256>& angleSinCosMap()	// 用于查表的角度对应 sin cos 的 float 值
		{
			static std::array<Position<float>, 256> data;
			return data;
		}
		inline static void StaticInit()
		{
			auto aa = pi / 128;
			for (int i = 0; i < 256; i++)
			{
				auto a = aa * i;
				angleSinCosMap()[i] = { std::cos(a), std::sin(a) };
			}
		}


		Window();
		virtual ~Window();

		inline virtual int Init()
		{
			assert(w && h);

			if (hWnd) return 0;

			if (!hInstance) hInstance = GetModuleHandle(0);
			if (!hInstance) return -1;

			winclass =
			{
				sizeof(WNDCLASSEX), CS_CLASSDC, &MsgProc, 0L, 0L,
				GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr,
				className, nullptr
			};

			if (!RegisterClassEx(&winclass)) return -1;

			xxlib::ScopeGuard sgRegisterClass = ([&]
			{
				UnregisterClass(className, hInstance);
				hInstance = nullptr;
			});

			RECT rect;
			rect.left = GetSystemMetrics(SM_CXSCREEN) / 2 - w / 2;
			rect.top = GetSystemMetrics(SM_CYSCREEN) / 2 - h / 2;
			rect.right = rect.left + w;
			rect.bottom = rect.top + h;

			LONG style = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE; //WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX;
			AdjustWindowRect(&rect, style, false);

			hWnd = CreateWindowEx(0, className, title, style,
				rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
				NULL, NULL, hInstance, NULL);

			if (!hWnd)
			{
				UnregisterClass(className, hInstance);
				hInstance = nullptr;
				return -2;
			}
			xxlib::ScopeGuard sgCreateWindow = ([&]
			{
				DestroyWindow(hWnd);
				hInstance = nullptr;
			});

			if (auto rtv = InitDI()) return rtv;
			if (auto rtv = InitD3D()) return rtv;

			ShowWindow(hWnd, SW_SHOW);
			UpdateWindow(hWnd);

			sgCreateWindow.Cancel();
			sgRegisterClass.Cancel();
			timeBeginPeriod(1);
			return 0;
		}

		inline int InitDI()
		{
			if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&di, nullptr))) return -1;

			if (FAILED(di->CreateDevice(GUID_SysKeyboard, &didk, nullptr))) return -2;
			if (FAILED(didk->SetDataFormat(&c_dfDIKeyboard))) return -3;
			if (FAILED(didk->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE))) return -4;
			if (FAILED(didk->Acquire())) return -5;

			if (FAILED(di->CreateDevice(GUID_SysMouse, &didm, nullptr))) return -6;
			if (FAILED(didm->SetDataFormat(&c_dfDIMouse))) return -7;
			if (FAILED(didm->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) return -8;
			if (FAILED(didm->Acquire())) return -9;

			return 0;
		}

		inline int InitD3D()
		{
			if (nullptr == (pd3d = Direct3DCreate9(D3D_SDK_VERSION))) return -3;

			//D3DDISPLAYMODE Mode;
			//if (FAILED(pd3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Mode)) || Mode.Format == D3DFMT_UNKNOWN) return -8;

			D3DPRESENT_PARAMETERS d3dpp;
			ZeroMemory(&d3dpp, sizeof(d3dpp));

			//d3dpp.BackBufferWidth = w;
			//d3dpp.BackBufferHeight = h;
			//d3dpp.BackBufferCount = 1;
			//d3dpp.BackBufferFormat = Mode.Format;
			//d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
			//d3dpp.hDeviceWindow = hWnd;
			d3dpp.Windowed = TRUE;
			d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
			d3dpp.PresentationInterval = enableVSync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
			if (enableZBuffer)
			{
				d3dpp.EnableAutoDepthStencil = TRUE;
				d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
			}

			if (FAILED(pd3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pd3dd))) return -4;

			if (FAILED(pd3dd->CreateVertexBuffer(batchQuadCount * sizeof(Quad),
				D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
				Vertex::fvf, D3DPOOL_DEFAULT, &vb, nullptr)))  return -5;

			if (FAILED(pd3dd->CreateIndexBuffer(batchQuadCount * 6 * 2,		// 6个顶点 2字节1个
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16, D3DPOOL_DEFAULT, &ib, nullptr))) return -6;

			uint16_t *pIndices, n = 0;
			if (FAILED(ib->Lock(0, 0, (void**)&pIndices, 0))) return -7;
			for (uint32_t i = 0; i < batchQuadCount; i++)
			{
				*pIndices++ = n;
				*pIndices++ = n + 1;
				*pIndices++ = n + 2;
				*pIndices++ = n + 2;
				*pIndices++ = n + 3;
				*pIndices++ = n;
				n += 4;
			}
			ib->Unlock();
			pd3dd->SetIndices(ib);

			pd3dd->SetFVF(Vertex::fvf);
			pd3dd->SetStreamSource(0, vb, 0, sizeof(Vertex));

			pd3dd->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			pd3dd->SetRenderState(D3DRS_LIGHTING, FALSE);
			pd3dd->SetRenderState(D3DRS_ZENABLE, enableZBuffer ? D3DZB_TRUE : D3DZB_FALSE);
			if (!enableZBuffer)
				pd3dd->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			pd3dd->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
			pd3dd->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			pd3dd->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

			pd3dd->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pd3dd->SetRenderState(D3DRS_ALPHAREF, 0x01);
			pd3dd->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

			pd3dd->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pd3dd->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pd3dd->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			pd3dd->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			pd3dd->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			pd3dd->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

			pd3dd->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
			pd3dd->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			pd3dd->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

			D3DXMATRIX matProj;
			D3DXMATRIX tmp;
			D3DXMatrixScaling(&matProj, 1.0f, -1.0f, 1.0f);
			D3DXMatrixTranslation(&tmp, -0.5f, h + 0.5f, 0.0f);
			D3DXMatrixMultiply(&matProj, &matProj, &tmp);
			D3DXMatrixOrthoOffCenterLH(&tmp, 0, (float)w, 0, (float)h, 0.0f, 1.0f);
			D3DXMatrixMultiply(&matProj, &matProj, &tmp);
			pd3dd->SetTransform(D3DTS_PROJECTION, &matProj);

			D3DXMATRIX matView;
			D3DXMatrixIdentity(&matView);
			pd3dd->SetTransform(D3DTS_VIEW, &matView);

			return 0;
		}

		inline static LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
			case WM_DESTROY:
				PostQuitMessage(0);
				return 0;
			}
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		inline int Render()
		{
			assert(pd3dd);
			if (enableZBuffer)
			{
				pd3dd->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
			}
			else
			{
				pd3dd->Clear(0, nullptr, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
			}
			if (FAILED(pd3dd->BeginScene())) return -1;
			if (auto rtv = Draw()) return rtv;
			pd3dd->EndScene();
			pd3dd->Present(nullptr, nullptr, nullptr, nullptr);
			return 0;
		}

		inline virtual int Run()
		{
			// Enter the message loop
			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					if (auto rtv = Scan()) return rtv;
					if (auto rtv = Draw()) return rtv;
					if (auto rtv = Render()) return rtv;
				}
			}
			return 0;
		}

		virtual int Draw();

		uint8_t keyboard[256];			// if (keyboard[DIK_ESCAPE] & 0x80)
		DIMOUSESTATE mouseState;		// mouse.rgbButtons[0]
		Position<int> mouse;

		inline bool IsMouseDown(int idx)
		{
			assert(idx >= 0 && idx < _countof(mouseState.rgbButtons));
			return mouseState.rgbButtons[idx] != 0;
		}
		inline bool IsKeyDown(uint8_t idx)
		{
			return (keyboard[idx] & 0x80) != 0;
		}

		inline virtual int Scan()
		{
			HRESULT hr;
			if (FAILED(hr = didk->GetDeviceState(sizeof(keyboard), (void*)&keyboard)))
			{
				if (hr == DIERR_NOTACQUIRED || hr == DIERR_INPUTLOST) didk->Acquire();
			}

			if (FAILED(hr = didm->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState)))
			{
				if (hr == DIERR_NOTACQUIRED || hr == DIERR_INPUTLOST) didm->Acquire();
			}

			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(hWnd, &pt);
			*(uint64_t*)&mouse = *(uint64_t*)&pt;

			return 0;
		}


		Texture* CreateTexture();
	};


#pragma endregion

#pragma region Sprite

	struct Sprite : public MPObject
	{
		Texture* t;
		uint32_t			idx_at_t_sprites;	// sprite 于 texture 中的下标( for 快删 )
		SpriteFrame*		sf = nullptr;		// 直接以整张贴图创建的精灵该项为空

		Position<float>		pos;				// 坐标( 默认为 0, 0 )
		Position<float>		anchor;				// 锚点 0..1( 默认为 0.5f, 0.5f )
		Size<float>			siz;				// 渲染成多大( 默认为 SpriteFrame 初始尺寸 )
		Position<float>		scale;				// 作用于 siz 之上进一步缩放( 默认为 1 )
		uint8_t				angle;				// 8bit 精度的角度( 方便高速查表 )

		bool				dirty;				// 属性变化后需要重算的标志
		Quad				quad;				// 缓存计算结果( draw 时被复制到 vb )

		Sprite(Texture* t, SpriteFrame& f);
		~Sprite();

		void		Calc();
		void		SetPos(Position<float> const& pos);
		void		SetSiz(Size<float> const& siz);
		void		SetScale(Position<float> const& scale);
		void		SetAngle(uint8_t angle);
		void		SetZ(float z);								// 0 ~ 1.0f
		float		GetZ();										// quad[0].z
		void		SetColor(Color c);							// ARGB
		Color	    GetColor();									// quad[0].c

	};

#pragma endregion

#pragma region Texture


	struct Texture : public MPObject
	{
		Window*								wnd = nullptr;
		LPDIRECT3DTEXTURE9					tex = nullptr;
		List<Sprite*>*						sprites = nullptr;
		String*								fn = nullptr;				// 文件名( 同时也被用于 dict key )
		uint32_t							idx_at_wnd_textures = -1;	// 放置于 wnd.textures 中的下标
		int									idx_at_wnd_textureMap = -1;	// 放置于 dict 中的下标
		Dict<String*, SpriteFrame, true>*	frames = nullptr;
		Size<float>							size;

		template<typename ...Args>
		Sprite* CreateSprite(Args &&... args)
		{
			return mempool().Create<Sprite>(this, std::forward<Args>(args)...);
		}

		Texture(Window* wnd)
			: wnd(wnd)
		{
			mempool().CreateTo(sprites);
			mempool().CreateTo(frames);
			idx_at_wnd_textures = wnd->textures->dataLen;	// 填索引
			wnd->textures->Add(this);
		}
		~Texture()
		{
			assert(wnd);
			assert(sprites);
			assert(frames);
			assert(!sprites->dataLen);

			frames->Release();
			frames = nullptr;

			sprites->Release();
			sprites = nullptr;

			if (tex)
			{
				tex->Release();
				tex = nullptr;

				assert(idx_at_wnd_textureMap >= 0 && wnd->textureDict->ValueAt(idx_at_wnd_textureMap) == this);
				wnd->textureDict->RemoveAt(idx_at_wnd_textureMap);
				idx_at_wnd_textureMap = -1;

				assert(wnd->textures->At(idx_at_wnd_textures) == this);
				if (wnd->textures->SwapRemoveAt(idx_at_wnd_textures))
				{
					wnd->textures->At(idx_at_wnd_textures)->idx_at_wnd_textures = idx_at_wnd_textures;	// 同步子下标
				}
			}
			if (fn)
			{
				fn->Release();
				fn = nullptr;
			}

			wnd->textures->Remove(this);
			wnd = nullptr;
		}

		// 从 texture packer 配置文件中加载贴图, 同时填充 sprite frames 字典
		inline int InitByTP(String* jsonfn)
		{
			// hold 文件名
			if (fn) fn->Release();
			fn = jsonfn;
			jsonfn->AddRef();

			// 重复请求贴图检查
			assert(wnd->textureDict->Find(jsonfn) < 0);

			TPJson::Root root;
			ajson::load_from_file(root, jsonfn->C_str());							// todo: try catch( 已知会造成 static 变量初始化看上去像泄露 )

			if (FAILED(D3DXCreateTextureFromFile(wnd->pd3dd, (wchar_t*)wnd->utf8converter.Convert(root.meta.image.c_str()), &tex))) return -2;

			size = { (float)root.meta.size.w, (float)root.meta.size.h };
			for (auto& sf : root.frames)
			{
				auto k = mempool().Create<String>(sf.filename);
				auto r = frames->Emplace(false, k);
				k->Release();
				assert(r.success);
				auto& v = frames->ValueAt(r.index);
				v.name = k;
				v.rect = { (float)sf.frame.x, (float)sf.frame.y, (float)sf.frame.w, (float)sf.frame.h };
				v.uvs[0] = { v.rect.x / size.w, v.rect.y / size.h };
				v.uvs[1] = { (v.rect.x + v.rect.w) / size.w, v.rect.y / size.h };
				v.uvs[2] = { (v.rect.x + v.rect.w) / size.w, (v.rect.y + v.rect.h) / size.h };
				v.uvs[3] = { v.rect.x / size.w, (v.rect.y + v.rect.h) / size.h };
			}

			// 放入字典并记录位置
			auto rtv = wnd->textureDict->Add(jsonfn, this);
			assert(rtv.success);
			idx_at_wnd_textureMap = rtv.index;
			return 0;
		}

		template<uint32_t len>
		inline int InitByTP(char const(&jsonfn)[len])
		{
			auto s = mempool().Create<xxlib::String>(jsonfn);
			auto rtv = InitByTP(s);
			s->Release();
			return rtv;
		}

		inline int Draw()
		{
			auto pd3dd = wnd->pd3dd;
			pd3dd->SetTexture(0, tex);
			Quad* p;
			if (FAILED(wnd->vb->Lock(0, 0, (void**)&p, D3DLOCK_DISCARD))) return -1;
			uint32_t i = 0;
			for (auto& spr : *sprites)
			{
				// todo: filter: if (spr->visible ? can't see ) continue;
				if (spr->dirty) spr->Calc();
				p[i++] = spr->quad;
				if (i == wnd->batchQuadCount)
				{
					wnd->vb->Unlock();
					pd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, i << 2, 0, i << 1);
					wnd->vb->Lock(0, 0, (void**)&p, D3DLOCK_DISCARD);
					i = 0;
				}
			}
			wnd->vb->Unlock();
			pd3dd->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, i << 2, 0, i << 1);
			//pd3dd->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, limit << 1);	// 性能似乎没差
			return 0;
		}
	};

#pragma endregion

#pragma region some impls

	inline void Sprite::Calc()
	{
		// 缩放后的尺寸
		auto sw = siz.w * scale.x;
		auto sh = siz.h * scale.y;

		// anchor 归 0 左上坐标 top
		auto tx = -anchor.x * sw;
		auto ty = -anchor.y * sh;

		if (angle)
		{
			// 逻辑上是先将 4 个点基于 anchor 为 0,0 点, 套  x * cosA - y * sinA, x * sinA + y * cosA  旋转公式, 再平移到 pos.x, y

			// anchor 归 0 右下坐标 bottom
			auto bx = tx + sw;
			auto by = ty + sh;

			// 三角函数值
			auto cosA = Window::angleSinCosMap()[angle].x;	// std::cos(angle);
			auto sinA = Window::angleSinCosMap()[angle].y;	// std::sin(angle);

			quad[0].x = tx * cosA - ty * sinA + pos.x;
			quad[0].y = tx * sinA + ty * cosA + pos.y;

			quad[1].x = bx * cosA - ty * sinA + pos.x;
			quad[1].y = bx * sinA + ty * cosA + pos.y;

			quad[2].x = bx * cosA - by * sinA + pos.x;
			quad[2].y = bx * sinA + by * cosA + pos.y;

			quad[3].x = tx * cosA - by * sinA + pos.x;
			quad[3].y = tx * sinA + by * cosA + pos.y;
		}
		else
		{
			// 左上角的点坐标
			auto x = tx + pos.x;
			auto y = ty + pos.y;
			quad[0].x = x;
			quad[0].y = y;
			// 其他点基于此点 + - 宽高即可得到
			quad[1].x = x + sw;
			quad[1].y = y;

			quad[2].x = x + sw;
			quad[2].y = y + sh;

			quad[3].x = x;
			quad[3].y = y + sh;
		}
		dirty = false;
	}

	inline void Sprite::SetPos(Position<float> const& pos)
	{
		if (this->pos.x != pos.x || this->pos.y != pos.y)
		{
			this->pos = pos;
			dirty = true;
		}
	}

	inline void Sprite::SetSiz(Size<float> const& siz)
	{
		if (this->siz.w != siz.w || this->siz.h != siz.h)
		{
			this->siz = siz;
			dirty = true;
		}
	}

	inline void Sprite::SetScale(Position<float> const& scale)
	{
		if (this->scale.x != scale.x || this->scale.y != scale.y)
		{
			this->scale = scale;
			dirty = true;
		}
	}

	inline void Sprite::SetAngle(uint8_t angle)
	{
		if (this->angle != angle)
		{
			this->angle = angle;
			dirty = true;
		}
	}

	inline void Sprite::SetZ(float z)
	{
		quad[0].z = z;
		quad[1].z = z;
		quad[2].z = z;
		quad[3].z = z;
	}

	inline float Sprite::GetZ()
	{
		return quad[0].z;
	}

	inline void Sprite::SetColor(Color c)
	{
		quad[0].c = c.data;
		quad[1].c = c.data;
		quad[2].c = c.data;
		quad[3].c = c.data;
	}

	inline Color Sprite::GetColor()
	{
		return *(Color*)&quad[0].c;
	}



	inline Sprite::Sprite(Texture* t, SpriteFrame& f)
	{
		this->t = t;
		this->sf = &f;
		t->AddRef();
		idx_at_t_sprites = t->sprites->dataLen;		// 同步子索引
		t->sprites->Add(this);

		pos = { 0, 0 };
		anchor = { 0.5f, 0.5f };
		siz = { f.rect.w, f.rect.h };
		scale = { 1.0f, 1.0f };
		angle = 0;
		dirty = true;

		quad[0] = { 0, 0, 0.5f, 0xFFFFFFFFu, f.uvs[0].x, f.uvs[0].y };
		quad[1] = { 0, 0, 0.5f, 0xFFFFFFFFu, f.uvs[1].x, f.uvs[1].y };
		quad[2] = { 0, 0, 0.5f, 0xFFFFFFFFu, f.uvs[2].x, f.uvs[2].y };
		quad[3] = { 0, 0, 0.5f, 0xFFFFFFFFu, f.uvs[3].x, f.uvs[3].y };
	}

	inline Sprite::~Sprite()
	{
		if (t)
		{
			assert(t->sprites->At(idx_at_t_sprites) == this);
			if (t->sprites->SwapRemoveAt(idx_at_t_sprites))					    // 交换删
			{
				t->sprites->At(idx_at_t_sprites)->idx_at_t_sprites = idx_at_t_sprites;	// 同步子索引
			}
			t->Release();
			t = nullptr;
		}
	}


	inline Window::Window()
	{
		mempool().CreateTo(textures);
		mempool().CreateTo(textureDict);
		mempool().CreateTo(logHelper);
	}

	inline Window::~Window()
	{
		assert(textures);
		assert(textureDict);
		assert(logHelper);

		for (auto&t : *textures)
		{
			for (auto& s : *t->sprites) s->Release();
			assert(!t->sprites->dataLen);
			t->Release();
		}
		assert(!textures->dataLen);
		assert(!textureDict->Count());
		textures->Release();
		textures = nullptr;

		textureDict->Release();
		textureDict = nullptr;

		if (vb)
		{
			vb->Release();
			vb = nullptr;
		}
		if (ib)
		{
			ib->Release();
			ib = nullptr;
		}
		if (pd3dd)
		{
			pd3dd->Release();
			pd3dd = nullptr;
		}
		if (pd3d)
		{
			pd3d->Release();
			pd3d = nullptr;
		}
		if (didk)
		{
			didk->Unacquire();
			didk->Release();
			didk = nullptr;
		}
		if (didm)
		{
			didm->Unacquire();
			didm->Release();
			didm = nullptr;
		}
		if (di)
		{
			di->Release();
			di = nullptr;
		}
		if (hWnd)
		{
			timeEndPeriod(1);
			DestroyWindow(hWnd);
			hWnd = nullptr;
		}
		UnregisterClass(className, hInstance);
		timeEndPeriod(1);

		logHelper->Release();
		logHelper = nullptr;
	}

	inline int Window::Draw()
	{
		for (auto& t : *textures)
		{
			if (auto rtv = t->Draw()) return rtv;
		}
		return 0;
	}

	inline Texture* Window::CreateTexture()
	{
		return mempool().Create<Texture>(this);
	}

#pragma endregion

}
