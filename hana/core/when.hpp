#pragma once

#include <hana/config.hpp>

namespace hana{
    template<bool condition>
    struct when;

    namespace core_detail{
        template<typename...>
        struct always_true
        {
            static constexpr bool value=true;
        };
    }

    template<typename... Dummy>
    using when_valid=when<core_detail::always_true<Dummy...>::value;
}