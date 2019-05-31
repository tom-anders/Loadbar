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
        const bool half;
        int c;
        double lastSpin;
    public:
        const int maximumIterations;
        const int barLength, decimalPoints;
        const char sym0, sym1, sym2;
        const std::string barEnd, spin;
        const bool isSpinning;
        const double spinPS;
        int maxSize;
        ProgressBar(int maximumIterations, int barLength = 20, int decimalPoints = 0, 
                bool isSpinning = true, double spinPS = 0.1, std::string spin = "|/-\\", 
                char sym0 = ' ', char sym1 = '-', char sym2 = '=', std::string barEnd = "Done") 
            : maximumIterations(maximumIterations), 
              barLength(barLength), 
              decimalPoints(decimalPoints), 
              isSpinning(isSpinning), 
              spinPS(spinPS), 
              lastSpin(0), 
              sym0(sym0), sym1(sym1), sym2(sym2),
              barEnd(barEnd), 
              mid(0), full(0), 
              half(sym1 != sym2), 
              spin(spin), 
              c(0), 
              maxSize(0) {}

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
