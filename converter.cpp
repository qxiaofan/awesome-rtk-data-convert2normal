#include "converter.h"

#include <cmath>

namespace converter {

int LLAToENU::setOrigin(double p_Lon, double p_Lat, double p_Alt) {
    /// set transform-matrix
    double sinLat = std::sin(p_Lat);
    double cosLat = std::cos(p_Lat);
    double sinLon = std::sin(p_Lon);
    double cosLon = std::cos(p_Lon);
    m_dTransMatrix[0][0] = -sinLon;
    m_dTransMatrix[0][1] = cosLon;
    m_dTransMatrix[0][2] = 0;
    m_dTransMatrix[1][0] = -sinLat * cosLon;
    m_dTransMatrix[1][1] = -sinLat * sinLon;
    m_dTransMatrix[1][2] = cosLat;
    m_dTransMatrix[2][0] = cosLat * cosLon;
    m_dTransMatrix[2][1] = cosLat * sinLon;
    m_dTransMatrix[2][2] = sinLat;

    /// set origin-coordinates
    transLLAToECEF(p_Lon, p_Lat, p_Alt, m_dX0_ECEF, m_dY0_ECEF, m_dZ0_ECEF);

    m_bOriginSeted = true;

    return 0;
}

int LLAToENU::transLLAToENU(double p_Lon, double p_Lat, double p_Alt, 
                    double &p_Xe, double &p_Xn, double &p_Xu)
{
    if (!m_bOriginSeted) {
        setOrigin(p_Lon, p_Lat, p_Alt);
        p_Xe = p_Xn = p_Xu = 0;
        return 0;
    }

    double x, y, z;
    transLLAToECEF(p_Lon, p_Lat, p_Alt, x, y, z);
    transECEFToENU(x, y, z, p_Xe, p_Xn, p_Xu);

    return 0;
}

int LLAToENU::transLLAToECEF(double p_Lon, double p_Lat, double p_Alt, 
                    double &p_Xecef, double &p_Yecef, double &p_Zecef) 
{
    double sinLat = std::sin(p_Lat);
    double cosLat = std::cos(p_Lat);
    double sinLon = std::sin(p_Lon);
    double cosLon = std::cos(p_Lon);
    double r = WGS84.a / (1 - WGS84.e2 * sinLat * sinLat);

    p_Xecef = (r + p_Alt) * cosLat * cosLon;
    p_Yecef = (r + p_Alt) * cosLat * sinLon;
    p_Zecef = (r * (1 - WGS84.e2) + p_Alt) * sinLat;

    return 0;
}

int LLAToENU::transECEFToENU(double p_Xecef, double p_Yecef, double p_Zecef, 
                    double &p_Xe, double &p_Xn, double &p_Xu)
{
    double dx = p_Xecef - m_dX0_ECEF;
    double dy = p_Yecef - m_dY0_ECEF;
    double dz = p_Zecef - m_dZ0_ECEF;

    p_Xe = m_dTransMatrix[0][0] * dx + m_dTransMatrix[0][1] * dy + m_dTransMatrix[0][2] * dz;
    p_Xn = m_dTransMatrix[1][0] * dx + m_dTransMatrix[1][1] * dy + m_dTransMatrix[1][2] * dz;
    p_Xu = m_dTransMatrix[2][0] * dx + m_dTransMatrix[2][1] * dy + m_dTransMatrix[2][2] * dz;

    return 0;
}

} // namespace converter