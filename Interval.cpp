#include "Interval.hpp"

const Interval Interval::s_Empty = Interval(+kInf, -kInf);
const Interval Interval::s_Universe = Interval(-kInf, +kInf);
