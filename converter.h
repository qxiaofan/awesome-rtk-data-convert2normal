#ifndef CONVERTER_H
#define CONVERTER_H

namespace converter { 

class LLAToENU {
public:
    LLAToENU() = default;
    ~LLAToENU() = default;

    int setOrigin(double p_Lon, double p_Lat, double p_Alt);

    int transLLAToENU(double p_Lon, double p_Lat, double p_Alt, 
                    double &p_Xe, double &p_Xn, double &p_Xu);

private:
    int transLLAToECEF(double p_Lon, double p_Lat, double p_Alt, 
                    double &p_Xecef, double &p_Yecef, double &p_Zecef);

    int transECEFToENU(double p_Xecef, double p_Yecef, double p_Zecef, 
                    double &p_Xe, double &p_Xn, double &p_Xu);

private:
    struct Earth_Param {
        const double a = 6378137.0;
        const double f = 3.352810664725e-3; // = 1/298.257223565
        // const double we = 7.2921151467e-5;
        // const double Gm = 2.99792458e8;
        const double e2 = 6.694379990097e-3; // e = (a-b)/a, e2 = f * (2-f)
    }WGS84;

    /// origin
    bool m_bOriginSeted = false;
    double m_dX0_ECEF, m_dY0_ECEF, m_dZ0_ECEF;
    double m_dTransMatrix[3][3];
};

} // namespace converter

#endif // CONVERTER_H