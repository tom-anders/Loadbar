#include <cmath>
//#include <complex>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
//#include <stdarg.h>
#include <string>
#include <iostream>
#include <omp.h> // -fopenmp
#include <fmt/core.h> // -DFMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/ostream.h>

class ProgressBar {
    private:
        double currentPercent;
        int mid, full;
        bool half;
        int c;
        double lastSpin;
    public:
        int maximumIterations;
        int barLength, decimalPoints;
        char sym0, sym1, sym2;
        std::string barEnd, spin;
        bool isSpinning;
        double spinPS;
        int maxSize;
        ProgressBar(int _maximumIterations, int _barLength = 20, int _decimalPoints = 0, bool _isSpinning = true, double _spinPS = 0.1, std::string _spin = "|/-\\", char _sym0 = ' ', char _sym1 = '-', char _sym2 = '=', std::string _barEnd = "Done") {
            maximumIterations   = _maximumIterations;
            barLength           = _barLength;
            decimalPoints       = _decimalPoints;
            isSpinning          = _isSpinning;
            spinPS              = _spinPS;
            lastSpin            = 0;
            sym0                = _sym0;
            sym1                = _sym1;
            sym2                = _sym2;
            barEnd              = _barEnd;
            mid                 = 0;
            full                = 0;
            half                = ( _sym1 == _sym2 ? false : true );
            spin                = _spin;
            c                   = 0;
            maxSize = 0;
        }
        void calculate(int currentIterations) {
            currentPercent  = 1.0*currentIterations/maximumIterations;
            full            = half ? std::ceil(barLength*2.0*std::max(.0,currentPercent-.5)) : 0;
            mid             = half ? std::ceil(barLength*2.0*std::min(.5,currentPercent)) : std::ceil(barLength*std::min(1.,currentPercent));
        }
        std::string print(int currentIterations, std::string barSuffix = "", std::string barPrefix = "") {
            calculate(currentIterations);
            std::string ret = "";
            for (int i = 0; i < barLength; i++)
                if (i < full)
                    ret += sym2;
                else if (i < mid)
                    ret += sym1;
                else
                    ret += sym0;
            if (omp_get_wtime() - lastSpin >= spinPS) { 
                lastSpin = omp_get_wtime();
                c++;
            }
            ret = barPrefix + "[" + ret + "]"
             + ( decimalPoints >= 0 ? fmt::format(" {:.{}f}\%", 1.0*currentIterations/maximumIterations*100, decimalPoints) : "" )
             + ( (isSpinning && currentIterations < maximumIterations) ? " [" + spin.substr(c%spin.size(),1)+"] " : " " )
             + ( currentIterations < maximumIterations ? barSuffix : barEnd );
            maxSize = (ret.size()>maxSize) ? ret.size() : maxSize;
            fmt::print("{:<{}}\r",ret,maxSize);
            return ret;
        }
};