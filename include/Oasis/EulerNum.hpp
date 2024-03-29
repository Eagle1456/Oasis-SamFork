//
// Created by Sam Collins on 3/22/2024
//

#ifndef OASIS_EULER_HPP
#define OASIS_EULER_HPP

#include "Constant.hpp"
#include "Expression.hpp"
#include <cmath>

namespace Oasis {
class EulerNum : public Constant {
public:
    EulerNum() = default;

    /**
     * Gets the value of the real number.
     * @return The value of the real number.
     */
    [[nodiscard]] auto GetValue() const -> double
    {
        return std::exp(1.0);
    }

    [[nodiscard]] auto ToString() const -> std::string override
    {
        return "e";
    }

    static auto Specialize(const Expression& other) -> std::unique_ptr<EulerNum>
    {
        return other.Is<EulerNum>() ? std::make_unique<EulerNum>(dynamic_cast<const EulerNum&>(other)) : nullptr;
    }
    static auto Specialize(const Expression& other, tf::Subflow& subflow) -> std::unique_ptr<EulerNum>
    {
        return other.Is<EulerNum>() ? std::make_unique<EulerNum>(dynamic_cast<const EulerNum&>(other)) : nullptr;
    }
};
} // Oasis

#endif