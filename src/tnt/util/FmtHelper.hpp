#ifndef TNT_FMTHELPER_HPP
#define TNT_FMTHELPER_HPP

#include <optional>
#include <variant>
#include <memory>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/time.h>
#include <fmt/chrono.h>
#include <fmt/ranges.h>

template<>
struct fmt::formatter<std::exception> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const std::exception &p, FormatContext &ctx) {
        return format_to(ctx.begin(),
                         "<exception>"
                         " what: {} ",
                         p.what());
    }
};

template<typename X>
struct fmt::formatter<std::optional<X>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const std::optional<X> &p, FormatContext &ctx) {
        if (p)
            return format_to(ctx.begin(), "<{}>_opt", *p);
        else
            return format_to(ctx.begin(), "<>_opt");


    }
};


template<typename X>
struct fmt::formatter<std::unique_ptr<X>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const std::unique_ptr<X> &p, FormatContext &ctx) {
        if (p)
            return format_to(ctx.begin(), "<{}>_unq", *p);
        else
            return format_to(ctx.begin(), "<>_unq");
    }
};

template<typename X, typename  Y>
struct fmt::formatter<std::variant<X,Y>> {
    template<typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(const std::variant<X,Y> &p, FormatContext &ctx) {
        if (std::holds_alternative<X>(p))
            return format_to(ctx.begin(), "<{}>_var", std::get<X>(p));
        else
            return format_to(ctx.begin(), "<{}>_var", std::get<Y>(p));
    }
};

//template<typename X>
//struct fmt::formatter<std::shared_ptr<const std::decay<X>>> {
//    template<typename ParseContext>
//    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }
//
//    template<typename FormatContext>
//    auto format(const std::shared_ptr<const std::decay<X>> &p, FormatContext &ctx) {
//        if (p)
//            return format_to(ctx.begin(), "<{}>_shd", *p);
//        else
//            return format_to(ctx.begin(), "<>_shd");
//    }
//};

#endif //TNT_FMTHELPER_HPP
