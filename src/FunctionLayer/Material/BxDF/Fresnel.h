/**
 * @file Fresnel.h
 * @author JunPing Yuan
 * @brief compute fresnel items
 * @version 0.1
 * @date 2022-9-6
 *
 * @copyright Copyright (c) 2022
 *
 */

#pragma  once
#include "math.h"
#include "CoreLayer/Geometry/Geometry.h"

namespace Fresnel {

    static inline  double dielectricReflectance(double eta, double cosThetaI, double &  cosThetaT){
        if (cosThetaI < 0.0) {
            eta = 1.0/eta;
            cosThetaI = -cosThetaI;
        }
        double sinThetaTSq = eta*eta*(1.0f - cosThetaI*cosThetaI);
        if (sinThetaTSq > 1.0) {
            cosThetaT = 0.0;
            return 1.0;
        }
        cosThetaT = std::sqrt(std::max(1.0 - sinThetaTSq, 0.0));

        double Rs = (eta*cosThetaI - cosThetaT)/(eta*cosThetaI + cosThetaT);
        double Rp = (eta*cosThetaT - cosThetaI)/(eta*cosThetaT + cosThetaI);

        return (Rs*Rs + Rp*Rp)*0.5;
    }

    static inline  double  dielectricReflectance(double eta, double cosThetaI){
        double cosThetaT;
        return dielectricReflectance(eta,cosThetaI,cosThetaT);
    }

    static inline double conductorReflectance(double eta, double k, double cosThetaI)
    {
        double cosThetaISq = cosThetaI*cosThetaI;
        double sinThetaISq = std::max(1.0f - cosThetaISq, 0.0);
        double sinThetaIQu = sinThetaISq*sinThetaISq;

        double innerTerm = eta*eta - k*k - sinThetaISq;
        double aSqPlusBSq = std::sqrt(std::max(innerTerm*innerTerm + 4.0f*eta*eta*k*k, 0.0));
        double a = std::sqrt(std::max((aSqPlusBSq + innerTerm)*0.5, 0.0));

        double Rs = ((aSqPlusBSq + cosThetaISq) - (2.0*a*cosThetaI))/
                   ((aSqPlusBSq + cosThetaISq) + (2.0*a*cosThetaI));
        double Rp = ((cosThetaISq*aSqPlusBSq + sinThetaIQu) - (2.0f*a*cosThetaI*sinThetaISq))/
                   ((cosThetaISq*aSqPlusBSq + sinThetaIQu) + (2.0f*a*cosThetaI*sinThetaISq));

        return 0.5f*(Rs + Rs*Rp);
    }

    static inline Vec3d conductorReflectance(const Vec3d &eta, const Vec3d &k, double cosThetaI)
    {
        return Vec3d(
                conductorReflectance(eta.x, k.x, cosThetaI),
                conductorReflectance(eta.y, k.y, cosThetaI),
                conductorReflectance(eta.z, k.z, cosThetaI)
        );
    }
}

