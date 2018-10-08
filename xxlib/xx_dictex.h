#pragma once
namespace xx
{

	template<typename V, typename ...KS>
	class DictEx : public Object
	{
	protected:
		static const int numKeys = (int)(sizeof...(KS));
		struct Data
		{
			V value;
			std::array<int, numKeys> idxs;

			Data(V&& value)
				: value(std::move(value))
			{}

			// todo: 右值版 copy
		};


		using Tuple = std::tuple<KS...>;

		template<int idx>
		struct DictType
		{
			using type = Dict<typename std::tuple_element<idx, Tuple>::type, Data*>;
		};
		template<>
		struct DictType<0>
		{
			using type = Dict<typename std::tuple_element<0, Tuple>::type, Data>;
		};
		template<int idx>
		using DictType_t = typename DictType<idx>::type;


		std::array<Unique<Object>, numKeys - 1> dicts;

		template<int idx>
		void InitDict()
		{
			dicts[idx] = mempool->MPCreate<DictType_t<idx>>();
			InitDict<idx - 1>();
		}

		template<>
		void InitDict<0>()
		{
			dicts[0] = mempool->MPCreate<DictType_t<0>>();
		}


	public:
		DictEx(MemPool* const& mp, int const& capacity = 16)
			: Object(mp)
		{
			InitDict<numKeys - 1>();
		}

		template<int idx>
		bool Exists(DictType_t<idx> const& key) const noexcept
		{
			return dicts[idx].As<DictType_t<idx>>->Find(key1) != -1;
		}

		template<int idx>
		bool TryGetValue(DictType_t<idx> const& key, V& value) const noexcept
		{
			auto& dict = dicts[idx].As<DictType_t<idx>>();
			auto idx = dict->Find(key);
			if (idx == -1) return false;
			if constexpr (idx)
			{
				value = dict->ValueAt(idx)->value;
			}
			else
			{
				value = dict->ValueAt(idx).value;
			}
			return true;
		}

		template<typename TV>
		DictAddResult Add(TV&& value, KS&&...keys) noexcept
		{
			auto r0 = dicts[0].As<DictType_t<0>>()->Add(std::forward<KS>(keys), Data(std::forward<TV>(value)));
			if (!r1.success)
			{
				return r1;
			}
			auto d = &dict.ValueAt(r1.index);

			auto r2 = d2.Add(std::forward<TK2>(key2), d);
			if (!r2.success)
			{
				dict.RemoveAt(r1.index);
				return r2;
			}

			d->idx1 = r1.index;
			d->idx2 = r2.index;

			return r1;
		}


		//bool Remove1(K1 const& key1) noexcept
		//{
		//	auto idx = dict.Find(key1);
		//	if (idx == -1) return false;
		//	RemoveAt(d2.ValueAt(idx));
		//	return true;
		//}
		//bool Remove2(K2 const& key2) noexcept
		//{
		//	auto idx = d2.Find(key2);
		//	if (idx == -1) return false;
		//	RemoveAt(*d2.ValueAt(idx));
		//	return true;
		//}


		//void RemoveAt(int const& idx) noexcept
		//{
		//	RemoveAt(dict.ValueAt(idx));
		//}
		//void RemoveAt(D const& d) noexcept
		//{
		//	d2.RemoveAt(d.idx2);
		//	dict.RemoveAt(d.idx1);
		//}


		//void Clear() noexcept
		//{
		//	d2.Clear();
		//	dict.Clear();
		//}


		//uint32_t Count() const noexcept
		//{
		//	return dict.Count();
		//}

		//V& ValueAt(int const& idx) noexcept
		//{
		//	return dict.ValueAt(idx);
		//}
		//V const& ValueAt(int const& idx) const noexcept
		//{
		//	return dict.ValueAt(idx);
		//}

		// todo: for iter
	};


	// 多 key 字典之 2 个 key
	//template<typename K1, typename K2, typename K3, typename V>
	//class DictEx : public Object
	//{
	//protected:
	//	struct D
	//	{
	//		V value;
	//		int idx1;
	//		int idx2;
	//		int idx3;
	//		D(V&& value) : value(std::move(value)) {}
	//	};
	//	Dict<K1, D> dict;
	//	Dict<K2, D*> d2;
	//	Dict<K2, D*> d3;

	//public:
	//	DictEx(MemPool* const& mp, int const& capacity = 16)
	//		: Object(mp)
	//		, dict(mp, capacity)
	//		, d2(mp, capacity)
	//		, d3(mp, capacity)
	//	{}


	//	bool Exists1(K1 const& key1) const noexcept
	//	{
	//		return dict.Find(key1) != -1;
	//	}
	//	bool Exists2(K2 const& key2) const noexcept
	//	{
	//		return d2.Find(key2) != -1;
	//	}
	//	bool Exists3(K3 const& key3) const noexcept
	//	{
	//		return d3.Find(key3) != -1;
	//	}


	//	bool TryGetValue1(K1 const& key1, V& value) const noexcept
	//	{
	//		return dict.TryGetValue(key1, value);
	//	}
	//	bool TryGetValue2(K2 const& key2, V& value) const noexcept
	//	{
	//		return d2.TryGetValue(key2, value);
	//	}
	//	bool TryGetValue3(K3 const& key3, V& value) const noexcept
	//	{
	//		return d3.TryGetValue(key3, value);
	//	}


	//	template<typename TK1, typename TK2, typename TV>
	//	DictAddResult Add(TK1&& key1, TK2&& key2, TV&& value) noexcept
	//	{
	//		auto r1 = dict.Add(std::forward<TK1>(key1), D(std::forward<TV>(value)));
	//		if (!r1.success)
	//		{
	//			return r1;
	//		}
	//		auto d = &dict.ValueAt(r1.index);
	//		d->idx1 = r1.index;

	//		auto r2 = d2.Add(std::forward<TK2>(key2), d);
	//		if (!r2.success)
	//		{
	//			dict.RemoveAt(r1.index);
	//			return r2;
	//		}
	//		d->idx2 = r2.index;

	//		auto r3 = d3.Add(std::forward<TK3>(key3), d);
	//		if (!r3.success)
	//		{
	//			d2.RemoveAt(r2.index);
	//			dict.RemoveAt(r1.index);
	//			return r3;
	//		}
	//		d->idx3 = r3.index;

	//		return r1;
	//	}


	//	bool Remove1(K1 const& key1) noexcept
	//	{
	//		auto idx = dict.Find(key1);
	//		if (idx == -1) return false;
	//		RemoveAt(d2.ValueAt(idx));
	//		return true;
	//	}
	//	bool Remove2(K2 const& key2) noexcept
	//	{
	//		auto idx = d2.Find(key2);
	//		if (idx == -1) return false;
	//		RemoveAt(*d2.ValueAt(idx));
	//		return true;
	//	}
	//	bool Remove3(K3 const& key3) noexcept
	//	{
	//		auto idx = d3.Find(key3);
	//		if (idx == -1) return false;
	//		RemoveAt(*d3.ValueAt(idx));
	//		return true;
	//	}


	//	void RemoveAt(int const& idx) noexcept
	//	{
	//		RemoveAt(dict.ValueAt(idx));
	//	}
	//	void RemoveAt(D const& d) noexcept
	//	{
	//		d3.RemoveAt(d.idx3);
	//		d2.RemoveAt(d.idx2);
	//		dict.RemoveAt(d.idx1);
	//	}


	//	void Clear() noexcept
	//	{
	//		d3.Clear();
	//		d2.Clear();
	//		dict.Clear();
	//	}


	//	uint32_t Count() const noexcept
	//	{
	//		return dict.Count();
	//	}

	//	V& ValueAt(int const& idx) noexcept
	//	{
	//		return dict.ValueAt(idx);
	//	}
	//	V const& ValueAt(int const& idx) const noexcept
	//	{
	//		return dict.ValueAt(idx);
	//	}
	//};
}
