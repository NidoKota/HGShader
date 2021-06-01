#include "ShaderFuncs.hpp"

//虹色のゆらめく円のシェーダー
vec3 RainbowCircleShader(vec3 uv, float t)
{
    vec3 result;

    result = rainbow(uv.x * uv.y / 3.f + t / 10.f);
    
    float radius = gradationNoise(uv * 1.5f + t / 6.f) * 0.5f + 0.5f;

    vec3 centerUV = (uv - 0.5f) * 2.f;
    centerUV.z = 0;

    float width = 0.125f;
    float fade = 0.03f;

    float length = lengthsqr(centerUV);
    float circle = smoothstep(length, length + fade, radius) - smoothstep(length + width, length + width + fade, radius);

    result = screenSynthesis(result, 0.5f);

    result *= circle;

    return result;
}

//ハートシェーダー
vec3 HeartShader(vec3 uv, float t)
{
    uv = (uv - 0.5f) * 2.f;

    uv += vec3(0.f, 0.3f);
    uv *= vec3(0.9f, 1.2f);
    
    float width = 0.01f;
    float radius = 0.3f;
    float fade = 0.05f;
    float length = pow(uv.x, 2.f) + pow(uv.y - sqrt(abs(uv.x)), 2.f) - t * 0.15f;
    float heart = smoothstep(radius + fade, radius, abs(sin(length) * 1.5f));
    
    length *= 2.f;
    float f13 = step(1.f / 3.f, fract(length));
    float f23 = step(2.f / 3.f, fract(length));
    vec3 result = vec3(1.f, 0.63f, 0.63f) * (1.f - f13) +
                  vec3(1.f, 0.3f, 0.4f) * f23 + 
                  vec3(1.f, 0.56f, 0.1f) * f13 * (1.f - f23);
                    
    result *= heart;
    result += 1.f - heart;

    return result;
}

//波シェーダー
vec3 WaveShader(vec3 uv, float t)
{
    float colorHeight = 2;

    float wave0 = (sin((0.2f * t + uv).x * 8.f + t / 10.f) + 0.5f) / 2.f;
    float wave1 = (sin((-0.15f * t + uv).x * -3.f + t / -20.f) + 0.5f) / 2.f;

    float sumWave = ((wave0 + wave1) / 6.f + uv).y;
    float resultWave = smoothstep(sumWave, sumWave + 0.03f, 0.55f);

    vec3 waveColor = (1.f - uv.y * colorHeight) * vec3(0.14f, 0.95f, 0.93f) + uv.y * colorHeight * vec3(0.1f, 0.5f, 1.f);
    vec3 backColor = (1.f - uv.y) * vec3(0.8f, 0.85f, 1.f) + uv.y * vec3(1.f, 0.75f, 0.65f);
    
    return (1.f - resultWave) * backColor + resultWave * waveColor;
}

float circle(vec3 p, float radius)
{
    return length(p) - radius;
}

float polygon(vec3 p, int n, float size)
{
    float a = atan2(p.x, p.y) + M_PI;
    float r = 2.f * M_PI / n;
    return cos(floor(0.5f + a / r) * r - a) * length(p) - size; 
}

float star(vec3 p, int n, float t, float size)
{
    p.z = 0.f;
    float a = 2.f * M_PI / ((float)n * 2.f);
    float aco = cos(a);
    float asi = sin(a);
    vec3 v33[3] = {vec3(aco, -asi), vec3(asi, aco), 0};
    vec3 r = mul3x333(p, v33);
    return (polygon(p, n, size) - polygon(r, n, size) * t) / (1.f - t);
}

float square(vec3 p, vec3 size)
{
    return max(abs(p.x) - size.x, abs(p.y) - size.y);
}

float getShapeRatio(float fade, float t)
{
    t = fract(t);
    float tRoop = fract(t / 5.f) * (1.f + fade) - fade / 2.f;
    return smoothstep(tRoop - 0.05f, tRoop, 0.f) - smoothstep(tRoop, tRoop + 0.05f, 0.f);
}

//形を変えながら回転するシェーダー
vec3 RotationShapesShader(vec3 uv, float t)
{
    uv = rotate(uv, smoothstep(0.f, 1.f, fract(t / 15.f)) * 2.5f * M_PI);
    uv -= 0.5f;
    uv *= 2.f;
    
    float squ = square(uv, (vec3)1.f);
    float str = star(uv, 5, 0.5f, 1.f);
    float cir = circle(uv, 1.f);
    float pol = polygon(uv, 3, 0.9f);

    t /= 30.f;
    float fade = 1.f / 4.f;
    return smoothstep(-0.475f, -0.5f, squ * getShapeRatio(fade, t + fade * 0.f) + 
                                      str * getShapeRatio(fade, t + fade * 1.f) + 
                                      cir * getShapeRatio(fade, t + fade * 2.f) +
                                      pol * getShapeRatio(fade, t + fade * 3.f));
}

//ローディングっぽいシェーダー
vec3 LoadingShader(vec3 uv, float t)
{
    t *= -1.f;

    float rotateSpeeed = 0.16f;
    float addRotateSpeeed = 0.11f;

    float addRotate = fract(t * addRotateSpeeed);
    float smoothstepDifferential = -6.f * addRotate * addRotate + 6.f * addRotate;

    float inCircleRadius = 0.6f + (smoothstepDifferential / 1.5f) * 0.15f;
    float outCircleRadius = 0.95f - (smoothstepDifferential / 1.5f) * 0.15f;

    float easyAddRotate = smoothstep(0.f, 1.f, addRotate) * 2.f;
    
    vec3 topRotUV = rotate(uv, t * rotateSpeeed * -2.f * (float)M_PI);
    vec3 bottomRotUV = rotate(topRotUV, fract(easyAddRotate) * -2.f * (float)M_PI);

    float topRot = step(topRotUV.x, 0.5f);
    float bottomRot = step(bottomRotUV.x, 0.5f);

    float halfRot1 = 1.f - clamp(topRot + bottomRot, 0.f, 1.f) + topRot;
    float halfRot2 = clamp(topRot - bottomRot, 0.f, 1.f);

    float halfRotSw = step(0.5f, fract(easyAddRotate));
    float rots = clamp(halfRot1 * halfRotSw + halfRot2 * (1.f - halfRotSw), 0.f, 1.f);
    
    float rotsSw = step(1.f, easyAddRotate);
    float allRot =  rots * (1.f - rotsSw) + (1.f - rots) * rotsSw;

    vec3 centerUV = (uv - 0.5f) * 2.f;
    float distance = centerUV.x * centerUV.x + centerUV.y * centerUV.y;
    float fade = 0.03f;
    float inCircle = smoothstep(distance, distance + fade, inCircleRadius * inCircleRadius);
    float outCircle = smoothstep(distance, distance + fade, outCircleRadius * outCircleRadius);

    float colorSw = fract(t * addRotateSpeeed / 4.f) * 4.f;
    float _1St = step(1.f, colorSw);
    float _2St = step(2.f, colorSw);
    float _3St = step(3.f, colorSw);

    vec3 blue = (1.f - _1St) * vec3(0.25f, 0.5f, 0.93f);
    vec3 red = (1.f - _2St) * _1St * vec3(0.89f, 0.26f, 0.2f);
    vec3 yellow = (1.f - _3St) * _2St * vec3(0.95f, 0.72f, 0.02f);
    vec3 green = _3St * vec3(0.2f, 0.64f, 0.3f);

    vec3 color = blue + red + yellow + green;

    return allRot * (outCircle - inCircle) * color;
}

float sphere(vec3 p)
{
    float size = 1.5f;
    return length(mod(p, (vec3)6.f) - (6.f / 2.f)) - size;
}

vec3 getSphereNormal(vec3 p)
{
    float d = 0.0001f;
    return normalize(vec3(sphere(p + vec3(d, 0.f, 0.f)) - sphere(p + vec3(-d, 0.f, 0.f)),
                          sphere(p + vec3(0.f, d, 0.f)) - sphere(p + vec3(0.f, -d, 0.f)),
                          sphere(p + vec3(0.f, 0.f, d)) - sphere(p + vec3(0.f, 0.f, -d))));
}

//宇宙っぽいシェーダー
vec3 SpaceShader(vec3 uv, float t)
{
    uv = (uv -0.5f) * 2.f;

    float fov = tan(radians(60.f / 2.f));
    vec3 lightDir = vec3(0.5f);

    vec3 color;
    
    vec3 comPos = vec3(0.f, 0.f, -t / 5.f);
    vec3 comForword = vecRotateZ(vecRotateY(vecRotateX(vec3(0.f, 0.f, 1.f), sin(t / 70.f)), cos(t / 70.f)), -sin(t / 70.f));
    vec3 comUp = vecRotateZ(vecRotateY(vecRotateX(vec3(0.f, 1.f, 0.f), sin(t / 70.f)), cos(t / 70.f)), -sin(t / 70.f));
    vec3 comRight = cross(comForword, comUp);
    
    vec3 ray = normalize(comForword + comRight * uv.x * fov + comUp * uv.y * fov);
    
    float distance = 0.f;
    float rLength = 0.f;
    vec3 rPos = comPos;
    for(int i = 0; i < 30; i++)
    {
        distance = sphere(rPos);
        rLength += distance;
        rPos = comPos + ray * rLength;
    }

    float sw = 1.f - step(0.001f, abs(distance));
    float fade = 0.01f;
    float dps = rLength / 40.f;
    float a = fract(t / 50.f) * (fade + 1.f) + fade;
    color = clamp(dot(lightDir, getSphereNormal(rPos)), 0.f, 1.f) * 0.3f + 0.3f + (step(a - fade, dps) - step(a + fade, dps)) * 0.3f; 
    color *= sw;

    return floor(color * 9.f) / 9.f;
}

float boxFunc(vec3 p)
{
    vec3 q = abs(p);
    return length(max(q - vec3(0.5f, 0.f, 0.5f), 0.f)) - 0.3f;
}

float torusFunc(vec3 p)
{
    vec3 t = vec3(0.65f, 0.2f);
    vec3 r = vec3(length(vec3(p.x, p.y)) - t.x, p.z);
    return length(r) - t.y;
}

float blockFunc(vec3 p)
{
    float d1 = torusFunc(p);
    float d2 = boxFunc(p);
    return max(-d1, d2);
}

vec3 getBlockNormal(vec3 p)
{
    float d = 0.0001f;
    return normalize(vec3(blockFunc(p + vec3(d, 0.f, 0.f)) - blockFunc(p + vec3(-d, 0.f, 0.f)),
                          blockFunc(p + vec3(0.f, d, 0.f)) - blockFunc(p + vec3(0.f, -d, 0.f)),
                          blockFunc(p + vec3(0.f, 0.f, d)) - blockFunc(p + vec3(0.f, 0.f, -d))));
}

//ブロックが回転するシェーダー
vec3 BlockShader(vec3 uv, float t)
{
    uv = (uv -0.5f) * 2.f;

    float fov = tan(radians(60.f / 2.f));

    vec3 color;
    
    vec3 comPos = vecRotateZ(vecRotateY(vec3(1.f), t / 3.f), t / 6.f) * 2.f;
    vec3 comForword = -normalize(comPos);
    vec3 comUp = vecRotateZ(comForword, M_PI / 2.f);
    vec3 comRight = cross(comForword, comUp);
    
    vec3 lightDir = -comForword;

    vec3 ray = normalize(comForword + comRight * uv.x * fov + comUp * uv.y * fov);
    
    float distance = 0.f;
    float rLength = 0.f;
    vec3 rPos = comPos;
    for(int i = 0; i < 30; i++)
    {
        distance = blockFunc(rPos);
        rLength += distance;
        rPos = comPos + ray * rLength;
    }

    float sw = 1.f - step(0.001f, abs(distance));
    color = clamp(dot(lightDir, getBlockNormal(rPos)), 0.f, 1.f) * 0.4f + 0.1f;
    color *= sw;

    return floor(color * 30.f) / 30.f;
}