
#ifndef _HELPERMETHODS_HLSL
#define _HELPERMETHODS_HLSL


//Gotten from source https://stackoverflow.com/questions/5149544/can-i-generate-a-random-number-inside-a-pixel-shader
float random(float2 p)
{
    float2 K1 = float2(
        23.14069263277926, // e^pi (Gelfond's constant)
        2.665144142690225 // 2^sqrt(2) (Gelfond–Schneider constant)
    );
    return frac(cos(dot(p, K1)) * 12345.6789);
}
#endif