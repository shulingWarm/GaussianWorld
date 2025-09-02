#pragma once
#include<memory>

template<class T>
using Ptr = std::shared_ptr<T>;

template<typename T, typename... Args>
inline auto makePtr(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}