//
// Created by Levy Lin on 2/09/2024.
//

#include "Oasis/Integrate.hpp"

#include "Oasis/Add.hpp"
#include "Oasis/Subtract.hpp"
#include "Oasis/Multiply.hpp"
#include "Oasis/Divide.hpp"
#include "Oasis/Exponent.hpp"
//#include "Oasis/Log.hpp"
//#include "Oasis/Imaginary.hpp"


namespace Oasis {

Integrate<Expression>::Integrate(const Expression& integrand, const Expression& differential)
    : BinaryExpression(integrand, differential)
{
}

auto Integrate<Expression>::Simplify() const -> std::unique_ptr<Expression>
{
    // Returns simplified Integral

    auto simplifiedIntegrand = mostSigOp ? mostSigOp->Simplify() : nullptr;
    auto simplifiedDifferential = leastSigOp ? leastSigOp->Simplify() : nullptr;

    Integrate simplifiedIntegrate { *simplifiedIntegrand, *simplifiedDifferential };

    // Unbounded Integration Rules

    // Constant Rule
    if (auto constCase = Integrate<Real, Variable>::Specialize(simplifiedIntegrate); constCase != nullptr) {
        const Variable& differential = constCase->GetLeastSigOp();
        Oasis::Real constant{constCase->GetMostSigOp()};

        return constant.Integrate(differential);
    }

    if (auto constCase = Integrate<Divide<Real>, Variable>::Specialize(simplifiedIntegrate); constCase != nullptr) {
        const Variable& differential = constCase->GetLeastSigOp();
        Oasis::Divide constant{constCase->GetMostSigOp()};
        auto constReal = constant.Simplify();

        return constReal->Integrate(differential);
    }

    // Power Rule
    if (auto powerCase = Integrate<Exponent<Expression>, Variable>::Specialize(simplifiedIntegrate); powerCase != nullptr) {
        const Variable& differential = powerCase->GetLeastSigOp();
        Oasis::Exponent exponent{powerCase->GetMostSigOp()};

        return exponent.Integrate(differential);
    }

    // Constant + Power Rule
    if (auto powerCase = Integrate<Multiply<Expression, Exponent<Expression>>, Variable>::Specialize(simplifiedIntegrate); powerCase != nullptr) {
        const Variable& differential = powerCase->GetLeastSigOp();
        Oasis::Exponent exponent{powerCase->GetMostSigOp().GetLeastSigOp()};
        auto integration = exponent.Integrate(differential);

        return std::make_unique<Multiply<Expression>>(powerCase->GetMostSigOp().GetMostSigOp(), (*integration));
    }

    if (auto powerCase = Integrate<Multiply<Exponent<Expression>, Expression>, Variable>::Specialize(simplifiedIntegrate); powerCase != nullptr) {
        const Variable& differential = powerCase->GetLeastSigOp();
        Oasis::Exponent exponent{powerCase->GetMostSigOp().GetMostSigOp()};
        auto integration = exponent.Integrate(differential);

        return std::make_unique<Multiply<Expression>>(powerCase->GetMostSigOp().GetLeastSigOp(), (*integration));
    }

    // U Substitution Rule
    // Needs Differentiation Implementation for U-Sub

    // Exponential Rule - a^x
    // Needs Euler's Number for Exponential Functions

    // Natural Log Rule
    // Needs Euler's Number for Exponential Functions

    // Euler's Number
    // Needs Euler's Number for Exponential Functions

    // Trigonometric Rules
    // Needs Trig for Exponential Functions

    // Powers of Sin and Cos
    // Needs Trig for Exponential Functions

    // Integration by Parts
    // Needs Trig and Euler's Number

    // Partial Fraction Decomposition
    // Work in Progress


    return simplifiedIntegrate.Copy();
}

auto Integrate<Expression>::Simplify(const Expression& upper, const Expression& lower) const -> std::unique_ptr<Expression>
{
    // Returns simplified Integral

    auto simplifiedIntegrand = mostSigOp ? mostSigOp->Simplify() : nullptr;
    auto simplifiedDifferential = leastSigOp ? leastSigOp->Simplify() : nullptr;

    Integrate simplifiedIntegrate { *simplifiedIntegrand, *simplifiedDifferential };

    // Bounded Integration Rules

    // Constant Rule
    if (auto constCase = Integrate<Real, Variable>::Specialize(simplifiedIntegrate); constCase != nullptr) {
        const Real& constant = constCase->GetMostSigOp();

        auto upper_bound = Real::Specialize(upper);
        auto lower_bound = Real::Specialize(lower);

        if (upper_bound!= nullptr && lower_bound != nullptr) {
            return std::make_unique<Real>((constant.GetValue() * upper_bound->GetValue()) - (constant.GetValue() * lower_bound->GetValue()));
        }
        else if (upper_bound != nullptr && lower_bound == nullptr) {
            return std::make_unique<Subtract<Real, Expression>>( Real {constant.GetValue() * upper_bound->GetValue()}, *Multiply<Expression>{constant, lower}.Simplify() );
        }
        else if (upper_bound == nullptr && lower_bound != nullptr) {
            return std::make_unique<Subtract<Expression, Real>>( *Multiply<Expression>{constant, upper}.Simplify(), Real{constant.GetValue() * lower_bound->GetValue()} );
        }
        return std::make_unique<Subtract<Expression>>( *Multiply<Expression>{constant, upper}.Simplify(), *Multiply<Expression>{constant, lower}.Simplify() );
    }

    if (auto constCase = Integrate<Divide<Real>, Variable>::Specialize(simplifiedIntegrate); constCase != nullptr) {
        Oasis::Divide constant{constCase->GetMostSigOp()};
        auto constReal = Real::Specialize(*constant.Simplify());

        auto upper_bound = Real::Specialize(upper);
        auto lower_bound = Real::Specialize(lower);

        if (upper_bound!= nullptr && lower_bound != nullptr) {
           return std::make_unique<Real>( (Real{*constReal}.GetValue() * upper_bound->GetValue()) - (Real{*constReal}.GetValue() * lower_bound->GetValue()) );
        }
        else if (upper_bound != nullptr && lower_bound == nullptr) {
            return std::make_unique<Subtract<Real, Expression>>( Real{ Real{*constReal}.GetValue() * upper_bound->GetValue() }, *Multiply<Expression>{ *(constReal), lower }.Simplify() );
        }
        else if (upper_bound == nullptr && lower_bound != nullptr) {
            return std::make_unique<Subtract<Expression, Real>>( *Multiply<Expression>{ *(constReal), upper }.Simplify(), Real{ Real{*constReal}.GetValue() * lower_bound->GetValue() } );
        }
        return std::make_unique<Subtract<Expression>>( *Multiply<Expression>{ *(constReal), upper }.Simplify(), *Multiply<Expression>{ *(constReal), lower }.Simplify());
    }


    // Power Rule
    if (auto powerCase = Integrate<Exponent<Expression>, Variable>::Specialize(simplifiedIntegrate); powerCase != nullptr) {
        const Variable& differential = powerCase->GetLeastSigOp();
        Oasis::Exponent exponent{powerCase->GetMostSigOp()};
        auto integral = Add<Divide<Exponent<Variable, Real>, Real>, Variable>::Specialize(*exponent.Integrate(differential));

        auto upper_bound = Real::Specialize(upper);
        auto lower_bound = Real::Specialize(lower);

        if (upper_bound!= nullptr && lower_bound != nullptr) {

//            const Real& divisor = integral->GetMostSigOp().GetLeastSigOp();
//            const Real& power = integral->GetMostSigOp().GetMostSigOp().GetLeastSigOp();
//            const IExpression<Real> auto& left = Exponent<Real>{ Real{upper_bound->GetValue()}, power };
//            const IExpression<Real> auto& right = *Exponent<Real>{ Real{lower_bound->GetValue()}, power }.Simplify();

//            return make_unique<Real>(  *Subtract<Real>{ *Divide<Real>{ left, divisor }.Simplify(), *Divide<Real>{ right, divisor }.Simplify() }.Simplify() );
        }
        else if (upper_bound != nullptr && lower_bound == nullptr) {

        }
        else if (upper_bound == nullptr && lower_bound != nullptr) {

        }
//        return
    }
//
//    // Constant + Power Rule
//    if (auto powerCase = Integrate<Multiply<Expression, Exponent<Expression>>, Variable>::Specialize(simplifiedIntegrate); powerCase != nullptr) {
//        const Variable& differential = powerCase->GetLeastSigOp();
//        Oasis::Exponent exponent{powerCase->GetMostSigOp().GetLeastSigOp()};
//        auto integration = exponent.Integrate(differential);
//
//        return std::make_unique<Multiply<Expression>>(powerCase->GetMostSigOp().GetMostSigOp(), (*integration));
//    }
//
//    if (auto powerCase = Integrate<Multiply<Exponent<Expression>, Expression>, Variable>::Specialize(simplifiedIntegrate); powerCase != nullptr) {
//        const Variable& differential = powerCase->GetLeastSigOp();
//        Oasis::Exponent exponent{powerCase->GetMostSigOp().GetMostSigOp()};
//        auto integration = exponent.Integrate(differential);
//
//        return std::make_unique<Multiply<Expression>>(powerCase->GetMostSigOp().GetLeastSigOp(), (*integration));
//    }

    // U Substitution Rule
    // Needs Differentiation Implementation for U-Sub

    // Exponential Rule - a^x
    // Needs Euler's Number for Exponential Functions

    // Natural Log Rule
    // Needs Euler's Number for Exponential Functions

    // Euler's Number
    // Needs Euler's Number for Exponential Functions

    // Trigonometric Rules
    // Needs Trig for Exponential Functions

    // Powers of Sin and Cos
    // Needs Trig for Exponential Functions

    // Integration by Parts
    // Needs Trig and Euler's Number

    // Partial Fraction Decomposition
    // Work in Progress


//    return simplifiedIntegrate.Copy();
}


auto Integrate<Expression>::ToString() const -> std::string
{
    return fmt::format("({} + {})", mostSigOp->ToString(), leastSigOp->ToString());
}

auto Integrate<Expression>::Simplify(tf::Subflow& subflow) const -> std::unique_ptr<Expression>
{
    std::unique_ptr<Expression> simplifiedIntegrand, simplifiedDifferential;

    // To be Implemented : Performs Integration on simplified Expression



}

auto Integrate<Expression>::Specialize(const Expression& other) -> std::unique_ptr<Integrate<Expression, Expression>>
{
    if (!other.Is<Oasis::Integrate>()) {
        return nullptr;
    }

    auto otherGeneralized = other.Generalize();
    return std::make_unique<Integrate>(dynamic_cast<const Integrate&>(*otherGeneralized));
}

auto Integrate<Expression>::Specialize(const Expression& other, tf::Subflow& subflow) -> std::unique_ptr<Integrate>
{
    if (!other.Is<Oasis::Integrate>()) {
        return nullptr;
    }

    auto otherGeneralized = other.Generalize(subflow);
    return std::make_unique<Integrate>(dynamic_cast<const Integrate&>(*otherGeneralized));
}

} // Oasis
