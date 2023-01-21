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

template <class Rep, class Period>
constexpr float duration_to_float(const std::chrono::duration<Rep, Period> &d) {
    return std::chrono::duration<float>(d).count();
}

template <class T, class ...Args>
T *construct_at(T *p, Args &&...args) {
	return ::new (const_cast<void *>(static_cast<const volatile void *>(p))) T(std::forward<Args>(args)...);
}

template <class T>
void destroy_at(T *p) {
	p->~T();
}

}

#endif
