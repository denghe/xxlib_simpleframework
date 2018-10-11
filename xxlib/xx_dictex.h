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

			template<typename TV>
			Data(TV&& value)
				: value(std::forward<TV>(value))
			{}

			Data(Data&&) = default;
			Data(Data const&) = delete;
		};


		using Tuple = std::tuple<KS...>;

		template<int idx>
		using KeyTypeAt = typename std::tuple_element<idx, Tuple>::type;

		template<int idx>
		struct DictType
		{
			using type = Dict<KeyTypeAt<idx>, Data*>;
		};
		template<>
		struct DictType<(int)0>
		{
			using type = Dict<KeyTypeAt<0>, Data>;
		};
		template<int idx>
		using DictType_t = typename DictType<idx>::type;


		std::array<Unique<Object>, numKeys> dicts;

		template<int idx>
		auto DictAt()->Unique<DictType_t<idx>>&
		{
			return dicts[idx].As<DictType_t<idx>>();
		}


		template<int idx>
		struct DictForEach;
		friend DictForEach;

		template<int idx>
		struct DictForEach
		{
			static void Init(DictEx& self)
			{
				self.dicts[idx] = self.mempool->MPCreate<DictType_t<idx>>();
				DictForEach<idx - 1>::Init(self);
			}
			static void RemoveAt(DictEx& self, Data* const& d)
			{
				self.DictAt<idx>()->RemoveAt(d->idxs[idx]);
				DictForEach<idx - 1>::RemoveAt(self, d);
			}
			static void Clear(DictEx& self)
			{
				self.DictAt<idx>()->Clear();
				DictForEach<idx - 1>::Clear(self);
			}
		};
		template<>
		struct DictForEach<(int)0>
		{
			static void Init(DictEx& self)
			{
				self.dicts[0] = self.mempool->MPCreate<DictType_t<0>>();
			}
			static void RemoveAt(DictEx& self, Data* const& d)
			{
				self.DictAt<0>()->RemoveAt(d->idxs[0]);
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
			std::cout << numKeys << std::endl;
			DictForEach<numKeys - 1>::Init(*this);
		}

		template<int idx>
		bool Exists(DictType_t<idx> const& key) const noexcept
		{
			return DictAt<idx>()->Find(key1) != -1;
		}

		template<int idx>
		bool TryGetValue(DictType_t<idx> const& key, V& value) const noexcept
		{
			auto& dict = DictAt<idx>();
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

		template<typename TV, typename...TKS>
		DictAddResult Add(TV&& value, TKS&&...keys) noexcept
		{
			static_assert(sizeof...(keys) == numKeys);
			return AddCore0(std::forward<TV>(value), std::forward<TKS>(keys)...);
		}

		template<typename TV, typename TK, typename...TKS>
		DictAddResult AddCore0(TV&& value, TK&& key, TKS&&...keys) noexcept
		{
			auto& dict = DictAt<0>();
			auto r = dict->Add(std::forward<TK>(key), Data(std::forward<TV>(value)), false);
			if (!r.success)
			{
				return r;
			}
			auto d = &dict->ValueAt(r.index);
			d->idxs[0] = r.index;

			if (!AddCore<1, TKS...>(d, std::forward<TKS>(keys)...))
			{
				r.index = -1;
				r.success = false;
			}
			return r;
		}

		template<int idx, typename TK, typename...TKS>
		bool AddCore(Data* const& d, TK&& key, TKS&&...keys) noexcept
		{
			auto r = DictAt<idx>()->Add(std::forward<TKS>(key), d);
			if (!r.success)
			{
				DictForEach<idx - 1>::RemoveAt(*this, d);
				return false;
			}
			d->idxs[idx] = r.index;
			return AddCore<idx + 1, TKS...>(d, std::forward<TKS>(keys)...);
		}

		template<int idx, typename TK>
		bool AddCore(Data* const& d, TK&& key)
		{
			auto r = DictAt<idx>()->Add(std::forward<TK>(key), d);
			if (!r.success)
			{
				DictForEach<idx - 1>::RemoveAt(*this, d);
				return false;
			}
			d->idxs[idx] = r.index;
			return true;
		}


		template<int idx>
		bool Remove(KeyTypeAt<idx> const& key) noexcept
		{
			auto& dict = DictAt<idx>();
			auto idx = dict->Find(key);
			if (idx == -1) return false;
			if constexpr (idx)
			{
				DictForEach<numKeys - 1>::RemoveAt(*this, *dict->ValueAt(idx));
			}
			else
			{
				DictForEach<numKeys - 1>::RemoveAt(*this, dict->ValueAt(idx));
			}
			return true;
		}


		void RemoveAt(int const& idx) noexcept
		{
			DictForEach<numKeys - 1>::RemoveAt(*this, DictAt<0>()->ValueAt(idx));
		}


		V& ValueAt(int const& idx) noexcept
		{
			return DictAt<0>()->ValueAt(idx).value;
		}
		V const& ValueAt(int const& idx) const noexcept
		{
			return DictAt<0>()->ValueAt(idx).value;
		}


		void Clear() noexcept
		{
			DictForEach<numKeys - 1>::Clear();
		}


		uint32_t Count() const noexcept
		{
			return DictAt<0>()->Count();
		}

		// todo: for iter
	};
}
