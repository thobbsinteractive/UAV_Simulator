#ifndef INC_FRUSTUMPLANE_H
#define INC_FRUSTUMPLANE_H

// Based on http://www.toymaker.info/Games/html/direct3d_faq.html#D3D5
struct frustumPlane
{
   D3DXVECTOR3 m_normal;
   float m_distance;

   inline void Normalise() 
   {
     float denom = 1 / sqrt((m_normal.x*m_normal.x) + (m_normal.y*m_normal.y) +                  (m_normal.z*m_normal.z));
     m_normal.x = m_normal.x * denom;
     m_normal.y = m_normal.y * denom;
     m_normal.z = m_normal.z * denom;
     m_distance = m_distance * denom;
   }
};

#endif // INC_FRUSTUMPLANE_H
