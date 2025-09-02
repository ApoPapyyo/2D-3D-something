#include "matrix.hpp"
#include <iostream>
#include <string>
#include <cassert>

using namespace okazawa;

int main()
{
    using m_t = matrix<3, 4, double>;
    m_t m_org{
        {1, 1, 1, 6},
        {2, 1, -1, 3},
        {1, -1, 2, 7}
    };

    // 元の行列を保持して不変性をチェック
    m_t m_org_copy = m_org;

    std::cerr << "Org: \n" << (std::string)m_org << std::endl << std::endl;

    // 1) 1行目と2行目を入れ替え (0<->1)
    {
        m_t result = m_org.swap_rows(0, 1);
        m_t expected{
            {2, 1, -1, 3},
            {1, 1,  1, 6},
            {1, -1, 2, 7}
        };
        std::cerr << "1, 2 行入れ替え:\n" << (std::string)result << std::endl;
        assert(result == expected);
        assert(m_org == m_org_copy);
    }

    // 2) 1列目と2列目を入れ替え (0<->1)
    {
        m_t result = m_org.swap_columns(0, 1);
        m_t expected{
            {1, 1, 1, 6},
            {1, 2,-1, 3},
            {-1,1, 2, 7}
        };
        std::cerr << "1, 2 列入れ替え:\n" << (std::string)result << std::endl;
        assert(result == expected);
        assert(m_org == m_org_copy);
    }

    // 3) 2行目を1行目に加算 (row0 += row1)
    {
        m_t result = m_org.add_row(1, 0);
        m_t expected{
            {3, 2, 0, 9},   // {1,1,1,6} + {2,1,-1,3}
            {2, 1,-1, 3},
            {1,-1, 2, 7}
        };
        std::cerr << "2行目を1行目に加算:\n" << (std::string)result << std::endl;
        assert(result == expected);
        assert(m_org == m_org_copy);
    }

    // 4) 2列目を1列目に加算 (col0 += col1)
    {
        m_t result = m_org.add_column(1, 0);
        m_t expected{
            {2, 1, 1, 6},   // col0:1+1=2
            {3, 1,-1, 3},   // col0:2+1=3
            {0,-1, 2, 7}    // col0:1+(-1)=0
        };
        std::cerr << "2列目を1列目に加算:\n" << (std::string)result << std::endl;
        assert(result == expected);
        assert(m_org == m_org_copy);
    }

    // 5) 2行目の2倍を1行目に加算 (row0 += 2 * row1)
    {
        m_t result = m_org.add_row(1, 0, 2.0);
        m_t expected{
            {5, 3,-1,12},   // {1,1,1,6} + 2*{2,1,-1,3} = {1+4,1+2,1-2,6+6}
            {2, 1,-1, 3},
            {1,-1, 2, 7}
        };
        std::cerr << "2行目の2倍を1行目に加算:\n" << (std::string)result << std::endl;
        assert(result == expected);
        assert(m_org == m_org_copy);
    }

    // 6) 2列目の2倍を1列目に加算 (col0 += 2 * col1)
    {
        m_t result = m_org.add_column(1, 0, 2.0);
        m_t expected{
            {3, 1, 1, 6},   // col0:1 + 2*1 = 3
            {4, 1,-1, 3},   // col0:2 + 2*1 = 4
            {-1,-1,2, 7}    // col0:1 + 2*(-1) = -1
        };
        std::cerr << "2列目の2倍を1列目に加算:\n" << (std::string)result << std::endl;
        assert(result == expected);
        assert(m_org == m_org_copy);
    }

    // 7) 1行目を2倍 (row0 *= 2)
    {
        m_t result = m_org.scale_row(0, 2.0);
        m_t expected{
            {2, 2, 2,12},
            {2, 1,-1, 3},
            {1,-1, 2, 7}
        };
        std::cerr << "1行目を2倍:\n" << (std::string)result << std::endl;
        assert(result == expected);
        assert(m_org == m_org_copy);
    }

    // 8) 1列目を2倍 (col0 *= 2)
    {
        m_t result = m_org.scale_column(0, 2.0);
        m_t expected{
            {2, 1, 1, 6},   // col0: 1*2 = 2
            {4, 1,-1, 3},   // col0: 2*2 = 4
            {2,-1, 2, 7}    // col0: 1*2 = 2
        };
        std::cerr << "1列目を2倍:\n" << (std::string)result << std::endl;
        assert(result == expected);
        assert(m_org == m_org_copy);
    }

    std::cerr << "All tests passed!" << std::endl;
    return 0;
}
