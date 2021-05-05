#include <iostream>
#include <vector>
#include <complex>
#include <tuple>
#include <math.h>
using namespace std;

struct DiscriminantStrategy
{
    virtual double calculate_discriminant(double a, double b, double c) = 0;
};

struct OrdinaryDiscriminantStrategy : DiscriminantStrategy
{
    // todo
    double calculate_discriminant(double a, double b, double c) override
    {
        double d = (b * b) - (4 * a * c);
        return d;
    }
};

struct RealDiscriminantStrategy : DiscriminantStrategy
{
    // todo
    double calculate_discriminant(double a, double b, double c) override
    {
        double d = (b * b) - (4 * a * c);
        return d < 0 ? NAN : d;
    }
};

class QuadraticEquationSolver
{
    DiscriminantStrategy& strategy;
public:
    QuadraticEquationSolver(DiscriminantStrategy &strategy) : strategy(strategy) {}

    tuple<complex<double>, complex<double>> solve(double a, double b, double c)
        {
          complex<double> disc{strategy.calculate_discriminant(a,b,c), 0};
          auto root_disc = sqrt(disc);
          return {
              (-b+root_disc) / (2*a),
              (-b-root_disc) / (2*a) };
        };
};

int main()
{
    return 0;
}
