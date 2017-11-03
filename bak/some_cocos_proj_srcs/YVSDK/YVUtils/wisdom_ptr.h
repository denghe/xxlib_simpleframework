#ifndef  AutoPointer_h
#define  AutoPointer_h
#include <assert.h>
#include "YVAtom.h"

//智能指针的实现
namespace YVSDK
{
#define WISDOM_FREE(name, class_name)	\
	class class_name##free{ \
	public:	\
	inline static void free(name* p) { if (p) ::free(p); } }; \
	typedef  AutoPointer< name, class_name##free>	class_name;

#define WISDOM_PTR(name, class_name)	\
	class class_name##free{ \
	public:	\
	inline static void free(name* p) { if (p) delete p; } }; \
	typedef  AutoPointer< name, class_name##free>	class_name;

#define WISDOM_RELEASE(name, class_name)	\
	class class_name##free{ \
	public:	\
	inline static void free(name* p) { if (p) p->release(); } }; \
	typedef  AutoPointer< name, class_name##free>	class_name;

#define WISDOM_TPTR(name, class_name)	\
	class class_name##free{ \
	public:	\
	inline static void free(name* p) { if (p) p->clear(); } }; \
	typedef  AutoPointer< name, class_name##free>	class_name;



	template<class T>
	class wf_delete
	{
	public:
		inline static void free(T* p) { if (p) delete p; }
	};

	class wf_free
	{
	public:
		inline static void free(void* p) { if (p) free(p); }
	};

template<class Obj, class Del>
class AutoPointer
{
public:
	AutoPointer()
	{
		m_pUseCount = NULL;
		m_pObject = NULL;
		m_released = true;
	}

	AutoPointer(Obj* pObject)
	{
		if (pObject == NULL)
		{
			m_pUseCount = NULL;
			m_pObject = NULL;
			m_released = true;
		}
		else
		{
			m_pUseCount = new long(1);
			m_pObject = pObject;
			m_released = false;
		}
	}

	AutoPointer(const AutoPointer& ap)
	{
		if (ap.m_pObject == NULL)
		{
			m_pUseCount = NULL;
			m_pObject = NULL;
			m_released = true;
			return;
		}

		AtomAdd(ap.m_pUseCount, 1);
		m_pObject = ap.m_pObject;
		m_pUseCount = ap.m_pUseCount;
		m_released = false;
	}

	template<class G, class H>
	AutoPointer(const AutoPointer<G, H>& ap)
	{
		if (ap.m_pObject == NULL)
		{
			m_pUseCount = NULL;
			m_pObject = NULL;
			m_released = true;
			return;
		}

		AtomAdd(ap.m_pUseCount, 1);
		m_pObject = static_cast<Obj*>(ap.m_pObject);
		m_pUseCount = ap.m_pUseCount;
		m_released = false;
	}

	AutoPointer& operator=(const AutoPointer& ap)
	{
		if (*this == ap) return *this;
		Release();
		if (ap.m_pObject == NULL)
		{
			m_pUseCount = NULL;
			m_pObject = NULL;
			m_released = true;
			return *this;
		}

		AtomAdd(ap.m_pUseCount, 1);
		m_pObject = ap.m_pObject;
		m_pUseCount = ap.m_pUseCount;
		m_released = false;
		return *this;
	}

	AutoPointer& operator=(void *pObject)
	{
		if (*this == pObject) return *this;
		Release();
		if (NULL == pObject)
		{
			m_pUseCount = NULL;
			m_pObject = NULL;
			m_released = true;
			return *this;
		}

		m_pUseCount = new long(1);
		m_pObject = (Obj*)pObject;
		m_released = false;
		return *this;
	}

	virtual ~AutoPointer()
	{
		Release();
	}

public:

	Obj* operator->()
	{
		assert(NULL != m_pObject);
		return m_pObject;
	}

	const Obj* operator->() const
	{
		assert(NULL != m_pObject);
		return m_pObject;
	}

	Obj &operator*()
	{
		assert(NULL != m_pObject);
		return *m_pObject;
	}

	const Obj &operator*() const
	{
		assert(NULL != m_pObject);
		return *m_pObject;
	}

	bool operator==(const AutoPointer &poniter) const
	{
		if (NULL == m_pObject || NULL == poniter.m_pObject) return false;
		return m_pObject == poniter.m_pObject ? true : false;
	}

	bool operator!=(const AutoPointer &poniter) const
	{
		if (NULL == m_pObject || NULL == poniter.m_pObject) return true;
		return m_pObject != poniter.m_pObject ? true : false;
	}

	bool operator==(const void *poniter) const
	{
		//if (NULL == m_pObject || NULL == poniter) return false;
		return m_pObject == poniter ? true : false;
	}

	bool operator!=(const void *poniter) const
	{
		//if (NULL == m_pObject || NULL == poniter) return true;
		return m_pObject != poniter ? true : false;
	}

private:
	void Release()
	{
		if (m_released) return;
		m_released = true;
		if (1 == AtomDec(m_pUseCount, 1))
		{
			Del::free(m_pObject);
			delete m_pUseCount;
		}
		m_pObject = NULL;
		return;
	}

public:
	long *m_pUseCount;
	Obj *m_pObject;
	bool m_released;
};

}

#endif
