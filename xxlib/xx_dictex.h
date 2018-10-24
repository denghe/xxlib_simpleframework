#pragma once
namespace xx
{
	template<int keyIndex, typename V, typename ...KS>
	using KeyType_t = typename std::tuple_element<keyIndex, std::tuple<KS...>>::type;

	template<int keyIndex, typename V, typename ...KS>
	struct DictType
	{
		using type = Dict<KeyType_t<keyIndex, V, KS...>, int>;
	};

	template<typename V, typename ...KS>
	struct DictType<(int)0, V, KS...>
	{
		using type = Dict<KeyType_t<0, V, KS...>, V>;
	};

	template<int keyIndex, typename V, typename ...KS>
	using DictType_t = typename DictType<keyIndex, V, KS...>::type;



	template<typename V, typename ...KS>
	class DictEx;

	template<int keyIndex, typename V, typename ...KS>
	struct DictForEach
	{
		static void Init(DictEx<V, KS...>& self);
		static void RemoveAt(DictEx<V, KS...>& self, int const& idx);
		static void Clear(DictEx<V, KS...>& self);
	};

	template<typename V, typename ...KS>
	struct DictForEach<(int)0, V, KS...>
	{
		static void Init(DictEx<V, KS...>& self);
		static void RemoveAt(DictEx<V, KS...>& self, int const& idx);
		static void Clear(DictEx<V, KS...>& self);
	};



	template<typename V, typename ...KS>
	class DictEx : public Object
	{
	protected:
		static const int numKeys = (int)(sizeof...(KS));

		// dicts[0] 作为主体存储, 其他 dict 的 value 存 index ( 实际用不到 )
		std::array<Unique<Object>, numKeys> dicts;

		template<int keyIndex>
		auto DictAt()
			->Unique<DictType_t<keyIndex, V, KS...>>&
		{
			return dicts[keyIndex].template As<DictType_t<keyIndex, V, KS...>>();
		}
		template<int keyIndex>
		auto DictAt() const
			->Unique<DictType_t<keyIndex, V, KS...>> const&
		{
			return dicts[keyIndex].template As<DictType_t<keyIndex, V, KS...>>();
		}

		template<int keyIndex, typename TV, typename ...TKS>
		friend struct DictForEach;

	public:
		DictEx(MemPool* const& mp, int const& capacity = 16)
			: Object(mp)
		{
			DictForEach<numKeys - 1, V, KS...>::Init(*this);
		}

		template<int keyIndex>
		bool Exists(KeyType_t<keyIndex, V, KS...> const& key) const noexcept
		{
			return DictAt<keyIndex>()->Find(key) != -1;
		}

		template<int keyIndex>
		bool TryGetValue(KeyType_t<keyIndex, V, KS...> const& key, V& value) const noexcept
		{
			auto& dict = *DictAt<keyIndex>();
			auto index = dict.Find(key);
			if (index == -1) return false;
			value = DictAt<0>()->ValueAt(index);
			return true;
		}

		template<typename TV, typename...TKS>
		DictAddResult Add(TV&& value, TKS&&...keys) noexcept
		{
			static_assert(sizeof...(keys) == numKeys);
			return AddCore0(std::forward<TV>(value), std::forward<TKS>(keys)...);
		}

	protected:
		template<typename TV, typename TK, typename...TKS>
		DictAddResult AddCore0(TV&& value, TK&& key, TKS&&...keys) noexcept
		{
			auto& dict = *DictAt<0>();
			auto r = dict.Add(std::forward<TK>(key), std::forward<TV>(value), false);
			if (r.success)
			{
				return AddCore<1, TKS...>(r.index, std::forward<TKS>(keys)...);
			}
			return r;
		}

		template<int keyIndex, typename TK, typename...TKS>
		DictAddResult AddCore(int const& index, TK&& key, TKS&&...keys) noexcept
		{
			auto r = DictAt<keyIndex>()->Add(std::forward<TK>(key), index);
			if (r.success)
			{
				return AddCore<keyIndex + 1, TKS...>(index, std::forward<TKS>(keys)...);
			}
			DictForEach<keyIndex - 1, V, KS...>::RemoveAt(*this, index);
			return r;
		}

		template<int keyIndex, typename TK>
		DictAddResult AddCore(int const& index, TK&& key) noexcept
		{
			auto r = DictAt<keyIndex>()->Add(std::forward<TK>(key), index);
			if (r.success) return r;
			DictForEach<keyIndex - 1, V, KS...>::RemoveAt(*this, index);
			return r;
		}

	public:
		template<int keyIndex>
		int Find(KeyType_t<keyIndex, V, KS...> const& key) const noexcept
		{
			return DictAt<keyIndex>()->Find(key);
		}

		template<int keyIndex>
		bool Remove(KeyType_t<keyIndex, V, KS...> const& key) noexcept
		{
			auto index = DictAt<keyIndex>()->Find(key);
			if (index == -1) return false;
			DictForEach<numKeys - 1, V, KS...>::RemoveAt(*this, index);
			return true;
		}


		void RemoveAt(int const& index) noexcept
		{
			DictForEach<numKeys - 1, V, KS...>::RemoveAt(*this, index);
		}


		template<int keyIndex, typename TK>
		bool Update(KeyType_t<keyIndex, V, KS...> const& oldKey, TK&& newKey) noexcept
		{
			return DictAt<keyIndex>()->Update(oldKey, std::forward<TK>(newKey));
		}

		template<int keyIndex, typename TK>
		bool UpdateAt(int const& index, TK&& newKey) noexcept
		{
			return DictAt<keyIndex>()->UpdateAt(index, std::forward<TK>(newKey));
		}


		template<int keyIndex>
		KeyType_t<keyIndex, V, KS...> const& KeyAt(int const& index) const noexcept
		{
			return DictAt<keyIndex>()->KeyAt(index);
		}

		V& ValueAt(int const& index) noexcept
		{
			return DictAt<0>()->ValueAt(index);
		}
		V const& ValueAt(int const& index) const noexcept
		{
			return DictAt<0>()->ValueAt(index);
		}


		void Clear() noexcept
		{
			DictForEach<numKeys - 1, V, KS...>::Clear();
		}


		uint32_t Count() const noexcept
		{
			return DictAt<0>()->Count();
		}



		// 支持 for (decltype(auto) iv : dictex) 遍历
		// 可用 KeyAt<?>( index ) 来查 key. 
		struct IterValue
		{
			int index;
			V& value;
		};

		struct Iter
		{
			DictType_t<0, V, KS...>& dict;
			int i;
			bool operator!=(Iter const& other) noexcept
			{
				return i != other.i;
			}
			Iter& operator++() noexcept
			{
				while (++i < dict.count)
				{
					if (dict.items[i].prev != -2) break;
				}
				return *this;
			}
			IterValue operator*() { return IterValue{ i, dict.items[i].value }; }
		};
		Iter begin() noexcept
		{
			auto& dict = *DictAt<0>();
			if (dict.Empty()) return end();
			for (int i = 0; i < dict.count; ++i)
			{
				if (dict.items[i].prev != -2) return Iter{ dict, i };
			}
			return end();
		}
		Iter end() noexcept
		{
			return Iter{ *DictAt<0>(), DictAt<0>()->count };
		}

	};


	template<int keyIndex, typename V, typename ...KS>
	void DictForEach<keyIndex, V, KS...>::Init(DictEx<V, KS...>& self)
	{
		self.dicts[keyIndex] = self.mempool->template MPCreate<DictType_t<keyIndex, V, KS...>>();
		DictForEach<keyIndex - 1, V, KS...>::Init(self);
	}
	template<int keyIndex, typename V, typename ...KS>
	void DictForEach<keyIndex, V, KS...>::RemoveAt(DictEx<V, KS...>& self, int const& idx)
	{
		self.template DictAt<keyIndex>()->RemoveAt(idx);
		DictForEach<keyIndex - 1, V, KS...>::RemoveAt(self, idx);
	}
	template<int keyIndex, typename V, typename ...KS>
	void DictForEach<keyIndex, V, KS...>::Clear(DictEx<V, KS...>& self)
	{
		self.template DictAt<keyIndex>()->Clear();
		DictForEach<keyIndex - 1, V, KS...>::Clear(self);
	}

	template<typename V, typename ...KS>
	void DictForEach<(int)0, V, KS...>::Init(DictEx<V, KS...>& self)
	{
		self.dicts[0] = self.mempool->template MPCreate<DictType_t<0, V, KS...>>();
	}
	template<typename V, typename ...KS>
	void DictForEach<(int)0, V, KS...>::RemoveAt(DictEx<V, KS...>& self, int const& idx)
	{
		self.template DictAt<0>()->RemoveAt(idx);
	}
	template<typename V, typename ...KS>
	void DictForEach<(int)0, V, KS...>::Clear(DictEx<V, KS...>& self)
	{
		self.template DictAt<0>()->Clear();
	}
}
