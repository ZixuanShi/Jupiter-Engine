// Copyright Jupiter Technologies, Inc. All Rights Reserved.

module;

#include <atomic>

export module jpt.Atomic;

export namespace jpt
{
	template<typename T>
	class Atomic
	{
	private:
		std::atomic<T> m_value;

	public:
        Atomic();
        Atomic(T value);
        Atomic(const Atomic&) = delete;
        Atomic& operator=(const Atomic&) = delete;

    public:
        T Load() const;
        void Store(T value);

        T Exchange(T value);
        bool CompareExchangeWeak(T& expected, T desired);
        bool CompareExchangeStrong(T& expected, T desired);

	public:
        operator T() const;
        Atomic& operator=(T value);
		Atomic& operator++();
		Atomic& operator+=(T value);
	};

    template<typename T>
    Atomic<T>::Atomic()
        : m_value() 
    {
    }

    template<typename T>
    Atomic<T>::Atomic(T value)
        : m_value(value) 
    {
    }

    template<typename T>
    T Atomic<T>::Load() const
    {
        return m_value.load();
    }

    template<typename T>
    void Atomic<T>::Store(T value)
    {
        m_value.store(value);
    }

    template<typename T>
    T Atomic<T>::Exchange(T value)
    {
        return m_value.exchange(value);
    }

    template<typename T>
    bool Atomic<T>::CompareExchangeWeak(T& expected, T desired)
    {
        return m_value.compare_exchange_weak(expected, desired);
    }

    template<typename T>
    bool Atomic<T>::CompareExchangeStrong(T& expected, T desired)
    {
        return m_value.compare_exchange_strong(expected, desired);
    }

    template<typename T>
    Atomic<T>::operator T() const
    {
        return Load();
    }

    template<typename T>
    Atomic<T>& Atomic<T>::operator=(T value)
    {
        Store(value);
		return *this;
    }

    template<typename T>
    Atomic<T>& Atomic<T>::operator++()
    {
		++m_value;
		return *this;
    }

    template<typename T>
    Atomic<T>& Atomic<T>::operator+=(T value)
    {
		m_value += value;
		return *this;
    }
}