#ifndef SLICPLUSPLUS_SLICCONFIG_HPP_
#define SLICPLUSPLUS_SLICCONFIG_HPP_

#include <boost/shared_ptr.hpp>

#define SLIC_BEGIN_NAMESPACE \
    namespace slic {

#define SLIC_END_NAMESPACE \
    }

#define SLIC_DECLARE(A) \
    class A;            \
    typedef boost::shared_ptr<A>  A##SP;

#endif /* SLICPLUSPLUS_SLICCONFIG_HPP_ */
