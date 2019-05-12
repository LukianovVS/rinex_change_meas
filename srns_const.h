/*
Here list of other const
*/
#ifndef SRNS_CONST_H
#define SRNS_CONST_H


const double _C_                      = 299792458;
// GPS
const double _GPS_FREQ_L1_            = 1575.42e6;
const double _GPS_FREQ_L2_            = 1227.60e6;
const double _GPS_RANGE_TO_CYCLES_L1_ = _GPS_FREQ_L1_ / _C_;
const double _GPS_RANGE_TO_CYCLES_L2_ = _GPS_FREQ_L2_ / _C_;




#endif // SRNS_CONST_H

