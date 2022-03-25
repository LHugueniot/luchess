
namespace chess{


#ifdef DEBUG_BUILD
	#define DEBUG(msg) do{ std::cout<<msg<<std::endl; }while(0)
	#define BREAKPOINT() do{  std::cout<<__FUNCTION__<<" at "<<__LINE__; }while(0)
#else
	#define DEBUG(msg)
	#define BREAKPOINT()
#endif

#define STR(s) #s

template<typename T>
T abs(T val)
{
	return val >= 0 ? val : -val;
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

}