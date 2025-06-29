#pragma once
#ifndef Convention_Runtime_Architecture_hpp
#define Convention_Runtime_Architecture_hpp

#include"Config.hpp"

namespace Convention
{
	// Interface

	struct ISignal
	{
		virtual ~ISignal() {}
	};

	struct IModel
	{
		virtual std::string Save() abstract;
		virtual void Load(std::string_view data) abstract;
	};

	template<typename T>
	struct IConvertable
	{
		virtual T ConvertTo() abstract;
	};

	template<typename T>
	struct IConvertModel
		: public IModel, public IConvertable<T>
	{
		virtual ~IConvertModel() {}
	};

	// Instance

	template<typename T>
	class SingletonModel
		: public IModel
	{
	private:
		static T& InjectInstance()
		{
			static T instance;
			return instance;
		}
	public:
		static T& Instance()
		{
			return InjectInstance();
		}

		virtual std::string Save() override
		{
			if constexpr (std::is_base_of_v<IModel, T>)
			{
				return Instance().Save();
			}
			throw std::runtime_error("InvalidOperationException");
		}

		virtual void Load(std::string_view data) override
		{
			if constexpr (std::is_base_of_v<IModel, T>)
			{
				Instance().Load(data);
			}
			throw std::runtime_error("InvalidOperationException");
		}

		operator T& ()
		{
			return Instance();
		}
	};

	template<template<typename> class ElementPtr = std::shared_ptr>
	class DependenceModel
		: public IConvertModel<bool>
	{
	private:
		std::vector<ElementPtr<IConvertable<bool>>> queries;
	public:
		DependenceModel(std::vector<ElementPtr<IConvertable<bool>>> queries) :__init(queries) {}

		bool ConvertTo()
		{
			for (auto&& query : queries)
			{
				if ((*query).ConvertTo() == false)
					return false;
			}
			return true;
		}

		decltype(auto) begin() noexcept
		{
			return queries.begin();
		}

		decltype(auto) end() noexcept
		{
			return queries.end();
		}

		virtual void Load(std::string_view data) override
		{
			throw std::runtime_error("NotImplementedException");
		}

		virtual string Save() override
		{
			throw std::runtime_error("NotImplementedException");
		}
	};

	template<>
	class DependenceModel<ElementTuple>
		: public IConvertModel<bool>
	{
	private:
		std::vector<IConvertable<bool>> queries;
	public:
		DependenceModel(std::vector<IConvertable<bool>> queries) :__init(queries) {}

		bool ConvertTo()
		{
			for (auto&& query : queries)
			{
				if (query.ConvertTo() == false)
					return false;
			}
			return true;
		}

		decltype(auto) begin() noexcept
		{
			return queries.begin();
		}

		decltype(auto) end() noexcept
		{
			return queries.end();
		}

		virtual void Load(std::string_view data) override
		{
			throw std::runtime_error("NotImplementedException");
		}

		virtual string Save() override
		{
			throw std::runtime_error("NotImplementedException");
		}
	};

	class Architecture
	{
		using Type = const type_info&;
		using TypeID = size_t;
	public:
		void InternalReset()
		{
			// Register System
			RegisterHistory.clear();
			UncompleteTargets.clear();
			Completer.clear();
			Dependences.clear();
			Childs.clear();
			// Event Listener
			SignalListener.clear();
			// Linear Chain for Dependence
			TimelineQuenes.clear();
			TimelineContentID = 0;
		}

#pragma region Objects Registered

	private:
		std::set<TypeID> RegisterHistory;
		std::map<TypeID, void*> UncompleteTargets;
		std::map<TypeID, std::function<void()>> Completer;
		std::map<TypeID, DependenceModel<>> Dependences;
		std::map<TypeID, void*> Childs;

		class TypeQuery
			: public IConvertable<bool>
		{
		private:
			TypeID queryType;

		public:
			TypeQuery(TypeID queryType) :__init(queryType) {}
			TypeQuery(Type queryType) :queryType(queryType.hash_code()) {}

			virtual bool ConvertTo() override
			{
				return SingletonModel<Architecture>::Instance().Childs.count(queryType);
			}
		};

	public:

		class Registering
			: public IConvertable<bool>
		{
		private:
			TypeID registerSlot;
		public:
			Registering(TypeID registerSlot) :__init(registerSlot) {}
			Registering(Type registerSlot) :registerSlot(registerSlot.hash_code()) {}

			virtual bool ConvertTo() override
			{
				return SingletonModel<Architecture>::Instance().Childs.count(registerSlot);
			}
		};

	private:

		bool InternalRegisteringComplete(_Out_ std::set<TypeID>& InternalUpdateBuffer)
		{
			bool result = false;
			for (auto&& dependence : Dependences)
			{
				if (dependence.second.ConvertTo())
				{
					InternalUpdateBuffer.insert(dependence.first);
					result = true;
				}
			}
			return result;
		}

		void InternalRegisteringUpdate(_In_ std::set<TypeID>& InternalUpdateBuffer)
		{
			for (auto&& complete : InternalUpdateBuffer)
			{
				Dependences.erase(complete);
			}
			for (auto&& complete : InternalUpdateBuffer)
			{
				Completer[complete]();
				Completer.erase(complete);
			}
			for (auto&& complete : InternalUpdateBuffer)
			{
				Childs[complete] = UncompleteTargets[complete];
				UncompleteTargets.erase(complete);
			}
		}

	public:
		Registering Register(Type slot, void* target, std::function<void()> completer, std::vector<Type> dependences)
		{
			if (RegisterHistory.count(slot.hash_code()))
			{
				throw std::runtime_error("Illegal duplicate registrations");
			}
			RegisterHistory.insert(slot.hash_code());
			Completer[slot.hash_code()] = completer;
			UncompleteTargets[slot.hash_code()] = target;
			std::vector<std::shared_ptr<IConvertable<bool>>> dependenceModel;
			for (auto&& type : dependences)
			{
				dependenceModel.push_back(std::make_shared<TypeQuery>(new TypeQuery(slot)));
			}
			Dependences[slot.hash_code()] = DependenceModel<>(dependenceModel);
			std::set<TypeID> buffer;
			while (InternalRegisteringComplete(buffer))
				InternalRegisteringUpdate(buffer);
			return Registering(slot);
		}

		template<typename T>
		Registering Register(T* target, std::function<void()> completer, std::vector<Type> dependences)
		{
			return Register(typeid(T), target, completer, dependences);
		}

		bool Contains(Type type) const noexcept
		{
			return Childs.count(type.hash_code());
		}

		template<typename T>
		bool Contains() const noexcept
		{
			return Contains(typeid(T));
		}

		void* InternalGet(Type type) const
		{
			return Childs.at(type.hash_code());
		}

		void* Get(Type type) const
		{
			return InternalGet(type);
		}

		template<typename T>
		T* Get()
		{
			return reinterpret_cast<T*>(Get(typeid(T)));
		}

#pragma endregion

#pragma region Signal & Update

	private:
		std::map<TypeID, std::set<std::function<void(const ISignal&)>>> SignalListener;

		class Listening
		{
		private :
			const std::function<void(ISignal)>& action;
			TypeID type;

		public:
			Listening(const std::function<void(const ISignal&)>& action, TypeID type)
				: __init(action),__init(type){ }
			Listening(const std::function<void(const ISignal&)>& action, Type type)
				: __init(action), type(type.hash_code()) {}

			void StopListening()
			{
				if (SingletonModel<Architecture>::Instance().SignalListener.count(type))
					SingletonModel<Architecture>::Instance().SignalListener[type].erase(action);
			}
		};

	public:
		template<typename Signal>
		Listening AddListener(std::enable_if_t<std::is_base_of_v<ISignal, Signal>, Type> slot, std::function<void(const Signal&)> listener)
		{
			if (SignalListener.count(slot.hash_code()) == false)
				SignalListener[slot.hash_code()] = {};
			const auto& action = *SignalListener[slot.hash_code()].insert([listener](const ISignal& x)
				{
					auto signal = dynamic_cast<const Signal* const>(&x);
					if (signal)
						listener(*signal);
				}).first;
			return Listening(action, slot);
		}

		template<typename Signal>
		Listening AddListener(std::enable_if_t<std::is_base_of_v<ISignal, Signal>, std::function<void(const Signal&)>> listener)
		{
			return AddListener<Signal>(typeof(Signal), listener);
		}

		void SendMessage(Type slot, const ISignal& signal)
		{
			if (SignalListener.count(slot.hash_code()))
			{
				for(auto&& action : SignalListener.at(slot.hash_code()))
				{
					action(signal);
				}
			}
		}

		template<typename Signal>
		void SendMessage(std::enable_if_t<std::is_base_of_v<ISignal, Signal>, const Signal&> signal)
		{
			return SendMessage(signal.GetType(), signal);
		}

#pragma endregion

#pragma region Timeline / Chain & Update

	private:
		struct TimelineQueneEntry
		{
			std::function<bool()> predicate;
			std::list<std::function<void()>> actions;
		};

		struct Timeline
		{
			std::map<const std::function<bool()>&, int> PredicateMapper;
			std::vector<TimelineQueneEntry> Quene;
			int Context = 0;
		};

		std::map<int, Timeline> TimelineQuenes;
		int TimelineContentID = 0;

	public:
		int CreateTimeline()
		{
			TimelineQuenes[TimelineContentID] = {};
			return TimelineContentID++;
		}

		void AddStep(int timelineId, std::function<bool()> predicate, std::list<std::function<void()>> actions)
		{
			auto&& timeline = TimelineQuenes[timelineId];
			if (timeline.PredicateMapper.count(predicate))
			{
				timeline.Quene[timeline.PredicateMapper[predicate]].actions.merge(actions);
			}
			else
			{
				int time = timeline.Quene.size();
				timeline.Quene.emplace_back();
				timeline.Quene.back().predicate = predicate;
				timeline.Quene.back().actions = actions;
				timeline.PredicateMapper.insert({ timeline.Quene.back().predicate, time });
			}
		}

		void UpdateTimeline()
		{
			for(auto&& [_, timeline] : TimelineQuenes)
			{
				if (timeline.Quene[timeline.Context].predicate())
				{
					for(auto&& action : timeline.Quene[timeline.Context].actions)
					{
						action();
					}
					timeline.Context++;
				}
			}
		}

		void ResetTimelineContext(int timelineId)
		{
			TimelineQuenes[timelineId].Context = 0;
		}

#pragma endregion
	};
}

#endif //Convention_Runtime_Architecture_hpp
