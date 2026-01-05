// fake MSL_C math for sdk specifically since it differs from game MSL_C math

extern inline float sqrtf(float x) {
    static const double _half=.5;
    static const double _three=3.0;
    volatile float y;
    if(x > 0.0f) {
        double guess = __frsqrte((double)x);   // returns an approximation to   
        guess = _half*guess*(_three - guess*guess*x);  // now have 12 sig bits
        guess = _half*guess*(_three - guess*guess*x);  // now have 24 sig bits
        guess = _half*guess*(_three - guess*guess*x);  // now have 32 sig bits
        y=(float)(x*guess);
        return y ;
    }
    return x ;
}   

extern inline float sqrt(float x) {
    static const double _half=.5;
    static const double _three=3.0;
    volatile float y;
    if(x > 0.0f) {
        double guess = __frsqrte((double)x);   // returns an approximation to   
        guess = _half*guess*(_three - guess*guess*x);  // now have 12 sig bits
        guess = _half*guess*(_three - guess*guess*x);  // now have 24 sig bits
        guess = _half*guess*(_three - guess*guess*x);  // now have 32 sig bits
        guess = _half*guess*(_three - guess*guess*x);  // now have 32 sig bits

        y=(float)(x*guess);
        return y ;
    }
    return x ;
}

extern inline float fabs(float x) {
#if __MIPS__
    return fabsf(x);
#else
    (*(int*)&x)&=0x7fffffff;
    return  x;
#endif
}

extern inline float fabsf(float x) {
    return __fabsf(x);
}

extern float cosf(float);
extern float sinf(float);
extern float tanf(float);
extern float acosf(float);

extern inline float cos(float x) {
    return cosf(x);
}

inline float floor(float x) {
    int i=(int)x;   
    float y=x-(float)i; 

    if(!y || x > 8388608.0f)
    return x ;               // x is already an int 

    if(x < 0) 
    return (float)--i;  
        // x < 0 -> int conversion of x above rounded toward zero(so decrement)                
    return (float)i;
}
