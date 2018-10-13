#pragma once
namespace xx
{
	template<typename V, typename ...KS>
	class DictEx : public Object
	{
	protected:
		static const int numKeys = (int)(sizeof...(KS));

		using Tuple = std::tuple<KS...>;

		template<int keyIndex>
		using KeyType_t = typename std::tuple_element<keyIndex, Tuple>::type;

		template<int keyIndex>
		struct DictType
		{
			using type = Dict<KeyType_t<keyIndex>, int>;
		};
		template<>
		struct DictType<(int)0>
		{
			using type = Dict<KeyType_t<0>, V>;
		};
		template<int keyIndex>
		using DictType_t = typename DictType<keyIndex>::type;


		// dicts[0] 作为主体存储, 其他 dict 的 value 存 index ( 实际用不到 )
		std::array<Unique<Object>, numKeys> dicts;

		template<int keyIndex>
		auto DictAt()
			->Unique<DictType_t<keyIndex>>&
		{
			return dicts[keyIndex].As<DictType_t<keyIndex>>();
		}
		template<int keyIndex>
		auto DictAt() const
			->Unique<DictType_t<keyIndex>> const&
		{
			return dicts[keyIndex].As<DictType_t<keyIndex>>();
		}


		template<int keyIndex>
		struct DictForEach
		{
			static void Init(DictEx& self)
			{
				self.dicts[keyIndex] = self.mempool->MPCreate<DictType_t<keyIndex>>();
				DictForEach<keyIndex - 1>::Init(self);
			}
			static void RemoveAt(DictEx& self, int const& idx)
			{
				self.DictAt<keyIndex>()->RemoveAt(idx);
				DictForEach<keyIndex - 1>::RemoveAt(self, idx);
			}
			static void Clear(DictEx& self)
			{
				self.DictAt<keyIndex>()->Clear();
				DictForEach<keyIndex - 1>::Clear(self);
			}
		};

		template<>
		struct DictForEach<(int)0>
		{
			static void Init(DictEx& self)
			{
				self.dicts[0] = self.mempool->MPCreate<DictType_t<0>>();
			}
			static void RemoveAt(DictEx& self, int const& idx)
			{
				self.DictAt<0>()->RemoveAt(idx);
			}
			static void Clear(DictEx& self)
			{
				self.DictAt<0>()->Clear();
			}
		};


	public:
		DictEx(MemPool* const& mp, int const& capacity = 16)
			: Object(mp)
		{
			DictForEach<numKeys - 1>::Init(*this);
		}

		template<int keyIndex>
		bool Exists(KeyType_t<keyIndex> const& key) const noexcept
		{
			return DictAt<keyIndex>()->Find(key1) != -1;
		}

		template<int keyIndex>
		bool TryGetValue(KeyType_t<keyIndex> const& key, V& value) const noexcept
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
			DictForEach<keyIndex - 1>::RemoveAt(*this, index);
			return r;
		}

		template<int keyIndex, typename TK>
		DictAddResult AddCore(int const& index, TK&& key) noexcept
		{
			auto r = DictAt<keyIndex>()->Add(std::forward<TK>(key), index);
			if (r.success) return r;
			DictForEach<keyIndex - 1>::RemoveAt(*this, index);
			return r;
		}

	public:
		template<int keyIndex>
		int Find(KeyType_t<keyIndex> const& key) const noexcept
		{
			return DictAt<keyIndex>()->Find(key);
		}

		template<int keyIndex>
		bool Remove(KeyType_t<keyIndex> const& key) noexcept
		{
			auto index = DictAt<keyIndex>()->Find(key);
			if (index == -1) return false;
			DictForEach<numKeys - 1>::RemoveAt(*this, index);
			return true;
		}


		void RemoveAt(int const& index) noexcept
		{
			DictForEach<numKeys - 1>::RemoveAt(*this, index);
		}


		template<int keyIndex, typename TK>
		bool Update(KeyType_t<keyIndex> const& oldKey, TK&& newKey) noexcept
		{
			return DictAt<keyIndex>()->Update(oldKey, std::forward<TK>(newKey));
		}

		template<int keyIndex, typename TK>
		bool UpdateAt(int const& index, TK&& newKey) noexcept
		{
			return DictAt<keyIndex>()->UpdateAt(index, std::forward<TK>(newKey));
		}


		template<int keyIndex>
		KeyType_t<keyIndex> const& KeyAt(int const& index) const noexcept
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
			DictForEach<numKeys - 1>::Clear();
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
			DictType_t<0>& dict;
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
}



//#pragma once
//namespace xx
//{
//	// todo: 看上去多个dict的下标是一致的. 可换用 hashset. hashset 需要改造一下以符合使用条件
//
//	template<typename V, typename ...KS>
//	class DictEx : public Object
//	{
//	protected:
//		static const int numKeys = (int)(sizeof...(KS));
//
//		// 值容器, 附加 *Data 位于各个 dict 中的 index
//		struct Data
//		{
//			V value;
//			std::array<int, numKeys> indexs;
//
//			template<typename TV>
//			Data(TV&& value)
//				: value(std::forward<TV>(value))
//			{}
//
//			Data(Data&&) = default;
//			Data(Data const&) = delete;
//		};
//
//
//		using Tuple = std::tuple<KS...>;
//
//		template<int keyIndex>
//		using KeyType_t = typename std::tuple_element<keyIndex, Tuple>::type;
//
//		template<int keyIndex>
//		struct DictType
//		{
//			using type = Dict<KeyType_t<keyIndex>, int>;
//		};
//		template<>
//		struct DictType<(int)0>
//		{
//			using type = Dict<KeyType_t<0>, Data>;
//		};
//		template<int keyIndex>
//		using DictType_t = typename DictType<keyIndex>::type;
//
//
//		// dicts[0] 作为主体存储, 存 Data. 其他存 dicts[0] 的下标
//		std::array<Unique<Object>, numKeys> dicts;
//
//		template<int keyIndex>
//		auto DictAt()
//			->Unique<DictType_t<keyIndex>>&
//		{
//			return dicts[keyIndex].As<DictType_t<keyIndex>>();
//		}
//		template<int keyIndex>
//		auto DictAt() const
//			->Unique<DictType_t<keyIndex>> const&
//		{
//			return dicts[keyIndex].As<DictType_t<keyIndex>>();
//		}
//
//
//		template<int keyIndex>
//		struct DictForEach
//		{
//			static void Init(DictEx& self)
//			{
//				self.dicts[keyIndex] = self.mempool->MPCreate<DictType_t<keyIndex>>();
//				DictForEach<keyIndex - 1>::Init(self);
//			}
//			static void RemoveAt(DictEx& self, Data* const& d)
//			{
//				self.DictAt<keyIndex>()->RemoveAt(d->indexs[keyIndex]);
//				DictForEach<keyIndex - 1>::RemoveAt(self, d);
//			}
//			static void Clear(DictEx& self)
//			{
//				self.DictAt<keyIndex>()->Clear();
//				DictForEach<keyIndex - 1>::Clear(self);
//			}
//		};
//
//		template<>
//		struct DictForEach<(int)0>
//		{
//			static void Init(DictEx& self)
//			{
//				self.dicts[0] = self.mempool->MPCreate<DictType_t<0>>();
//			}
//			static void RemoveAt(DictEx& self, Data* const& d)
//			{
//				self.DictAt<0>()->RemoveAt(d->indexs[0]);
//			}
//			static void Clear(DictEx& self)
//			{
//				self.DictAt<0>()->Clear();
//			}
//		};
//
//
//	public:
//		DictEx(MemPool* const& mp, int const& capacity = 16)
//			: Object(mp)
//		{
//			DictForEach<numKeys - 1>::Init(*this);
//		}
//
//		template<int keyIndex>
//		bool Exists(KeyType_t<keyIndex> const& key) const noexcept
//		{
//			return DictAt<keyIndex>()->Find(key1) != -1;
//		}
//
//		template<int keyIndex>
//		bool TryGetValue(KeyType_t<keyIndex> const& key, V& value) const noexcept
//		{
//			auto& dict = *DictAt<keyIndex>();
//			auto index = dict.Find(key);
//			if (index == -1) return false;
//			if constexpr (index)
//			{
//				value = DictAt<0>()->ValueAt(dict.ValueAt(index)).value;
//			}
//			else
//			{
//				value = dict.ValueAt(index).value;
//			}
//			return true;
//		}
//
//		template<typename TV, typename...TKS>
//		DictAddResult Add(TV&& value, TKS&&...keys) noexcept
//		{
//			static_assert(sizeof...(keys) == numKeys);
//			return AddCore0(std::forward<TV>(value), std::forward<TKS>(keys)...);
//		}
//
//	protected:
//		template<typename TV, typename TK, typename...TKS>
//		DictAddResult AddCore0(TV&& value, TK&& key, TKS&&...keys) noexcept
//		{
//			auto& dict = *DictAt<0>();
//			auto r = dict.Add(std::forward<TK>(key), Data(std::forward<TV>(value)), false);
//			if (!r.success)
//			{
//				return r;
//			}
//			auto d = &dict.ValueAt(r.index);
//			d->indexs[0] = r.index;
//
//			if (!AddCore<1, TKS...>(d, std::forward<TKS>(keys)...))
//			{
//				r.index = -1;
//				r.success = false;
//			}
//			return r;
//		}
//
//		template<int keyIndex, typename TK, typename...TKS>
//		bool AddCore(Data* const& d, TK&& key, TKS&&...keys) noexcept
//		{
//			auto r = DictAt<keyIndex>()->Add(std::forward<KS>(key), d->indexs[0]);
//			if (!r.success)
//			{
//				DictForEach<keyIndex - 1>::RemoveAt(*this, d);
//				return false;
//			}
//			d->indexs[keyIndex] = r.index;
//			return AddCore<keyIndex + 1, TKS...>(d, std::forward<TKS>(keys)...);
//		}
//
//		template<int keyIndex, typename TK>
//		bool AddCore(Data* const& d, TK&& key) noexcept
//		{
//			auto r = DictAt<keyIndex>()->Add(std::forward<TK>(key), d->indexs[0]);
//			if (!r.success)
//			{
//				DictForEach<keyIndex - 1>::RemoveAt(*this, d);
//				return false;
//			}
//			d->indexs[keyIndex] = r.index;
//			return true;
//		}
//
//	public:
//		template<int keyIndex>
//		int Find(KeyType_t<keyIndex> const& key) const noexcept
//		{
//			auto& dict = *DictAt<keyIndex>();
//			if constexpr (keyIndex)
//			{
//				int r = dict.Find(key);
//				if (r == -1) return -1;
//				return dict.ValueAt(r);
//			}
//			else
//			{
//				return dict.Find(key);
//			}
//		}
//
//		template<int keyIndex>
//		bool Remove(KeyType_t<keyIndex> const& key) noexcept
//		{
//			auto& dict = *DictAt<keyIndex>();
//			auto index = dict.Find(key);
//			if (index == -1) return false;
//			if constexpr (keyIndex)
//			{
//				DictForEach<numKeys - 1>::RemoveAt(*this, &DictAt<0>()->ValueAt(dict.ValueAt(index)));
//			}
//			else
//			{
//				DictForEach<numKeys - 1>::RemoveAt(*this, &dict.ValueAt(index));
//			}
//			return true;
//		}
//
//
//		void RemoveAt(int const& index) noexcept
//		{
//			DictForEach<numKeys - 1>::RemoveAt(*this, &DictAt<0>()->ValueAt(index));
//		}
//
//
//		template<int keyIndex, typename TK>
//		bool Update(KeyType_t<keyIndex> const& oldKey, TK&& newKey) noexcept
//		{
//			return DictAt<keyIndex>()->Update(oldKey, std::forward<TK>(newKey));
//		}
//
//		template<int keyIndex, typename TK>
//		bool UpdateAt(int const& index, TK&& newKey) noexcept
//		{
//			return DictAt<keyIndex>()->UpdateAt(index, std::forward<TK>(newKey));
//		}
//
//
//		template<int keyIndex>
//		KeyType_t<keyIndex> const& KeyAt(int const& index) const noexcept
//		{
//			return DictAt<keyIndex>()->KeyAt(index);
//		}
//
//		V& ValueAt(int const& index) noexcept
//		{
//			return DictAt<0>()->ValueAt(index).value;
//		}
//		V const& ValueAt(int const& index) const noexcept
//		{
//			return DictAt<0>()->ValueAt(index).value;
//		}
//
//
//		void Clear() noexcept
//		{
//			DictForEach<numKeys - 1>::Clear();
//		}
//
//
//		uint32_t Count() const noexcept
//		{
//			return DictAt<0>()->Count();
//		}
//
//
//
//		// 支持 for (decltype(auto) iv : dictex) 遍历
//		// 可用 KeyAt<?>( IterValue.index ) 来查 key. 
//		struct IterValue
//		{
//			int index;
//			V* valuePtr;
//			V& Value()
//			{
//				return *valuePtr;
//			}
//		};
//
//		struct Iter
//		{
//			DictType_t<0>& dict;
//			int i;
//			bool operator!=(Iter const& other) noexcept
//			{
//				return i != other.i;
//			}
//			Iter& operator++() noexcept
//			{
//				while (++i < dict.count)
//				{
//					if (dict.items[i].prev != -2) break;
//				}
//				return *this;
//			}
//			IterValue operator*() { return IterValue{ i, &dict.items[i].value.value }; }
//		};
//		Iter begin() noexcept
//		{
//			auto& dict = *DictAt<0>();
//			if (dict.Empty()) return end();
//			for (int i = 0; i < dict.count; ++i)
//			{
//				if (dict.items[i].prev != -2) return Iter{ dict, i };
//			}
//			return end();
//		}
//		Iter end() noexcept
//		{
//			return Iter{ *DictAt<0>(), DictAt<0>()->count };
//		}
//
//	};
//}
