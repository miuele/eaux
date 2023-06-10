#ifndef EAUX_H_INCLUDED
#define EAUX_H_INCLUDED

namespace eaux {

namespace numbers {

template <class T>
struct pi {
	static constexpr T value = 3.141592653589793238462643383279502884L;
};

template <class T>
constexpr T pi<T>::value;

}

constexpr float lerp(float a, float b, float t) noexcept {
	return a + t * (b - a);
}

template <class T>
constexpr const T &clamp(const T &v, const T &lo, const T &hi) {
    return v < lo ? lo : hi < v ? hi : v; 
}

template <class T, class Rep, class Period>
constexpr T to_seconds(const std::chrono::duration<Rep, Period> &d) {
	return static_cast<std::chrono::duration<T>>(d).count();
}

template <class Duration, class T, class Rep, class Period>
constexpr auto per_duration(const T &value, const std::chrono::duration<Rep, Period> &delta_t)
	-> std::enable_if_t<std::is_floating_point<T>::value, T>
{
	return value / static_cast<std::chrono::duration<T, typename Duration::period>>(delta_t).count();
}

template <class T, class ...Args>
T *construct_at(T *p, Args &&...args) {
	return ::new (const_cast<void *>(static_cast<const volatile void *>(p))) T(std::forward<Args>(args)...);
}

template <class T>
void destroy_at(T *p) {
	p->~T();
}

struct construct_on_init_t {
	explicit construct_on_init_t() = default;
};

template <class T>
struct object_cell {
	using object_type = T;

	constexpr object_cell()
		: object_(nullptr)
		, storage_{}	// must be initialized for constexpr constructor
	{}

	template <class ...Args>
	object_cell(construct_on_init_t, Args &&...args) {
		construct(std::forward<Args>(args)...);
	}

	~object_cell() {
		if (object_) {
			destruct();
		}
	}

	template <class ...Args>
	T *construct(Args &&...args) {
		object_ = eaux::construct_at(reinterpret_cast<T *>(storage_), std::forward<Args>(args)...);
		return object_;
	}

	void destruct() {
		eaux::destroy_at(object_);
		object_ = nullptr;
	}

	constexpr T *object() {
		return object_;
	}

	constexpr const T *object() const {
		return object_;
	}

private:
	T *object_;
	alignas(T) unsigned char storage_[sizeof(T)];
};

template <class F, class T>
auto visit_cell(eaux::object_cell<T> &cell, F &&f) {
	if (cell.object()) {
		std::forward<F>(f)(*cell.object());
	}
}

template <class F, class T>
auto visit_cell(const eaux::object_cell<T> &cell, F &&f) {
	if (cell.object()) {
		std::forward<F>(f)(*cell.object());
	}
}

}

#endif
