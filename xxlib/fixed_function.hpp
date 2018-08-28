// Copyright(c) 2018 zendo (734181759@qq.com) all rights reserved.
// support lambda, std::functin, std::bind, std::packaged_task, function pointer
#pragma once
#include <type_traits>
#include <functional>	// std::bad_function_call
#include <cstring>		// memcpy

namespace kapala
{
    
    template <typename Signature, size_t StorageSize = 64>
    class fixed_function;
    
    template <typename R, typename... Args, size_t StorageSize>
    class fixed_function<R(Args...), StorageSize>
    {
    public:
        using func_ptr_type = R(*)(Args...);
        using method_type = R(*)(void*, func_ptr_type, Args...);
        using alloc_type = void(*)(void*, void*);
        using storage_type = typename std::aligned_storage<StorageSize, sizeof(size_t)>::type;
    private:
        union data_type
        {
            storage_type storage_;
            func_ptr_type function_ptr_{nullptr};
        };
        data_type data_;
        method_type method_ptr_{nullptr};
        alloc_type alloc_ptr_{nullptr};
    public:
        fixed_function() = default;
        
        fixed_function(std::nullptr_t) noexcept
        {
        }
        
        fixed_function(const fixed_function& other) noexcept
        : method_ptr_(other.method_ptr_)
        , alloc_ptr_(other.alloc_ptr_)
        {
            data_.function_ptr_ = other.data_.function_ptr_;
        }
        
        template<typename Function>
        fixed_function(Function&& object) noexcept
        : fixed_function()
        {
            //typename std::remove_cv<typename std::remove_reference<Function>::type>::type;
            using unref_type = typename std::remove_reference<Function>::type;
            static_assert(sizeof(unref_type) < StorageSize, "functional object doesn't fit into internal storage");
            static_assert(std::is_move_constructible<unref_type>::value, "should be of movable type");
            
            method_ptr_ = [] (void* object_ptr, func_ptr_type, Args...args)->R {
                return (*static_cast<unref_type*>(object_ptr))(args...);
            };
            alloc_ptr_ = [] (void* storage_ptr, void* object_ptr) {
                if( object_ptr )
                {
					if constexpr (std::is_trivial<unref_type>::value)
					{
						memcpy(storage_ptr, object_ptr, sizeof(unref_type));
					}
					else
					{
						new (storage_ptr) unref_type(std::move(*static_cast<unref_type*>(object_ptr)));
					}
                }
                else
                {
                    static_cast<unref_type*>(storage_ptr)->~unref_type();
                }
            };
            alloc_ptr_(&data_.storage_, &object);
        }
        
        template<typename Ret, typename...Params>
        fixed_function(Ret(*func_ptr)(Params...)) noexcept
        : fixed_function()
        {
            data_.function_ptr_ = func_ptr;
            method_ptr_ = [] (void*, func_ptr_type f_ptr, Args...args)->R {
                return static_cast<Ret(*)(Params...)>(f_ptr)(args...);
            };
        }
        
        fixed_function(fixed_function&& other) noexcept
        : fixed_function()
        {
            move_from_other(other);
        }
        
        ~fixed_function()
        {
            if( alloc_ptr_ )
            {
                alloc_ptr_(&data_.storage_, nullptr);
            }
        }
        
        fixed_function& operator= (const fixed_function& other) noexcept
        {
            method_ptr_ = other.method_ptr_;
            alloc_ptr_ = other.alloc_ptr_;
            data_.function_ptr_ = other.data_.function_ptr_;
            return *this;
        }
        
        fixed_function& operator= (fixed_function&& other) noexcept
        {
            move_from_other(other);
            return *this;
        }
        
        template<typename Function>
        fixed_function& operator= (Function&& object) noexcept
        {
            using unref_type = typename std::remove_reference<Function>::type;
            static_assert(sizeof(unref_type) < StorageSize, "functional object doesn't fit into internal storage");
            static_assert(std::is_move_constructible<unref_type>::value, "should be of movable type");
            
            if(data_.function_ptr_)
            {
                alloc_ptr_(&data_.storage_, nullptr);
            }
            
            method_ptr_ = [] (void* object_ptr, func_ptr_type, Args...args)->R {
                return (*static_cast<unref_type*>(object_ptr))(args...);
            };
            
            alloc_ptr_ = [] (void* storage_ptr, void* object_ptr) {
                if( object_ptr )
                {
					if constexpr (std::is_trivial<unref_type>::value)
					{
						memcpy(storage_ptr, object_ptr, sizeof(unref_type));
					}
					else
					{
						new (storage_ptr) unref_type(std::move(*static_cast<unref_type*>(object_ptr)));
					}
                }
                else
                {
                    static_cast<unref_type*>(storage_ptr)->~unref_type();
                }
            };
            alloc_ptr_(&data_.storage_, &object);
            return *this;
        }
        
        fixed_function& operator= (std::nullptr_t) noexcept
        {
            if( alloc_ptr_ )
            {
                alloc_ptr_(&data_.storage_, nullptr);
            }
            alloc_ptr_ = nullptr;
            data_.function_ptr_ = nullptr;
            method_ptr_ = nullptr;
            return *this;
        }
        
		R operator()(Args...args) const
		{
			if (!method_ptr_)
			{
				throw std::bad_function_call();
			}
			return method_ptr_((void*)&data_.storage_, data_.function_ptr_, args...);
		}
        
        constexpr static size_t standard_layout_size()
        {
            return sizeof(fixed_function);
        }

		inline operator bool() const noexcept
		{
			return data_.function_ptr_ != nullptr;
		}
        
    private:
        void move_from_other(fixed_function& other) noexcept
        {
            if( this == &other )
            {
                return;
            }
            if( alloc_ptr_ )
            {
                alloc_ptr_(&data_.storage_, nullptr);
                alloc_ptr_ = nullptr;
            }
            else
            {
                data_.function_ptr_ = nullptr;
            }
            method_ptr_ = other.method_ptr_;
            other.method_ptr_ = nullptr;
            if( other.alloc_ptr_ )
            {
                alloc_ptr_ = other.alloc_ptr_;
                alloc_ptr_(&data_.storage_, &other.data_.storage_);
            }
            else
            {
                data_.function_ptr_ = other.data_.function_ptr_;
            }
        }
        
    };
    
}

/***************** example: *************
 using namespace kapala;
 struct test_func
 {
    int func(int num)
    {
        std::cout << "bind number func, args : " << num << std::endl;
        return 0;
    }
 };
 
 int func(int num)
 {
    std::cout << "func ptr, args : " << num << std::endl;
    return 0;
 }
 
 void test_fixed_function()
 {
    test_func test_f;
    kapala::fixed_function<int(int), 64> fixed_func = [] (int num) { std::cout << "lambda, args : " << num << std::endl; return 1; };
    fixed_func(11);
    fixed_func = std::bind(&test_func::func, &test_f, std::placeholders::_1);
    fixed_func(22);
    fixed_func = &func;
    fixed_func(33);
 }
 ***************************************/



